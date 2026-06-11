// Fill out your copyright notice in the Description page of Project Settings.

#include "ProyectilEspectro.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "../Player/CentCosmosPawn.h"
// --- NUEVOS INCLUDES PARA NIAGARA ---
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AProyectilEspectro::AProyectilEspectro()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. HITBOX INVISIBLE: La esfera base
	ProyectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProyectilMesh"));
	RootComponent = ProyectilMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMesh.Succeeded())
	{
		ProyectilMesh->SetStaticMesh(SphereMesh.Object);
	}

	// Ocultamos la esfera visualmente
	ProyectilMesh->SetHiddenInGame(true);
	ProyectilMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProyectilMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	SetActorScale3D(FVector(0.4f, 0.4f, 0.4f));

	// 2. EFECTO VISUAL NIAGARA
	EfectoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EfectoNiagara"));
	EfectoNiagara->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("NiagaraSystem'/Game/sA_Rayvfx/Fx/NiagaraSystems/NS_Energy_3.NS_Energy_3'"));
	if (NiagaraAsset.Succeeded())
	{
		EfectoNiagara->SetAsset(NiagaraAsset.Object);
	}

	// Velocidad del proyectil (Ráfaga veloz)
	Velocidad = 2300.0f;
}

void AProyectilEspectro::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3.0f); // Límite para que no viaje al infinito
}

void AProyectilEspectro::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NuevaPosicion = GetActorLocation() + (GetActorForwardVector() * Velocidad * DeltaTime);
	SetActorLocation(NuevaPosicion);
}

void AProyectilEspectro::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
		if (Nave)
		{
			Nave->RecibirDanioNave(15.0f); // DAÑO SEGÚN LISTA
			Destroy();
		}
	}
}

