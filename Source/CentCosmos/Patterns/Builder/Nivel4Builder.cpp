#include "Nivel4Builder.h"
#include "EnemyFactory.h"
#include "GeneradorAmbientacion.h"
#include "Kismet/GameplayStatics.h"
#include "ArenaCombate.h"
#include "Tribunal_Ira.h"

ANivel4Builder::ANivel4Builder()
{
    PrimaryActorTick.bCanEverTick = true;
    bBossSpawneado = false;
    Factory = nullptr;
    Generador = nullptr;
}

void ANivel4Builder::BeginPlay()
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

void ANivel4Builder::Reset()
{
    EnemigosOla1.Empty();
    bBossSpawneado = false;
}

void ANivel4Builder::SetMetadatos(const FString& Nombre, float Duracion)
{
}

void ANivel4Builder::SetDificultad(float Nivel)
{
}

void ANivel4Builder::AgregarAmbientacion(UWorld* Mundo)
{
    if (!Generador) return;

    SpawnNaves(Generador, 8);
    SpawnSatelites(Generador, 8);
    SpawnRestos(Generador, 8);
    SpawnRastrosFuego(Generador, 6);
}

void ANivel4Builder::AgregarEnemigos(UWorld* Mundo)
{
    if (!Factory) return;

    SpawnVastagos(Factory, 16);
    SpawnVigias(Factory, 4);
    SpawnHeraldos(Factory, 3);
    SpawnBombarderosFuego(Factory, 2);
}

FNivel ANivel4Builder::ObtenerNivel()
{
    return FNivel(); 
}

void ANivel4Builder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    VerificarYSpawnearBoss();
}

void ANivel4Builder::SpawnVastagos(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector  Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarVastago(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel4Builder::SpawnVigias(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector  Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarVigia(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel4Builder::SpawnHeraldos(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector  Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarHeraldo(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel4Builder::SpawnBombarderosFuego(AEnemyFactory* F, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector  Pos = FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 0.f);
        AActor* E = F->FabricarBombarderoFuego(Pos, FRotator::ZeroRotator);
        if (E) EnemigosOla1.Add(E);
    }
}

void ANivel4Builder::SpawnNaves(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarNave(Pos, FRotator::ZeroRotator);
    }
}

void ANivel4Builder::SpawnSatelites(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarSatelite(Pos, FRotator::ZeroRotator);
    }
}

void ANivel4Builder::SpawnRestos(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarRestos(Pos, FRotator::ZeroRotator);
    }
}

void ANivel4Builder::SpawnRastrosFuego(AGeneradorAmbientacion* G, int32 N)
{
    for (int32 i = 0; i < N; i++)
    {
        FVector Pos = FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-2000.f, 2000.f), 0.f);
        G->FabricarRastrosFuego(Pos, FRotator::ZeroRotator);
    }
}

void ANivel4Builder::VerificarYSpawnearBoss()
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
    Mundo->SpawnActor<ATribunal_Ira>(ATribunal_Ira::StaticClass(), PosBoss, FRotator::ZeroRotator);
}