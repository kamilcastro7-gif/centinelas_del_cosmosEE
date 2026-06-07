// Fill out your copyright notice in the Description page of Project Settings.

#include "RastrosFuego.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h" // NUEVO: Librería de partículas
#include "../Player/CentCosmosPawn.h"
#include "Engine/World.h"
#include "TimerManager.h"

ARastrosFuego::ARastrosFuego()
{
	PrimaryActorTick.bCanEverTick = false;

	// --- 1. CONFIGURACIÓN DE LA ZONA DE DAÑO (INVISIBLE) ---
	FuegoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuegoMesh"));
	RootComponent = FuegoMesh;

	FuegoMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FuegoMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (PlaneMesh.Succeeded()) FuegoMesh->SetStaticMesh(PlaneMesh.Object);

	// Lo hacemos invisible durante el juego, pero su colisión sigue activa
	FuegoMesh->SetHiddenInGame(true);

	// --- 2. CONFIGURACIÓN DEL EFECTO VISUAL (PARTÍCULA) ---
	EfectoFuego = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EfectoFuego"));
	EfectoFuego->SetupAttachment(RootComponent); // Se pega al centro de la malla invisible

	// Inyectamos la partícula exacta que quieres
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticulaAsset(TEXT("ParticleSystem'/Game/AuraFX01/Particles/P_ky_aura05.P_ky_aura05'"));
	if (ParticulaAsset.Succeeded()) {
		EfectoFuego->SetTemplate(ParticulaAsset.Object);
	}

	JugadorEnFuego = nullptr;
}

void ARastrosFuego::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(4.0f);
}

void ARastrosFuego::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor->IsA(ACentCosmosPawn::StaticClass()))
	{
		JugadorEnFuego = Cast<ACentCosmosPawn>(OtherActor);
		// Inicia el loop de daño: Daño inmediato y luego cada 2 segundos
		GetWorld()->GetTimerManager().SetTimer(TimerDanoContinuo, this, &ARastrosFuego::AplicarDanoContinuo, 2.0f, true, 0.0f);
	}
}

void ARastrosFuego::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (OtherActor && OtherActor == JugadorEnFuego)
	{
		// Si sale del fuego, cancelamos el timer
		GetWorld()->GetTimerManager().ClearTimer(TimerDanoContinuo);
		JugadorEnFuego = nullptr;
	}
}

void ARastrosFuego::AplicarDanoContinuo()
{
	if (JugadorEnFuego)
	{
		JugadorEnFuego->RecibirDanioNave(3.0f); // DAÑO CONSTANTE SEGÚN LISTA
	}
}

