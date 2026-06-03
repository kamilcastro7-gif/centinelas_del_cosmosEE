#include "AGeneradorDificil.h"
#include "ObstaculoRestos.h"
#include "EHeraldo_De_La_Ruina.h"
#include "Engine/World.h"

AGeneradorDificil::AGeneradorDificil()
{
    PrimaryActorTick.bCanEverTick = false;
    ClaseObstaculo = AObstaculoRestos::StaticClass();
    ClaseEnemigo = AEHeraldo_De_La_Ruina::StaticClass();
}

AActor* AGeneradorDificil::FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AObstaculoRestos* Obstaculo = Mundo->SpawnActor<AObstaculoRestos>(
        ClaseObstaculo, Posicion, Rotacion, Params);
    if (Obstaculo) Obstaculo->InicializarObstaculo();
    return Obstaculo;
}

AActor* AGeneradorDificil::FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return Mundo->SpawnActor<AEHeraldo_De_La_Ruina>(ClaseEnemigo, Posicion, Rotacion, Params);
}