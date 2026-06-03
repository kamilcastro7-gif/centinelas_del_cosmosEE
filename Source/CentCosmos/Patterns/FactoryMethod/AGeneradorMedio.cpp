#include "AGeneradorMedio.h"
#include "ObstaculoSatelite.h"
#include "EVigia_Del_Vacio.h"
#include "Engine/World.h"

AGeneradorMedio::AGeneradorMedio()
{
    PrimaryActorTick.bCanEverTick = false;
    ClaseObstaculo = AObstaculoSatelite::StaticClass();
    ClaseEnemigo = AEVigia_Del_Vacio::StaticClass();
}

AActor* AGeneradorMedio::FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AObstaculoSatelite* Obstaculo = Mundo->SpawnActor<AObstaculoSatelite>(ClaseObstaculo, Posicion, Rotacion, Params);
    if (Obstaculo) Obstaculo->InicializarObstaculo();
    return Obstaculo;
}

AActor* AGeneradorMedio::FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return Mundo->SpawnActor<AEVigia_Del_Vacio>(ClaseEnemigo, Posicion, Rotacion, Params);
}