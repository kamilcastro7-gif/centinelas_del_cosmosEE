// Fill out your copyright notice in the Description page of Project Settings.

#include "PRafaga.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/CentCosmosPawn.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

APRafaga::APRafaga()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. HITBOX INVISIBLE
	MallaProyectil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPRafaga"));
	RootComponent = MallaProyectil;

	MallaProyectil->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MallaProyectil->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MallaProyectil->SetGenerateOverlapEvents(true); // Forzamos colisiones para evitar que el daño falle al ser invisible

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Trim.Shape_Trim'"));
	if (MeshAsset.Succeeded())
	{
		MallaProyectil->SetStaticMesh(MeshAsset.Object);
	}

	// Ocultamos la malla visualmente
	MallaProyectil->SetHiddenInGame(true);
	SetActorScale3D(FVector(0.8f, 0.8f, 0.8f));

	// 2. EFECTO VISUAL NIAGARA
	EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
	EfectoNiagara->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Energy_5.NS_Energy_5'"));
	if (NiagaraAsset.Succeeded())
	{
		EfectoNiagara->SetAsset(NiagaraAsset.Object);
	}

	Velocidad = 1200.f;
}

void APRafaga::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(4.0f);
}

void APRafaga::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DireccionHaciaAdelante = GetActorForwardVector();
	DireccionHaciaAdelante.Z = 0.f;
	DireccionHaciaAdelante.Normalize();

	FVector NuevoDesplazamiento = DireccionHaciaAdelante * Velocidad * DeltaTime;
	AddActorWorldOffset(NuevoDesplazamiento, true);
}

void APRafaga::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		Cast<ACentCosmosPawn>(OtherActor)->RecibirDanioNave(10.0f);
		Destroy();
	}
}
