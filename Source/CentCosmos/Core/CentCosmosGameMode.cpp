#include "CentCosmosGameMode.h"
#include "CentCosmosPawn.h"
#include "Engine/World.h"

ACentCosmosGameMode::ACentCosmosGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
    DefaultPawnClass = ACentCosmosPawn::StaticClass();
    FacadeMaestro = nullptr;
}

void ACentCosmosGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* const Mundo = GetWorld();
    if (Mundo)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        // 1. Spawneamos el Facade Maestro
        FacadeMaestro = Mundo->SpawnActor<AFacade>(AFacade::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
        // 2. ¡OJO AQUÍ! Forzamos la generación del Nivel 1 para tu prueba rápida
        if (FacadeMaestro)
        {
            IniciarNivel(4);
        }
    }
}

void ACentCosmosGameMode::IniciarNivel(int32 NumeroNivel)
{
    if (!FacadeMaestro) return;

    switch (NumeroNivel)
    {
    case 1: FacadeMaestro->GenerarNivel1(); break;
    case 2: FacadeMaestro->GenerarNivel2(); break;
    case 3: FacadeMaestro->GenerarNivel3(); break;
    case 4: FacadeMaestro->GenerarNivel4(); break;
    case 5: FacadeMaestro->GenerarNivel5(); break;
    case 6: FacadeMaestro->GenerarNivel6(); break;
    default: FacadeMaestro->GenerarNivel1(); break;
    }
}

void ACentCosmosGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (NewPlayer)
    {
        FInputModeGameOnly InputMode;
        NewPlayer->SetInputMode(InputMode);
        NewPlayer->bShowMouseCursor = false;
        NewPlayer->SetIgnoreLookInput(false);
        NewPlayer->SetIgnoreMoveInput(false);
    }
}