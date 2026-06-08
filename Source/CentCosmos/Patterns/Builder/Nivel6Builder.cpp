#include "Nivel6Builder.h"
#include "EnemyFactory.h"
#include "GeneradorAmbientacion.h"
#include "Kismet/GameplayStatics.h"
#include "ArenaCombate.h"
#include "TribunalSupremo.h"

ANivel6Builder::ANivel6Builder()
{
    PrimaryActorTick.bCanEverTick = true;
    bBossSpawneado = false;
    Factory = nullptr;
    Generador = nullptr;
}

void ANivel6Builder::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyFactory::StaticClass(), Found);
    if (Found.Num() > 0) Factory = Cast<AEnemyFactory>(Found[0]);

    Found.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeneradorAmbientacion::StaticClass(), Found);
    if (Found.Num() > 0) Generador = Cast<AGeneradorAmbientacion>(Found[0]);
}

// ── INivelBuilder ────────────────────────────────────────────────────────────

void ANivel6Builder::Reset()
{
    EnemigosOla1.Empty();
    bBossSpawneado = false;
}

void ANivel6Builder::SetMetadatos(const FString& Nombre, float Duracion)
{
}

void ANivel6Builder::SetDificultad(float Nivel)
{
}

void ANivel6Builder::AgregarAmbientacion(UWorld* Mundo)
{
    if (!Generador) return;

    SpawnNaves(Generador, 8);
    SpawnSatelites(Generador, 8);
    SpawnRestos(Generador, 8);
    SpawnEclipsesSilenciosos(Generador, 6);
}

void ANivel6Builder::AgregarEnemigos(UWorld* Mundo)
{
    if (!Factory) return;

    SpawnVastagos(Factory, 20);
    SpawnVigias(Factory, 5);
    SpawnHeraldos(Factory, 3);
    SpawnEcoCorruptos(Factory, 2);
}

FNivel ANivel6Builder::ObtenerNivel()
{
    return FNivel();
}

void ANivel6Builder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    VerificarYSpawnearBoss();
}


void ANivel6Builder::SpawnVastagos(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarVastago(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel6Builder::SpawnVigias(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarVigia(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel6Builder::SpawnHeraldos(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarHeraldo(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel6Builder::SpawnEcoCorruptos(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarEcoCorrupto(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

// ── Pasos atómicos — Ambientación ───────────────────────────────────────────

void ANivel6Builder::SpawnNaves(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarNave(Pos, FRotator::ZeroRotator);
    }
}

void ANivel6Builder::SpawnSatelites(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarSatelite(Pos, FRotator::ZeroRotator);
    }
}

void ANivel6Builder::SpawnRestos(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarRestos(Pos, FRotator::ZeroRotator);
    }
}

void ANivel6Builder::SpawnEclipsesSilenciosos(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarEclipseSilencioso(Pos, FRotator::ZeroRotator);
    }
}

// ── Boss ─────────────────────────────────────────────────────────────────────

void ANivel6Builder::VerificarYSpawnearBoss()
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
    Mundo->SpawnActor<ATribunalSupremo>(ATribunalSupremo::StaticClass(), PosBoss, FRotator::ZeroRotator);
}