// Fill out your copyright notice in the Description page of Project Settings.

#include "PSupremo.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"
// --- NUEVOS INCLUDES PARA NIAGARA ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

APSupremo::APSupremo()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. HITBOX INVISIBLE
	MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPSupremo"));
	RootComponent = MallaProyectil;

	MallaProyectil->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MallaProyectil->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MallaProyectil->SetGenerateOverlapEvents(true); // Forzamos colisiones para que siga detectando el impacto

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Trim_90_Out.Shape_Trim_90_Out'"));
	if (MeshAsset.Succeeded())
	{
		MallaProyectil->SetStaticMesh(MeshAsset.Object);
	}

	// Ocultamos la malla visualmente
	MallaProyectil->SetHiddenInGame(true);
	SetActorScale3D(FVector(0.7f, 0.7f, 0.7f));

	// 2. EFECTO VISUAL NIAGARA
	EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
	EfectoNiagara->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Hit_1.NS_Hit_1'"));
	if (NiagaraAsset.Succeeded())
	{
		EfectoNiagara->SetAsset(NiagaraAsset.Object);
	}

	Velocidad = 1000.f;
}

void APSupremo::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(4.0f);
}

void APSupremo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DireccionHaciaAdelante = GetActorForwardVector();
	DireccionHaciaAdelante.Z = 0.f;
	DireccionHaciaAdelante.Normalize();

	FVector NuevoDesplazamiento = DireccionHaciaAdelante * Velocidad * DeltaTime;
	AddActorWorldOffset(NuevoDesplazamiento, true);
}

void APSupremo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		Cast<ACentCosmosPawn>(OtherActor)->RecibirDanioNave(10.0f);
		Destroy();
	}
}

