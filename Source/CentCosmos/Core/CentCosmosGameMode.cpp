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
    BuilderFacil = Mundo->SpawnActor<ANivel1Builder>(ANivel1Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    ManejadorHorda = Mundo->SpawnActor<AFacade>(AFacade::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && BuilderFacil)
    {
        Director->SetBuilder(TScriptInterface<INivelBuilder>(BuilderFacil));
        Director->ConstruirNivel(Mundo, TEXT("Nivel_Facil"), 300.0f, 1.0f);
    }

    // Esperar un frame para que el PC y el Pawn estén completamente inicializados
    Mundo->GetTimerManager().SetTimer(
        TimerHandle_InputFix,
        this,
        &ACentCosmosGameMode::RestaurarInputJugador,
        0.1f,
        false
    );
}

void ACentCosmosGameMode::RestaurarInputJugador()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    APlayerController* PC = Mundo->GetFirstPlayerController();
    if (!PC) return;

    // Si el pawn no fue poseído automáticamente, lo spawneamos y poseemos
    if (!PC->GetPawn())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ACentCosmosPawn* NuevoPawn = Mundo->SpawnActor<ACentCosmosPawn>(
            ACentCosmosPawn::StaticClass(),
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            SpawnParams
        );

        if (NuevoPawn)
            PC->Possess(NuevoPawn);
    }

    // Restaurar control al jugador
    FInputModeGameOnly InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = false;
    PC->SetIgnoreLookInput(false);
    PC->SetIgnoreMoveInput(false);

    UE_LOG(LogTemp, Warning, TEXT("[CentCosmos] Input restaurado. Pawn: %s"),
        PC->GetPawn() ? *PC->GetPawn()->GetName() : TEXT("NINGUNO"));
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

void ACentCosmosGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ManejadorHorda)
        ManejadorHorda->MoverHorda();
}