// Fill out your copyright notice in the Description page of Project Settings.
#include "SobreCargaApex.h"
#include "CentCosmosPawn.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Patterns/Decorator/EnemDecorador.h"
#include "Patterns/Decorator/EnemSobrecargaApex.h"
// --- NUEVOS INCLUDES PARA CASCADE ---
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

APowerUpSobrecargaApex::APowerUpSobrecargaApex()
{
	// Esfera verde — velocidad/poder (HITBOX INVISIBLE)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (FormaMesh.Succeeded() && MallaVisual)
	{
		MallaVisual->SetStaticMesh(FormaMesh.Object);
		MallaVisual->SetRelativeScale3D(FVector(0.4f));

		// Ocultamos la malla visualmente
		MallaVisual->SetHiddenInGame(true);
	}

	// EFECTO VISUAL AURA
	EfectoAura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EfectoAura"));
	EfectoAura->SetupAttachment(MallaVisual);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AuraAsset(TEXT("ParticleSystem'/Game/AuraFX01/Particles/P_ky_aura29.P_ky_aura29'"));
	if (AuraAsset.Succeeded())
	{
		EfectoAura->SetTemplate(AuraAsset.Object);
	}
}

void APowerUpSobrecargaApex::AplicarEfecto(ACentCosmosPawn* Nave)
{
	if (!Nave) return;

	UEnemSobrecargaApex* Efecto = NewObject<UEnemSobrecargaApex>(Nave);
	if (Efecto)
	{
		Efecto->AplicarEfecto(Nave);
	}
}