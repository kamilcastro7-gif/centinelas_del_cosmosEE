#include "AGeneradorFacil.h"
#include "ObstaculoNave.h"
#include "EVastago_Del_infierno.h"
#include "Engine/World.h"

AGeneradorFacil::AGeneradorFacil()
{
    PrimaryActorTick.bCanEverTick = false;
    ClaseObstaculo = AObstaculoNave::StaticClass();
    ClaseEnemigo = AEVastago_Del_infierno::StaticClass();
}

AActor* AGeneradorFacil::FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AObstaculoNave* Obstaculo = Mundo->SpawnActor<AObstaculoNave>(ClaseObstaculo, Posicion, Rotacion, Params);
    if (Obstaculo) Obstaculo->InicializarObstaculo();
    return Obstaculo;
}

AActor* AGeneradorFacil::FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    return Mundo->SpawnActor<AEVastago_Del_infierno>(ClaseEnemigo, Posicion, Rotacion, Params);
}