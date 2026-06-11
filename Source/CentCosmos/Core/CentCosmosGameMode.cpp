#include "CentCosmosGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Facade.h"
#include "CentCosmosPawn.h"
#include "Engine/World.h"
#include "TimerManager.h"

ACentCosmosGameMode::ACentCosmosGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
    DefaultPawnClass = ACentCosmosPawn::StaticClass();
    FacadeMaestro = nullptr;
}

void ACentCosmosGameMode::BeginPlay()
{
    Super::BeginPlay();
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FacadeMaestro = Mundo->SpawnActor<AFacade>(AFacade::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (FacadeMaestro)
    {
        FString NombreMapa = Mundo->GetMapName();
        NombreMapa.RemoveFromStart(Mundo->StreamingLevelsPrefix);
        UE_LOG(LogTemp, Warning, TEXT("[CentCosmos] Mapa detectado: %s"), *NombreMapa);

        if (NombreMapa.Contains(TEXT("LV1"))) FacadeMaestro->GenerarNivel1();
        else if (NombreMapa.Contains(TEXT("LV2"))) FacadeMaestro->GenerarNivel2();
        else if (NombreMapa.Contains(TEXT("LV3"))) FacadeMaestro->GenerarNivel3();
        else if (NombreMapa.Contains(TEXT("LV4"))) FacadeMaestro->GenerarNivel4();
        else if (NombreMapa.Contains(TEXT("LV5"))) FacadeMaestro->GenerarNivel5();
        else if (NombreMapa.Contains(TEXT("LV6"))) FacadeMaestro->GenerarNivel6();
        else UE_LOG(LogTemp, Warning, TEXT("[CentCosmos] Menú o mapa desconocido."));
    }

    Mundo->GetTimerManager().SetTimer(TimerHandle_InputFix, this, &ACentCosmosGameMode::RestaurarInputJugador, 0.1f, false);
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
    default: UE_LOG(LogTemp, Warning, TEXT("[CentCosmos] Nivel desconocido: %d"), NumeroNivel); break;
    }
}

void ACentCosmosGameMode::OnJefeDerrotado()
{
    ACentCosmosPawn* Jugador = Cast<ACentCosmosPawn>(
        UGameplayStatics::GetPlayerPawn(this, 0));

    if (!Jugador) return;

    if (Jugador->VidaActual > 0.f)
        UGameplayStatics::OpenLevel(this, NombreNivelGanador);
    else
        UGameplayStatics::OpenLevel(this, NombreNivelGameOver);
}

void ACentCosmosGameMode::RestaurarInputJugador()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;
    APlayerController* PC = Mundo->GetFirstPlayerController();
    if (!PC) return;

    if (!PC->GetPawn())
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        ACentCosmosPawn* NuevoPawn = Mundo->SpawnActor<ACentCosmosPawn>(
            ACentCosmosPawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
        if (NuevoPawn) PC->Possess(NuevoPawn);
    }

    PC->SetInputMode(FInputModeGameOnly());
    PC->bShowMouseCursor = false;
    PC->SetIgnoreLookInput(false);
    PC->SetIgnoreMoveInput(false);
    UE_LOG(LogTemp, Warning, TEXT("[CentCosmos] Input restaurado."));
}

void ACentCosmosGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (!NewPlayer) return;
    NewPlayer->SetInputMode(FInputModeGameOnly());
    NewPlayer->bShowMouseCursor = false;
    NewPlayer->SetIgnoreLookInput(false);
    NewPlayer->SetIgnoreMoveInput(false);
}