// Copyright Epic Games, Inc. All Rights Reserved.
#include "CentCosmosGameMode.h"
#include "CentCosmosPawn.h"
#include "CentCosmos.h"
#include "Facade.h"
#include "Engine/World.h"
#include "Patterns/Builder/NivelDirector.h"
#include "Patterns/Builder/NivelFacilBuilder.h"

ACentCosmosGameMode::ACentCosmosGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = ACentCosmosPawn::StaticClass();
	Director = nullptr;
	BuilderFacil = nullptr;
	ManejadorHorda = nullptr;
}

void ACentCosmosGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogCentCosmos, Log, TEXT("[CentCosmos] BeginPlay ejecutado en %s"), *GetName());

	UWorld* const Mundo = GetWorld();
	if (!Mundo) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawneamos el Director y el Builder
	Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	BuilderFacil = Mundo->SpawnActor<ANivelFacilBuilder>(ANivelFacilBuilder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	ManejadorHorda = Mundo->SpawnActor<AFacade>(AFacade::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (Director && BuilderFacil)
	{
		// El Director recibe el builder y construye el nivel completo
		Director->SetBuilder(TScriptInterface<INivelBuilder>(BuilderFacil));
		Director->ConstruirNivel(Mundo, TEXT("Nivel_Facil"), 300.0f, 1.0f);
	}
}

void ACentCosmosGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ManejadorHorda)
	{
		ManejadorHorda->MoverHorda();
	}
}