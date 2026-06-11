// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyFactory.h"
#include "EVastago_Del_infierno.h"
#include "EVigia_Del_Vacio.h"
#include "EHeraldo_De_La_Ruina.h"
#include "Engine/World.h"

AEnemyFactory::AEnemyFactory()
{
    PrimaryActorTick.bCanEverTick = false;

    ClaseVastago = AEVastago_Del_infierno::StaticClass();
    ClaseVigia = AEVigia_Del_Vacio::StaticClass();
    ClaseHeraldo = AEHeraldo_De_La_Ruina::StaticClass();
}

AActor* AEnemyFactory::SpawnEnemigo(ETipoEnemigo Tipo, const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    switch (Tipo)
    {
    case ETipoEnemigo::Vastago:
        return Mundo->SpawnActor<AEVastago_Del_infierno>(ClaseVastago, Posicion, Rotacion, Params);

    case ETipoEnemigo::Vigia:
        return Mundo->SpawnActor<AEVigia_Del_Vacio>(ClaseVigia, Posicion, Rotacion, Params);

    case ETipoEnemigo::Heraldo:
        return Mundo->SpawnActor<AEHeraldo_De_La_Ruina>(ClaseHeraldo, Posicion, Rotacion, Params);

    default:
        return nullptr;
    }
}