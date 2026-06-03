#include "CentCosmosGameMode.h"
#include "CentCosmosPawn.h"
#include "CentCosmos.h"
#include "Engine/World.h"

ACentCosmosGameMode::ACentCosmosGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    DefaultPawnClass = ACentCosmosPawn::StaticClass();
    Director = nullptr;
    BuilderFacil = nullptr;
    ManejadorHorda = nullptr;
}

void ACentCosmosGameMode::BeginPlay()
{
    Super::BeginPlay();
    UWorld* const Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    Director = Mundo->SpawnActor<ANivelDirector>(ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    BuilderFacil = Mundo->SpawnActor<ANivelFacilBuilder>(ANivelFacilBuilder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    ManejadorHorda = Mundo->SpawnActor<AFacade>(AFacade::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && BuilderFacil)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(BuilderFacil));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_Facil"), 300.0f, 1.0f);
    }
}

void ACentCosmosGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (ManejadorHorda)
        ManejadorHorda->MoverHorda();
}