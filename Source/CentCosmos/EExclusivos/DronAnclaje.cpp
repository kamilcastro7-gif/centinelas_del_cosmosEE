// Fill out your copyright notice in the Description page of Project Settings.

#include "DronAnclaje.h"
#include "Escudo.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

ADronAnclaje::ADronAnclaje()
{
	NivelExclusivo = 1;
	Vida = 15.0f; // --- VIDA ASIGNADA SEGÚN LISTA ---

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TuberiaMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe_180.Shape_Pipe_180'"));
	if (TuberiaMesh.Succeeded() && EnemigoMesh != nullptr)
	{
		EnemigoMesh->SetStaticMesh(TuberiaMesh.Object);
	}
	SetActorScale3D(FVector(1.7f, 1.7f, 1.7f));
}

void ADronAnclaje::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(TimerEscudoHandle, this, &ADronAnclaje::OtorgarEscudoAEnemigo, 10.0f, true);
	}
}

void ADronAnclaje::OtorgarEscudoAEnemigo()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	TArray<AActor*> EnemigosCandidatos;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* ActorActual = *It;
		if (ActorActual != nullptr && ActorActual != this && !ActorActual->IsPendingKill())
		{
			FString Nombre = ActorActual->GetName();
			if (Nombre.Contains(TEXT("Vastago")) || Nombre.Contains(TEXT("Vigia")) || Nombre.Contains(TEXT("Heraldo")))
			{
				EnemigosCandidatos.Add(ActorActual);
			}
		}
	}

	if (EnemigosCandidatos.Num() > 0)
	{
		int32 IndiceAleatorio = FMath::RandRange(0, EnemigosCandidatos.Num() - 1);
		AActor* EnemigoElegido = EnemigosCandidatos[IndiceAleatorio];

		if (EnemigoElegido != nullptr && !EnemigoElegido->IsPendingKill())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AEscudo* NuevoEscudo = World->SpawnActor<AEscudo>(AEscudo::StaticClass(), EnemigoElegido->GetActorLocation(), EnemigoElegido->GetActorRotation(), SpawnParams);
			if (NuevoEscudo != nullptr)
			{
				NuevoEscudo->AttachToActor(EnemigoElegido, FAttachmentTransformRules::KeepWorldTransform);
				NuevoEscudo->AddActorLocalOffset(FVector(0.0f, 0.0f, 0.0f));
			}
		}
	}
}