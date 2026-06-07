// Fill out your copyright notice in the Description page of Project Settings.

#include "DronAnclaje.h"
#include "Escudo.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h" 
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

ADronAnclaje::ADronAnclaje()
{
	NivelExclusivo = 1;
	Vida = 15.0f;

	// 1. EL TRUCO DE LA HITBOX: La Tubería será nuestra colisión invisible
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TuberiaMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe_180.Shape_Pipe_180'"));
	if (TuberiaMesh.Succeeded() && EnemigoMesh != nullptr)
	{
		EnemigoMesh->SetStaticMesh(TuberiaMesh.Object);

		EnemigoMesh->SetHiddenInGame(true);
		EnemigoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		EnemigoMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		EnemigoMesh->SetGenerateOverlapEvents(true);

		// --- CAMBIO 1: AUMENTAMOS LA HITBOX INVISIBLE ---
		// Antes estaba en 1.7f, ahora es 3.0f para asegurar que reciba bien los golpes.
		EnemigoMesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	}

	// 2. MALLA VISUAL: El Fighter Drone 
	MallaFighterDrone = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MallaFighterDrone"));
	if (RootComponent) {
		MallaFighterDrone->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DroneAsset(TEXT("SkeletalMesh'/Game/Fighter_Drone/mesh/fighter_drone_mesh.fighter_drone_mesh'"));
	if (DroneAsset.Succeeded())
	{
		MallaFighterDrone->SetSkeletalMesh(DroneAsset.Object);
	}

	// Escala visual del Dron
	MallaFighterDrone->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	// --- LA SOLUCIÓN: CENTRAR EL DRON EN LA HITBOX ---
	// Bajamos el dron en el Eje Z para que quede dentro del tubo. 
	// Prueba con -100.0f. Si baja demasiado, ponle -50.0f. Si le falta bajar, ponle -150.0f.
	MallaFighterDrone->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	MallaFighterDrone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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