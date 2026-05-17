// Copyright Epic Games, Inc. All Rights Reserved.

#include "CentCosmosGameMode.h"
#include "EnemyFactory.h"
#include "Facade.h"
#include "Engine/World.h"

ACentCosmosGameMode::ACentCosmosGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    Factory = nullptr;
    ManejadorHorda = nullptr;
}

void ACentCosmosGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    Factory = Mundo->SpawnActor<AEnemyFactory>(AEnemyFactory::StaticClass());
    ManejadorHorda = Mundo->SpawnActor<AFacade>(AFacade::StaticClass());

    if (!Factory) return;

    // En ACentCosmosGameMode::BeginPlay()
    for (int32 i = 0; i < 15; i++) {
        FVector PosAleatoria = FVector(FMath::RandRange(-1000, 1000), FMath::RandRange(-1000, 1000), 150.0f);
        Factory->SpawnEnemigo(ETipoEnemigo::Vastago, PosAleatoria, FRotator::ZeroRotator);
    }

    const FVector    Origen = FVector(0.0f, 0.0f, 150.0f);
    const FRotator   SinRotacion = FRotator::ZeroRotator;

    Factory->SpawnEnemigo(ETipoEnemigo::Vastago, Origen + FVector(200, 0, 0), SinRotacion);
    Factory->SpawnEnemigo(ETipoEnemigo::Vigia, Origen + FVector(-200, 0, 0), SinRotacion);
    Factory->SpawnEnemigo(ETipoEnemigo::Heraldo, Origen + FVector(0, 300, 0), SinRotacion);
}

void ACentCosmosGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ManejadorHorda)
    {
        ManejadorHorda->MoverHorda();
    }
}
