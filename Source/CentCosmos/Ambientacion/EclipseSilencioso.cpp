// Fill out your copyright notice in the Description page of Project Settings.

#include "EclipseSilencioso.h"
#include "../Player/CentCosmosPawn.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
// --- NUEVOS INCLUDES PARA CASCADE ---
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

AEclipseSilencioso::AEclipseSilencioso()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. HITBOX INVISIBLE
	EclipseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EclipseMesh"));
	RootComponent = EclipseMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
	if (MeshAsset.Succeeded())
	{
		EclipseMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Ocultamos el cono visualmente
	EclipseMesh->SetHiddenInGame(true);

	// --- ACTIVAMOS EL SENSOR DE COLISIÓN ---
	EclipseMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	EclipseMesh->SetGenerateOverlapEvents(true);

	// 2. EFECTO VISUAL (AURA CASCADE)
	EfectoAura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EfectoAura"));
	EfectoAura->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AuraAsset(TEXT("ParticleSystem'/Game/AuraFX01/Particles/P_ky_aura26.P_ky_aura26'"));
	if (AuraAsset.Succeeded())
	{
		EfectoAura->SetTemplate(AuraAsset.Object);
	}
}

void AEclipseSilencioso::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(20.0f);
	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AEclipseSilencioso::RealizarDash, TiempoEntreDash, true);
}

void AEclipseSilencioso::RealizarDash()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn)
	{
		FVector Dir = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector NuevaPos = GetActorLocation() + (Dir * DistanciaPorDash);
		SetActorLocation(NuevaPos);
	}
}

void AEclipseSilencioso::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ACentCosmosPawn* Nave = Cast<ACentCosmosPawn>(OtherActor);
	if (Nave)
	{
		// 1. Silenciamos la nave
		Nave->bPuedeDisparar = false;

		// 2. Escondemos el Eclipse y apagamos su colisión (parecerá que se destruyó)
		// ¡Esto ocultará automáticamente el EfectoAura!
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		GetWorldTimerManager().ClearTimer(DashTimerHandle); // Detenemos el dash

		// 3. Lanzamos el Timer. Como el Eclipse sigue vivo de fondo, esto SÍ funcionará
		FTimerHandle SilencioHandle;
		GetWorldTimerManager().SetTimer(SilencioHandle, [Nave]()
			{
				if (IsValid(Nave))
				{
					Nave->bPuedeDisparar = true; // Le devolvemos el disparo
				}
			}, 5.0f, false);

		// 4. Lo destruimos 6 segundos después (para dar tiempo a que el timer termine)
		SetLifeSpan(6.0f);
	}
}