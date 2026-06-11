// Fill out your copyright notice in the Description page of Project Settings.
#include "NucleoEter.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h"
#include "Engine/Engine.h"
// --- NUEVOS INCLUDES PARA CASCADE ---
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

ANucleoEter::ANucleoEter()
{
	CantidadCuracion = 50.0f;

	// Cubo cian — curacion (HITBOX INVISIBLE)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (FormaMesh.Succeeded() && MallaVisual)
	{
		MallaVisual->SetStaticMesh(FormaMesh.Object);
		MallaVisual->SetRelativeScale3D(FVector(0.3f));

		// Ocultamos la malla visualmente
		MallaVisual->SetHiddenInGame(true);
	}

	// EFECTO VISUAL AURA
	EfectoAura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EfectoAura"));
	EfectoAura->SetupAttachment(MallaVisual);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AuraAsset(TEXT("ParticleSystem'/Game/AuraFX01/Particles/P_ky_aura25.P_ky_aura25'"));
	if (AuraAsset.Succeeded())
	{
		EfectoAura->SetTemplate(AuraAsset.Object);
	}
}

void ANucleoEter::AplicarEfecto(ACentCosmosPawn* Nave)
{
	if (!Nave) return;

	// RestaurarVida conecta con el Decorator y notifica al Observer
	Nave->RestaurarVida(CantidadCuracion);
}