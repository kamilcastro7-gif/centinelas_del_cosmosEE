// Fill out your copyright notice in the Description page of Project Settings.
#include "GeneradorAmbientacion.h"
#include "ObstaculoNave.h"
#include "ObstaculoSatelite.h"
#include "ObstaculoRestos.h"
#include "Asteroides.h"
#include "NubeIonizada.h"
#include "FragmentoErupcion.h"
#include "GrietaAntimateria.h"
#include "PAntimateria.h"
#include "RastrosFuego.h"
#include "EclipseSilencioso.h"
#include "Engine/World.h"


AGeneradorAmbientacion::AGeneradorAmbientacion()
{
    PrimaryActorTick.bCanEverTick = false;

    ClaseNave = AObstaculoNave::StaticClass();
    ClaseSatelite = AObstaculoSatelite::StaticClass();
    ClaseRestos = AObstaculoRestos::StaticClass();
    ClaseAsteroide = AAsteroide::StaticClass();
    ClaseNubeIonizada = ANubeIonizada::StaticClass();
    ClaseFragmentoErupcion = AFragmentoErupcion::StaticClass();
    ClaseGrietaAntimateria = AGrietaAntimateria::StaticClass();
    ClasePAntimateria = APAntimateria::StaticClass();
    ClaseRastrosFuego = ARastrosFuego::StaticClass();
    ClaseEclipseSilencioso = AEclipseSilencioso::StaticClass();
}
AActor* AGeneradorAmbientacion::FabricarEclipseSilencioso(
    const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    return Mundo->SpawnActor<AEclipseSilencioso>(
        ClaseEclipseSilencioso, Posicion, Rotacion, Params);
}


AActor* AGeneradorAmbientacion::FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion)
{
    // El builder llama los métodos individuales — este queda vacío
    return nullptr;
}

AActor* AGeneradorAmbientacion::FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion)
{
    // GeneradorAmbientacion no produce enemigos
    return nullptr;
}

AActor* AGeneradorAmbientacion::FabricarNave(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AObstaculoNave* Obs = Mundo->SpawnActor<AObstaculoNave>(ClaseNave, Posicion, Rotacion, Params);
    if (Obs) Obs->InicializarObstaculo();
    return Obs;
}

AActor* AGeneradorAmbientacion::FabricarSatelite(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AObstaculoSatelite* Obs = Mundo->SpawnActor<AObstaculoSatelite>(ClaseSatelite, Posicion, Rotacion, Params);
    if (Obs) Obs->InicializarObstaculo();
    return Obs;
}

AActor* AGeneradorAmbientacion::FabricarRestos(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AObstaculoRestos* Obs = Mundo->SpawnActor<AObstaculoRestos>(ClaseRestos, Posicion, Rotacion, Params);
    if (Obs) Obs->InicializarObstaculo();
    return Obs;
}

AActor* AGeneradorAmbientacion::FabricarAsteroide(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return Mundo->SpawnActor<AAsteroide>(ClaseAsteroide, Posicion, Rotacion, Params);
}

AActor* AGeneradorAmbientacion::FabricarNubeIonizada(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return Mundo->SpawnActor<ANubeIonizada>(ClaseNubeIonizada, Posicion, Rotacion, Params);
}

AActor* AGeneradorAmbientacion::FabricarFragmentoErupcion(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return Mundo->SpawnActor<AFragmentoErupcion>(ClaseFragmentoErupcion, Posicion, Rotacion, Params);
}

AActor* AGeneradorAmbientacion::FabricarGrietaAntimateria(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return Mundo->SpawnActor<AGrietaAntimateria>(ClaseGrietaAntimateria, Posicion, Rotacion, Params);
}

AActor* AGeneradorAmbientacion::FabricarPAntimateria(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return Mundo->SpawnActor<APAntimateria>(ClasePAntimateria, Posicion, Rotacion, Params);
}

AActor* AGeneradorAmbientacion::FabricarRastrosFuego(const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return Mundo->SpawnActor<ARastrosFuego>(ClaseRastrosFuego, Posicion, Rotacion, Params);
}