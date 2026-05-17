// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFactory.generated.h"

UENUM(BlueprintType)
enum class ETipoEnemigo : uint8
{
    Vastago,
    Vigia,
    Heraldo
};

UCLASS()
class CENTCOSMOS_API AEnemyFactory : public AActor
{
    GENERATED_BODY()
public:
    AEnemyFactory();
    AActor* SpawnEnemigo(ETipoEnemigo Tipo, const FVector& Posicion, const FRotator& Rotacion);

private:
    TSubclassOf<class AEVastago_Del_infierno> ClaseVastago;
    TSubclassOf<class AEVigia_Del_Vacio>      ClaseVigia;
    TSubclassOf<class AEHeraldo_De_La_Ruina>  ClaseHeraldo;
};