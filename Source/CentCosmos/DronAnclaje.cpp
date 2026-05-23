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

	// 1. Buscamos el asset estático de la tubería curva
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TuberiaMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe_180.Shape_Pipe_180'"));

	// 2. Si lo encuentra y el componente del padre existe, le asignamos la malla
	if (TuberiaMesh.Succeeded() && EnemigoMesh != nullptr)
	{
		EnemigoMesh->SetStaticMesh(TuberiaMesh.Object);
	}
}

void ADronAnclaje::BeginPlay()
{
	Super::BeginPlay();

	// Timer repetitivo de 10 segundos
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

			if (Nombre.Contains(TEXT("Vastago")) ||
				Nombre.Contains(TEXT("Vigia")) ||
				Nombre.Contains(TEXT("Heraldo")))
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
				NuevoEscudo->AddActorLocalOffset(FVector(0.0f, 0.0f, 40.0f));

				if (GEngine != nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Yellow,
						FString::Printf(TEXT("¡[NIVEL 1] Dron Tubería acopló cubo escudo a: %s!"), *EnemigoElegido->GetName()));
				}
			}
		}
	}
}