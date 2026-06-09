#include "Nivel5Builder.h"
#include "EnemyFactory.h"
#include "GeneradorAmbientacion.h"
#include "Kismet/GameplayStatics.h"
#include "ArenaCombate.h"
#include "TribunalCaos.h"

ANivel5Builder::ANivel5Builder()
{
    PrimaryActorTick.bCanEverTick = true;
    bBossSpawneado = false;
    Factory = nullptr;
    Generador = nullptr;
}

void ANivel5Builder::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyFactory::StaticClass(), Found);
    if (Found.Num() > 0) Factory = Cast<AEnemyFactory>(Found[0]);

    Found.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeneradorAmbientacion::StaticClass(), Found);
    if (Found.Num() > 0) Generador = Cast<AGeneradorAmbientacion>(Found[0]);
}


void ANivel5Builder::Reset()
{
    EnemigosOla1.Empty();
    bBossSpawneado = false;
}

void ANivel5Builder::SetMetadatos(const FString& Nombre, float Duracion)
{
}

void ANivel5Builder::SetDificultad(float Nivel)
{
}

void ANivel5Builder::AgregarAmbientacion(UWorld* Mundo)
{
    if (!Generador) return;

    SpawnNaves(Generador, 8);
    SpawnSatelites(Generador, 6);
    SpawnRestos(Generador, 6);
    SpawnGrietasAntimateria(Generador, 6);
}

void ANivel5Builder::AgregarEnemigos(UWorld* Mundo)
{
    if (!Factory) return;

    SpawnVastagos(Factory, 18);
    SpawnVigias(Factory, 4);
    SpawnHeraldos(Factory, 3);
    SpawnEspectrosErrantes(Factory, 2);
}

FNivel ANivel5Builder::ObtenerNivel()
{
    return FNivel();
}

void ANivel5Builder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    VerificarYSpawnearBoss();
}

void ANivel5Builder::SpawnVastagos(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarVastago(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel5Builder::SpawnVigias(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarVigia(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel5Builder::SpawnHeraldos(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarHeraldo(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel5Builder::SpawnEspectrosErrantes(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarEspectroErrante(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

// ── Pasos atómicos — Ambientación ───────────────────────────────────────────

void ANivel5Builder::SpawnNaves(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarNave(Pos, FRotator::ZeroRotator);
    }
}

void ANivel5Builder::SpawnSatelites(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarSatelite(Pos, FRotator::ZeroRotator);
    }
}

void ANivel5Builder::SpawnRestos(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarRestos(Pos, FRotator::ZeroRotator);
    }
}

void ANivel5Builder::SpawnGrietasAntimateria(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarGrietaAntimateria(Pos, FRotator::ZeroRotator);
    }
}

// ── Boss ─────────────────────────────────────────────────────────────────────

void ANivel5Builder::VerificarYSpawnearBoss()
{
    if (bBossSpawneado) return;

    EnemigosOla1.RemoveAll([](const TWeakObjectPtr<AActor>& Ptr)
    {
        return !Ptr.IsValid();
    });

    if (EnemigosOla1.Num() > 0) return;

    bBossSpawneado = true;

    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    FVector PosBoss = FVector(0.f, 600.f, 0.f);
    Mundo->SpawnActor<ATribunalCaos>(ATribunalCaos::StaticClass(), PosBoss, FRotator::ZeroRotator);
}