#include "GeneradorAmbientacion.h"
#include "ObstaculoNave.h"
#include "ObstaculoSatelite.h"
#include "ObstaculoRestos.h"
#include "Engine/World.h"

AGeneradorAmbientacion::AGeneradorAmbientacion()
{
    PrimaryActorTick.bCanEverTick = false;
    ClaseNave = AObstaculoNave::StaticClass();
    ClaseSatelite = AObstaculoSatelite::StaticClass();
    ClaseRestos = AObstaculoRestos::StaticClass();
}

void AGeneradorAmbientacion::BeginPlay()
{
    Super::BeginPlay();

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    float RangoMin = -2000.0f, RangoMax = 2000.0f;

    // Spawna 30 obstáculos usando FabricarObstaculo — rota entre los 3 tipos internamente
    for (int32 i = 0; i < 30; i++)
    {
        FVector Pos = FVector(FMath::RandRange(RangoMin, RangoMax),
            FMath::RandRange(RangoMin, RangoMax), 150.0f);
        FabricarObstaculo(Pos, FRotator::ZeroRotator);
    }
}

AActor* AGeneradorAmbientacion::FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Rota entre los 3 tipos de obstáculo
    const int32 Tipo = FMath::RandRange(0, 2);

    if (Tipo == 0)
    {
        AObstaculoNave* Obs = Mundo->SpawnActor<AObstaculoNave>(ClaseNave, Posicion, Rotacion, Params);
        if (Obs) Obs->InicializarObstaculo();
        return Obs;
    }
    else if (Tipo == 1)
    {
        AObstaculoSatelite* Obs = Mundo->SpawnActor<AObstaculoSatelite>(ClaseSatelite, Posicion, Rotacion, Params);
        if (Obs) Obs->InicializarObstaculo();
        return Obs;
    }
    else
    {
        AObstaculoRestos* Obs = Mundo->SpawnActor<AObstaculoRestos>(ClaseRestos, Posicion, Rotacion, Params);
        if (Obs) Obs->InicializarObstaculo();
        return Obs;
    }
}

AActor* AGeneradorAmbientacion::FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion)
{
    // GeneradorAmbientacion no produce enemigos
    return nullptr;
}