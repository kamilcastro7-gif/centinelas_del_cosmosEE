// Copyright Epic Games, Inc. All Rights Reserved.
#include "Centinela_Del_CosmosGameMode.h"
#include "EnemyFactory.h"
#include "Facade.h"
#include "Engine/World.h"

ACentinela_Del_CosmosGameMode::ACentinela_Del_CosmosGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    Factory = nullptr;
    ManejadorHorda = nullptr;
}

void ACentinela_Del_CosmosGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    Factory = Mundo->SpawnActor<AEnemyFactory>(AEnemyFactory::StaticClass());
    ManejadorHorda = Mundo->SpawnActor<AFacade>(AFacade::StaticClass());

    if (!Factory) return;

    const FVector   Origen = FVector(0.0f, 0.0f, 150.0f);
    const FRotator  SinRotacion = FRotator::ZeroRotator;

    Factory->SpawnEnemigo(ETipoEnemigo::Vastago, Origen + FVector(200, 0, 0), SinRotacion);
    Factory->SpawnEnemigo(ETipoEnemigo::Vigia, Origen + FVector(-200, 0, 0), SinRotacion);
    Factory->SpawnEnemigo(ETipoEnemigo::Heraldo, Origen + FVector(0, 300, 0), SinRotacion);
}

void ACentinela_Del_CosmosGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ManejadorHorda)
    {
        ManejadorHorda->MoverHorda();
    }
}