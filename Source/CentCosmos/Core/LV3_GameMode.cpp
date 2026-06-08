// LV03_GameMode.cpp
#include "LV3_GameMode.h"
#include "Engine/World.h"

ALV3_GameMode::ALV3_GameMode()
    : Director(nullptr), Builder(nullptr)
{
}

void ALV3_GameMode::BeginPlay()
{
    Super::BeginPlay();  // <- dispara el timer del input fix autom�ticamente

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    Builder = Mundo->SpawnActor<ANivel3Builder>(ANivel3Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && Builder)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(Builder));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_3"), 300.0f, 1.0f);
    }
}