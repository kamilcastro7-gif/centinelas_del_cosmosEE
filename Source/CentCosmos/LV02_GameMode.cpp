// LV01_GameMode.cpp
#include "LV02_GameMode.h"
#include "Engine/World.h"

ALV02_GameMode::ALV02_GameMode()
    : Director(nullptr), Builder(nullptr)
{
}

void ALV02_GameMode::BeginPlay()
{
    Super::BeginPlay();  // <- dispara el timer del input fix automáticamente

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    Builder = Mundo->SpawnActor<ANivel2Builder>(ANivel2Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && Builder)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(Builder));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_2"), 300.0f, 1.0f);
    }
}