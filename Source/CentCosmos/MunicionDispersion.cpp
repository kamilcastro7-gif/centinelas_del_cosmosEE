// Fill out your copyright notice in the Description page of Project Settings.
#include "MunicionDispersion.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CentCosmosPawn.h"
#include "Engine/Engine.h"
#include "Patterns/Decorator/EnemDecorador.h"
#include "Patterns/Decorator/EnemDisparoTriple.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

AMunicionDispersion::AMunicionDispersion()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FormaMesh(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (FormaMesh.Succeeded() && MallaVisual)
	{
		MallaVisual->SetStaticMesh(FormaMesh.Object);
		MallaVisual->SetRelativeScale3D(FVector(0.3f));

		MallaVisual->SetHiddenInGame(true);
	}
	EfectoAura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EfectoAura"));
	EfectoAura->SetupAttachment(MallaVisual);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AuraAsset(TEXT("ParticleSystem'/Game/AuraFX01/Particles/P_ky_aura15.P_ky_aura15'"));
	if (AuraAsset.Succeeded())
	{
		EfectoAura->SetTemplate(AuraAsset.Object);
	}
}

void AMunicionDispersion::AplicarEfecto(ACentCosmosPawn* Nave)
{
	if (!Nave) return;

	// 1. Instanciamos el decorador puro
	UEnemDisparoTriple* DecoradorTriple = NewObject<UEnemDisparoTriple>(Nave);
	if (DecoradorTriple)
	{
		// 2. Envolvemos la cadena de la nave
		Nave->AgregarDecorador(DecoradorTriple);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Disparo Triple: Decorador Puro Aplicado"));

		// 3. Temporizador para quitar el efecto
		FTimerHandle Handle;
		Nave->GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([Nave, DecoradorTriple]()
			{
				if (IsValid(Nave)) Nave->RemoverDecorador(DecoradorTriple);
			}), 10.f, false);
	}
}