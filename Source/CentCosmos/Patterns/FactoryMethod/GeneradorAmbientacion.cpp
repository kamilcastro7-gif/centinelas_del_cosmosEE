// Fill out your copyright notice in the Description page of Project Settings.
#include "GeneradorAmbientacion.h"

// Obstáculos originales
#include "ObstaculoNave.h"
#include "ObstaculoSatelite.h"
#include "ObstaculoRestos.h"

// Obstáculos nuevos
#include "Asteroides.h"
#include "NubeIonizada.h"
#include "FragmentoErupcion.h"
#include "GrietaAntimateria.h"
#include "PAntimateria.h"
#include "RastrosFuego.h"

#include "Engine/World.h"

AGeneradorAmbientacion::AGeneradorAmbientacion()
{
    PrimaryActorTick.bCanEverTick = false;

    // Obstáculos originales
    ClaseNave = AObstaculoNave::StaticClass();
    ClaseSatelite = AObstaculoSatelite::StaticClass();
    ClaseRestos = AObstaculoRestos::StaticClass();

    // Obstáculos nuevos
    ClaseAsteroide = AAsteroide::StaticClass();
    ClaseNubeIonizada = ANubeIonizada::StaticClass();
    ClaseFragmentoErupcion = AFragmentoErupcion::StaticClass();
    ClaseGrietaAntimateria = AGrietaAntimateria::StaticClass();
    ClasePAntimateria = APAntimateria::StaticClass();
    ClaseRastrosFuego = ARastrosFuego::StaticClass();
}

void AGeneradorAmbientacion::BeginPlay()
{
    Super::BeginPlay();
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    float RangoMin = -2000.0f, RangoMax = 2000.0f;

    // Spawna 30 obstáculos rotando entre todos los tipos
    for (int32 i = 0; i < 30; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(RangoMin, RangoMax),
            FMath::RandRange(RangoMin, RangoMax),
            150.0f);
        FabricarObstaculo(Pos, FRotator::ZeroRotator);
    }
}

AActor* AGeneradorAmbientacion::FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Rota entre los 9 tipos de obstáculo
    const int32 Tipo = FMath::RandRange(0, 8);

    switch (Tipo)
    {
    case 0:
    {
        AObstaculoNave* Obs = Mundo->SpawnActor<AObstaculoNave>(ClaseNave, Posicion, Rotacion, Params);
        if (Obs) Obs->InicializarObstaculo();
        return Obs;
    }
    case 1:
    {
        AObstaculoSatelite* Obs = Mundo->SpawnActor<AObstaculoSatelite>(ClaseSatelite, Posicion, Rotacion, Params);
        if (Obs) Obs->InicializarObstaculo();
        return Obs;
    }
    case 2:
    {
        AObstaculoRestos* Obs = Mundo->SpawnActor<AObstaculoRestos>(ClaseRestos, Posicion, Rotacion, Params);
        if (Obs) Obs->InicializarObstaculo();
        return Obs;
    }
    case 3:
        return Mundo->SpawnActor<AAsteroide>(ClaseAsteroide, Posicion, Rotacion, Params);
    case 4:
        return Mundo->SpawnActor<ANubeIonizada>(ClaseNubeIonizada, Posicion, Rotacion, Params);
    case 5:
        return Mundo->SpawnActor<AFragmentoErupcion>(ClaseFragmentoErupcion, Posicion, Rotacion, Params);
    case 6:
        return Mundo->SpawnActor<AGrietaAntimateria>(ClaseGrietaAntimateria, Posicion, Rotacion, Params);
    case 7:
        return Mundo->SpawnActor<APAntimateria>(ClasePAntimateria, Posicion, Rotacion, Params);
    case 8:
        return Mundo->SpawnActor<ARastrosFuego>(ClaseRastrosFuego, Posicion, Rotacion, Params);
    default:
        return nullptr;
    }
}

AActor* AGeneradorAmbientacion::FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion)
{
    // GeneradorAmbientacion no produce enemigos
    return nullptr;
}