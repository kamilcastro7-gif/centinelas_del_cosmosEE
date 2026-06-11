// Fill out your copyright notice in the Description page of Project Settings.

#include "Facade.h"
#include "Engine/World.h"
#include "GestorNiveles.h" 
#include "Patterns/Builder/NivelDirector.h"
#include "Patterns/Builder/Nivel1Builder.h"
#include "Nivel2Builder.h"
#include "Nivel3Builder.h"
#include "Nivel4Builder.h"
#include "Nivel5Builder.h"
#include "Nivel6Builder.h"

AFacade::AFacade()
{
    PrimaryActorTick.bCanEverTick = false;
    GestorNiveles = nullptr;
}

void AFacade::BeginPlay()
{
    Super::BeginPlay();

    UWorld* const Mundo = GetWorld();
    if (Mundo)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GestorNiveles = Mundo->SpawnActor<AGestorNiveles>(AGestorNiveles::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    }
}

void AFacade::GenerarNivel1()
{
    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ANivelDirector* Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    ANivel1Builder* Builder = Mundo->SpawnActor<ANivel1Builder>(ANivel1Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && Builder)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(Builder));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_1_Facil"), 300.0f, 1.0f);
    }

    if (GestorNiveles)
    {
        GestorNiveles->CambiarEstado(GestorNiveles->ObtenerEstadoNivel1());
    }
}

void AFacade::GenerarNivel2()
{
    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ANivelDirector* Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    ANivel2Builder* Builder = Mundo->SpawnActor<ANivel2Builder>(ANivel2Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && Builder)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(Builder));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_2_Intermedio"), 360.0f, 1.5f);
    }

    if (GestorNiveles)
    {
        GestorNiveles->CambiarEstado(GestorNiveles->ObtenerEstadoNivel2());
    }
}

void AFacade::GenerarNivel3()
{
    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ANivelDirector* Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    ANivel3Builder* Builder = Mundo->SpawnActor<ANivel3Builder>(ANivel3Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && Builder)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(Builder));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_3_Tormenta"), 420.0f, 2.0f);
    }

    if (GestorNiveles)
    {
        GestorNiveles->CambiarEstado(GestorNiveles->ObtenerEstadoNivel3());
    }
}

void AFacade::GenerarNivel4()
{
    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ANivelDirector* Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    ANivel4Builder* Builder = Mundo->SpawnActor<ANivel4Builder>(ANivel4Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && Builder)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(Builder));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_4_Infierno"), 480.0f, 2.5f);
    }

    if (GestorNiveles)
    {
        GestorNiveles->CambiarEstado(GestorNiveles->ObtenerEstadoNivel4());
    }
}

void AFacade::GenerarNivel5()
{
    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ANivelDirector* Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    ANivel5Builder* Builder = Mundo->SpawnActor<ANivel5Builder>(ANivel5Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && Builder)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(Builder));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_5_Caos"), 540.0f, 3.0f);
    }

    if (GestorNiveles)
    {
        GestorNiveles->CambiarEstado(GestorNiveles->ObtenerEstadoNivel5());
    }
}

void AFacade::GenerarNivel6()
{
    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ANivelDirector* Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    ANivel6Builder* Builder = Mundo->SpawnActor<ANivel6Builder>(ANivel6Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && Builder)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(Builder));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_6_Supremo"), 600.0f, 4.0f);
    }

    if (GestorNiveles)
    {
        GestorNiveles->CambiarEstado(GestorNiveles->ObtenerEstadoNivel6());
    }
}


