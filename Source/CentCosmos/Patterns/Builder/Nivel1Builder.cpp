// Fill out your copyright notice in the Description page of Project Settings.
#include "Nivel1Builder.h"
#include "Engine/World.h"
#include "GeneradorAmbientacion.h"
#include "TribunalVigilante.h"
#include "Engine/Engine.h"
#include "ArenaCombate.h"
#include "ObstaculoNave.h"
#include "ObstaculoSatelite.h"
#include "ObstaculoRestos.h"
#include "Kismet/GameplayStatics.h"
// Nota: EnemyFactory.h ya viene incluido transitivamente desde Nivel1Builder.h

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────

ANivel1Builder::ANivel1Builder()
{
    PrimaryActorTick.bCanEverTick = true;
    bBossSpawneado = false;
}

// ─────────────────────────────────────────────────────────────────────────────
// INivelBuilder — pasos de construcción principales
// Equivalen a: Reset / ProducePart* / GetProduct del canónico
// ─────────────────────────────────────────────────────────────────────────────

void ANivel1Builder::Reset()
{
    NivelActual = FNivel();
    EnemigosGenerados.Empty();
    EnemigosOla1.Empty();
    bBossSpawneado = false;
}

void ANivel1Builder::SetMetadatos(const FString& Nombre, float TiempoLimite)
{
    NivelActual.Nombre = Nombre;
    NivelActual.TiempoLimite = TiempoLimite;
}

void ANivel1Builder::SetDificultad(float Dificultad)
{
    NivelActual.Dificultad = Dificultad;
}

void ANivel1Builder::AgregarAmbientacion(UWorld* World)
{
    if (!World) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // AGeneradorAmbientacion implementa IAbstractFactory.
    // Su BeginPlay ya spawna 30 obstáculos automáticamente — no hacemos nada más.
    World->SpawnActor<AGeneradorAmbientacion>(
        AGeneradorAmbientacion::StaticClass(),
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        Params);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
            TEXT("[Nivel1] Ambientación generada."));
}

// AgregarEnemigos — equivale a BuildFullFeaturedProduct() del canónico.
// Solo coordina; el trabajo real está en los pasos atómicos privados.
void ANivel1Builder::AgregarEnemigos(UWorld* World)
{
    if (!World) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AEnemyFactory* Factory = World->SpawnActor<AEnemyFactory>(
        AEnemyFactory::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (!Factory) return;

    // Ola 1: 12 Vástagos + 2 Vigías + 1 Heraldo
    SpawnVastagos(Factory, 12);
    SpawnVigias(Factory, 2);
    SpawnHeraldos(Factory, 1);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange,
            FString::Printf(TEXT("[Nivel1] Ola 1 generada: %d enemigos."), EnemigosOla1.Num()));
}

FNivel ANivel1Builder::ObtenerNivel()
{
    NivelActual.Enemigos = EnemigosGenerados;
    return NivelActual;
}

// ─────────────────────────────────────────────────────────────────────────────
// Pasos atómicos privados — equivalen a ProducePartA/B/C del canónico.
// Cada uno tiene una única responsabilidad: spawnear UN tipo de enemigo.
// ─────────────────────────────────────────────────────────────────────────────

void ANivel1Builder::SpawnVastagos(AEnemyFactory* Factory, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-1000, 1000),
            FMath::RandRange(-1000, 1000),
            150.f);

        AActor* E = Factory->FabricarVastago(Pos, FRotator::ZeroRotator);
        if (E)
        {
            EnemigosGenerados.Add(E);
            EnemigosOla1.Add(E);
        }
    }
}

void ANivel1Builder::SpawnVigias(AEnemyFactory* Factory, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-1000, 1000),
            FMath::RandRange(-1000, 1000),
            150.f);

        AActor* E = Factory->FabricarVigia(Pos, FRotator::ZeroRotator);
        if (E)
        {
            EnemigosGenerados.Add(E);
            EnemigosOla1.Add(E);
        }
    }
}

void ANivel1Builder::SpawnHeraldos(AEnemyFactory* Factory, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(   
            FMath::RandRange(-800, 800),
            FMath::RandRange(-800, 800),
            150.f);

        AActor* E = Factory->FabricarHeraldo(Pos, FRotator::ZeroRotator);
        if (E)
        {
            EnemigosGenerados.Add(E);
            EnemigosOla1.Add(E);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Monitoreo de ola — lógica de juego post-construcción
// ─────────────────────────────────────────────────────────────────────────────

void ANivel1Builder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Salimos si el boss ya spawneó o si la ola aún no fue generada
    if (bBossSpawneado || EnemigosOla1.Num() == 0) return;

    UWorld* World = GetWorld();
    if (!World) return;

    VerificarYSpawnearBoss(World);
}

void ANivel1Builder::VerificarYSpawnearBoss(UWorld* World)
{
    // Contamos cuántos enemigos de la ola 1 siguen vivos
    int32 Vivos = 0;
    for (const TWeakObjectPtr<AActor>& Ref : EnemigosOla1)
    {
        if (Ref.IsValid()) Vivos++;
    }

    if (Vivos > 0) return;

    // Todos muertos — activamos el evento del jefe
    bBossSpawneado = true;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    TArray<AActor*> Encontrados;

    // 1. Eliminar ambientación
    UGameplayStatics::GetAllActorsOfClass(World, AObstaculoNave::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    UGameplayStatics::GetAllActorsOfClass(World, AObstaculoSatelite::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    UGameplayStatics::GetAllActorsOfClass(World, AObstaculoRestos::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    UGameplayStatics::GetAllActorsOfClass(World, AGeneradorAmbientacion::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            TEXT("[Nivel1] Ambientación eliminada."));

    // 2. Spawnear arena de combate (centro del mapa)
    World->SpawnActor<AArenaCombate>(
        AArenaCombate::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    // 3. Spawnear jefe (elevado para no quedar enterrado en el suelo)
    ATribunalVigilante* Boss = World->SpawnActor<ATribunalVigilante>(
        ATribunalVigilante::StaticClass(), FVector(0.f, 0.f,0.f), FRotator::ZeroRotator, Params);

    if (Boss)
    {
        EnemigosGenerados.Add(Boss);
        NivelActual.Enemigos.Add(Boss);

        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                TEXT("[Nivel1] OLA 1 COMPLETADA — ¡Arena y TribunalVigilante aparecen!"));
    }
}