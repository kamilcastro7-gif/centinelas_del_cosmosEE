// Fill out your copyright notice in the Description page of Project Settings.
#include "SobreCargaApex.h"
#include "CentCosmosPawn.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Patterns/Decorator/EnemDecorador.h"
#include "Patterns/Decorator/EnemSobrecargaApex.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

APowerUpSobrecargaApex::APowerUpSobrecargaApex()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (FormaMesh.Succeeded() && MallaVisual)
	{
		MallaVisual->SetStaticMesh(FormaMesh.Object);
		MallaVisual->SetRelativeScale3D(FVector(0.4f));

		MallaVisual->SetHiddenInGame(true);
	}

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

	// 1. Instanciamos el decorador puro
	UEnemSobrecargaApex* DecoradorApex = NewObject<UEnemSobrecargaApex>(Nave);
	if (DecoradorApex)
	{
		// 2. Le decimos a la nave que se envuelva con este nuevo decorador
		Nave->AgregarDecorador(DecoradorApex);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Sobrecarga Apex: Decorador Puro Aplicado"));

		// 3. El PowerUp inicia el temporizador para remover el decorador en 10 segundos
		FTimerHandle Handle;
		Nave->GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([Nave, DecoradorApex]()
			{
				if (IsValid(Nave)) Nave->RemoverDecorador(DecoradorApex);
			}), 10.f, false);
	}
}