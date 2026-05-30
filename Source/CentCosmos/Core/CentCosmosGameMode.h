// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CentCosmosPawn.h"  
#include "CentCosmosGameMode.generated.h"


UCLASS()
class CENTCOSMOS_API ACentCosmosGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACentCosmosGameMode();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    // MÉTODOS DE FABRICACIÓN DECLARADOS CORRECTAMENTE
    AActor* FabricarObstaculoNave(UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarObstaculoSatelite(UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarObstaculoRestos(UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion);

private:
    UPROPERTY()
    class AEnemyFactory* Factory;

    UPROPERTY()
    class AFacade* ManejadorHorda;

    // VARIABLES RECONOCIDAS POR EL COMPILADOR
    TSubclassOf<class AObstaculoNave> ClaseNave;
    TSubclassOf<class AObstaculoSatelite> ClaseSatelite;
    TSubclassOf<class AObstaculoRestos> ClaseRestos;
};



