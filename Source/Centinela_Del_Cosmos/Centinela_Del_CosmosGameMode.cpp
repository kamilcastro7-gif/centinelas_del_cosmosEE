// Copyright Epic Games, Inc. All Rights Reserved.

#include "Centinela_Del_CosmosGameMode.h"
#include "EVastago_Del_infierno.h"
#include "EVigia_Del_Vacio.h"
#include "EHeraldo_De_La_Ruina.h"
#include "Facade.h"
#include "Engine/World.h"

ACentinela_Del_CosmosGameMode::ACentinela_Del_CosmosGameMode()
{
    PrimaryActorTick.bCanEverTick = true;

    ClaseVastago = AEVastago_Del_infierno::StaticClass();
    ClaseVigia = AEVigia_Del_Vacio::StaticClass();
    ClaseHeraldo = AEHeraldo_De_La_Ruina::StaticClass();
}

void ACentinela_Del_CosmosGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* const Mundo = GetWorld();
    if (Mundo)
    {
        FActorSpawnParameters ParametrosSpawn;
        ParametrosSpawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        // Origen central para el combate
        FVector Origen(0.0f, 0.0f, 150.0f);

        // Spawn Vástago (Esfera)
        Mundo->SpawnActor<AEVastago_Del_infierno>(ClaseVastago, Origen + FVector(200, 0, 0), FRotator::ZeroRotator, ParametrosSpawn);

        // Spawn Vigía (Toroide - Ahora se moverá en Y)
        Mundo->SpawnActor<AEVigia_Del_Vacio>(ClaseVigia, Origen + FVector(-200, 0, 0), FRotator::ZeroRotator, ParametrosSpawn);

        // Spawn Heraldo (Cubo)
        Mundo->SpawnActor<AEHeraldo_De_La_Ruina>(ClaseHeraldo, Origen + FVector(0, 300, 0), FRotator::ZeroRotator, ParametrosSpawn);

        // Spawneamos la Fachada
        ManejadorHorda = Mundo->SpawnActor<AFacade>(AFacade::StaticClass());
    }
}

void ACentinela_Del_CosmosGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ManejadorHorda)
    {
        ManejadorHorda->MoverHorda();
    }
}

