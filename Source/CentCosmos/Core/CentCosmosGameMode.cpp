// Copyright Epic Games, Inc. All Rights Reserved.

#include "CentCosmosGameMode.h"
#include "CentCosmosPawn.h" 
#include "CentCosmos.h"
#include "EnemyFactory.h"
#include "Facade.h" 
#include "ObstaculoNave.h"
#include "ObstaculoSatelite.h"
#include "ObstaculoRestos.h"
#include "Engine/World.h"

ACentCosmosGameMode::ACentCosmosGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    Factory = nullptr;
    ManejadorHorda = nullptr;
    DefaultPawnClass = ACentCosmosPawn::StaticClass();

    // Vinculamos las clases nativas
    ClaseNave = AObstaculoNave::StaticClass();
    ClaseSatelite = AObstaculoSatelite::StaticClass();
    ClaseRestos = AObstaculoRestos::StaticClass();
}

void ACentCosmosGameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogCentCosmos, Log, TEXT("[CentCosmos] BeginPlay ejecutado en %s"), *GetName());

    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    // --- GENERACI�N DE ENEMIGOS DE TU AMIGO ---
    Factory = Mundo->SpawnActor<AEnemyFactory>(AEnemyFactory::StaticClass());
    ManejadorHorda = Mundo->SpawnActor<AFacade>(AFacade::StaticClass());

    if (Factory)
    {
        for (int32 i = 0; i < 15; i++) {
            FVector PosAleatoria = FVector(FMath::RandRange(-1000, 1000), FMath::RandRange(-1000, 1000), 150.0f);
            Factory->SpawnEnemigo(ETipoEnemigo::Vastago, PosAleatoria, FRotator::ZeroRotator);
        }

        const FVector Origen = FVector(0.0f, 0.0f, 150.0f);
        const FRotator SinRotacion = FRotator::ZeroRotator;

        Factory->SpawnEnemigo(ETipoEnemigo::Vastago, Origen + FVector(200, 0, 0), SinRotacion);
        Factory->SpawnEnemigo(ETipoEnemigo::Vigia, Origen + FVector(-200, 0, 0), SinRotacion);
        Factory->SpawnEnemigo(ETipoEnemigo::Heraldo, Origen + FVector(0, 300, 0), SinRotacion);
    }

    // --- AQU� VA: GENERACI�N DE TUS 30 OBST�CULOS DISPERSOS EN RANGO AMPLIO ---
    for (int32 i = 0; i < 10; i++)
    {
        // Rango expandido para evitar que se amontonen y permitir que sobresalgan del l�mite actual
        FVector PosNave = FVector(FMath::RandRange(-2500, 2500), FMath::RandRange(-2500, 2500), 150.0f);
        FVector PosSatelite = FVector(FMath::RandRange(-2500, 2500), FMath::RandRange(-2500, 2500), 150.0f);
        FVector PosRestos = FVector(FMath::RandRange(-2500, 2500), FMath::RandRange(-2500, 2500), 150.0f);

        // Rotaci�n aleatoria en el eje Z (Yaw) para que no queden todos sim�tricos
        FRotator RotAleatoriaNave = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);
        FRotator RotAleatoriaSat = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);
        FRotator RotAleatoriaRes = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

        FabricarObstaculoNave(Mundo, PosNave, RotAleatoriaNave);
        FabricarObstaculoSatelite(Mundo, PosSatelite, RotAleatoriaSat);
        FabricarObstaculoRestos(Mundo, PosRestos, RotAleatoriaRes);
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

AActor* ACentCosmosGameMode::FabricarObstaculoNave(UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion)
{
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AObstaculoNave* Obstaculo = Mundo->SpawnActor<AObstaculoNave>(ClaseNave, Posicion, Rotacion, Params);
    if (Obstaculo)
    {
        Obstaculo->InicializarObstaculo();
        return Obstaculo;
    }
    return nullptr;
}

AActor* ACentCosmosGameMode::FabricarObstaculoSatelite(UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion)
{
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AObstaculoSatelite* Obstaculo = Mundo->SpawnActor<AObstaculoSatelite>(ClaseSatelite, Posicion, Rotacion, Params);
    if (Obstaculo)
    {
        Obstaculo->InicializarObstaculo();
        return Obstaculo;
    }
    return nullptr;
}

AActor* ACentCosmosGameMode::FabricarObstaculoRestos(UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion)
{
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AObstaculoRestos* Obstaculo = Mundo->SpawnActor<AObstaculoRestos>(ClaseRestos, Posicion, Rotacion, Params);
    if (Obstaculo)
    {
        Obstaculo->InicializarObstaculo();
        return Obstaculo;
    }
    return nullptr;
}
