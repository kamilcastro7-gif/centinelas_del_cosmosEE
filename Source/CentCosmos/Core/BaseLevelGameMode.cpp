#include "BaseLevelGameMode.h"
#include "Engine/World.h"

ABaseLevelGameMode::ABaseLevelGameMode()
{
    DefaultPawnClass = ACentCosmosPawn::StaticClass();
}

void ABaseLevelGameMode::BeginPlay()
{
    Super::BeginPlay();
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    Mundo->GetTimerManager().SetTimer(
        TimerHandle_InputFix,
        this,
        &ABaseLevelGameMode::RestaurarInputJugador,
        0.1f,
        false
    );
}

void ABaseLevelGameMode::RestaurarInputJugador()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;
    APlayerController* PC = Mundo->GetFirstPlayerController();
    if (!PC) return;

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

    FInputModeGameOnly InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = false;
    PC->SetIgnoreLookInput(false);
    PC->SetIgnoreMoveInput(false);

    UE_LOG(LogTemp, Warning, TEXT("[CentCosmos] Input restaurado. Pawn: %s"),
        PC->GetPawn() ? *PC->GetPawn()->GetName() : TEXT("NINGUNO"));
}

void ABaseLevelGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (!NewPlayer) return;
    FInputModeGameOnly InputMode;
    NewPlayer->SetInputMode(InputMode);
    NewPlayer->bShowMouseCursor = false;
    NewPlayer->SetIgnoreLookInput(false);
    NewPlayer->SetIgnoreMoveInput(false);
}