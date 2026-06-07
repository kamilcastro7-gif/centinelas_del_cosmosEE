// Fill out your copyright notice in the Description page of Project Settings.

#include "Escudo.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h" // NUEVO: Para crear el componente
#include "Particles/ParticleSystem.h"          // NUEVO: Para cargar el asset
#include "Engine/Engine.h"

AEscudo::AEscudo()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. EL TRUCO DE LA HITBOX: Creamos el cubo base
	EscudoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EscudoMesh"));
	RootComponent = EscudoMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CuboMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (CuboMesh.Succeeded())
	{
		EscudoMesh->SetStaticMesh(CuboMesh.Object);
	}

	// Lo volvemos invisible pero mantenemos su colisión sólida
	EscudoMesh->SetHiddenInGame(true);
	EscudoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EscudoMesh->SetCollisionProfileName(TEXT("BlockAll")); // Mantenemos tu perfil original para que bloquee

	// Aumentamos el tamaño de la hitbox del escudo
	SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));

	VidaEscudo = 10.0f;

	// 2. EFECTO VISUAL: Añadimos el sistema de partículas (El Aura)
	EscudoParticulas = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EscudoParticulas"));
	if (RootComponent) {
		EscudoParticulas->SetupAttachment(RootComponent); // Lo pegamos al cubo invisible
	}

	// Cargamos el Asset de partículas de tu aura
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticulaAsset(TEXT("ParticleSystem'/Game/AuraFX01/Particles/P_ky_aura10.P_ky_aura10'"));
	if (ParticulaAsset.Succeeded())
	{
		EscudoParticulas->SetTemplate(ParticulaAsset.Object);
	}

	// Opcional: Si el aura se ve muy exagerada o muy pequeña respecto al cubo, 
	// puedes ajustar su tamaño visual aquí sin afectar las colisiones.
	EscudoParticulas->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AEscudo::BeginPlay()
{
	Super::BeginPlay();
}

void AEscudo::RecibirDanoEscudo(float CantidadDano)
{
	VidaEscudo -= CantidadDano;

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange,
			FString::Printf(TEXT("¡Escudo Impactado! Vida restante: %.0f"), VidaEscudo));
	}

	if (VidaEscudo <= 0.0f)
	{
		Destroy();
	}
}

