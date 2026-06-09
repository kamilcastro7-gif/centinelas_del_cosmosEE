#include "CentCosmosGameMode.h"
#include "CentCosmos.h"
#include "CentCosmosPawn.h"
#include "Engine/World.h"

ACentCosmosGameMode::ACentCosmosGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    DefaultPawnClass = ACentCosmosPawn::StaticClass();
    Director = nullptr;
    ManejadorHorda = nullptr;
    FacadeMaestro = nullptr;
}

TScriptInterface<INivelBuilder> ACentCosmosGameMode::CrearBuilderParaMapa(UWorld* Mundo, float& OutDificultad)
{
    FString NombreMapa = Mundo->GetMapName();
    NombreMapa.RemoveFromStart(Mundo->StreamingLevelsPrefix);

    UE_LOG(LogTemp, Warning, TEXT("[CentCosmos] Mapa detectado: %s"), *NombreMapa);

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (NombreMapa.Contains(TEXT("LV1"))) { OutDificultad = 1.0f; return TScriptInterface<INivelBuilder>(Mundo->SpawnActor<ANivel1Builder>(ANivel1Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params)); }
    if (NombreMapa.Contains(TEXT("LV2"))) { OutDificultad = 2.0f; return TScriptInterface<INivelBuilder>(Mundo->SpawnActor<ANivel2Builder>(ANivel2Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params)); }
    if (NombreMapa.Contains(TEXT("LV3"))) { OutDificultad = 3.0f; return TScriptInterface<INivelBuilder>(Mundo->SpawnActor<ANivel3Builder>(ANivel3Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params)); }
    if (NombreMapa.Contains(TEXT("LV4"))) { OutDificultad = 4.0f; return TScriptInterface<INivelBuilder>(Mundo->SpawnActor<ANivel4Builder>(ANivel4Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params)); }
    if (NombreMapa.Contains(TEXT("LV5"))) { OutDificultad = 5.0f; return TScriptInterface<INivelBuilder>(Mundo->SpawnActor<ANivel5Builder>(ANivel5Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params)); }
    if (NombreMapa.Contains(TEXT("LV6"))) { OutDificultad = 6.0f; return TScriptInterface<INivelBuilder>(Mundo->SpawnActor<ANivel6Builder>(ANivel6Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params)); }

    UE_LOG(LogTemp, Warning, TEXT("[CentCosmos] Mapa no reconocido, usando Nivel1Builder por defecto"));
    OutDificultad = 1.0f;
    return TScriptInterface<INivelBuilder>(Mundo->SpawnActor<ANivel1Builder>(ANivel1Builder::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params));
}

void ACentCosmosGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    float Dificultad = 1.0f;
    BuilderActivo = CrearBuilderParaMapa(Mundo, Dificultad);

    Director = Mundo->SpawnActor<ANivelDirector>(
        ANivelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    ManejadorHorda = Mundo->SpawnActor<AFacade>(
        AFacade::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    FacadeMaestro = Mundo->SpawnActor<AFacade>(
        AFacade::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Director && BuilderActivo.GetObject())
    {
        Director->SetBuilder(BuilderActivo);
        Director->ConstruirNivel(Mundo, TEXT("Nivel"), 300.0f, Dificultad);
    }

    Mundo->GetTimerManager().SetTimer(
        TimerHandle_InputFix,
        this,
        &ACentCosmosGameMode::RestaurarInputJugador,
        0.1f,
        false
    );
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
        default:
            UE_LOG(LogTemp, Warning, TEXT("[CentCosmos] IniciarNivel: numero de nivel no reconocido: %d"), NumeroNivel);
            break;
    }
}

void ACentCosmosGameMode::RestaurarInputJugador()
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
            ACentCosmosPawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

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

void ACentCosmosGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (!NewPlayer) return;

    FInputModeGameOnly InputMode;
    NewPlayer->SetInputMode(InputMode);
    NewPlayer->bShowMouseCursor = false;
    NewPlayer->SetIgnoreLookInput(false);
    NewPlayer->SetIgnoreMoveInput(false);
}

void ACentCosmosGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
