#include "LV01_GameMode.h"
#include "Engine/World.h"

ALV01_GameMode::ALV01_GameMode()
    : Director(nullptr), Builder(nullptr)
{
}

void ALV01_GameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    Builder = Mundo->SpawnActor<ANivel1Builder>(ANivel1Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && Builder)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(Builder));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_1"), 300.0f, 1.0f);
    }
}