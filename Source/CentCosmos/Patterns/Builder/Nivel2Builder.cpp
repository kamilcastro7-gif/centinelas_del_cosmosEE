// Fill out your copyright notice in the Description page of Project Settings.
#include "Nivel2Builder.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GeneradorAmbientacion.h"
#include "TribAsedio.h"
#include "ArenaCombate.h"
#include "ObstaculoNave.h"
#include "ObstaculoSatelite.h"
#include "ObstaculoRestos.h"
#include "NubeIonizada.h"
#include "Kismet/GameplayStatics.h"

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────

ANivel2Builder::ANivel2Builder()
{
    PrimaryActorTick.bCanEverTick = true;
    bBossSpawneado = false;
}

// ─────────────────────────────────────────────────────────────────────────────
// INivelBuilder — pasos de construcción principales
// ─────────────────────────────────────────────────────────────────────────────

void ANivel2Builder::Reset()
{
    NivelActual = FNivel();
    EnemigosGenerados.Empty();
    EnemigosOla1.Empty();
    bBossSpawneado = false;
}

void ANivel2Builder::SetMetadatos(const FString& Nombre, float TiempoLimite)
{
    NivelActual.Nombre = Nombre;
    NivelActual.TiempoLimite = TiempoLimite;
}

void ANivel2Builder::SetDificultad(float Dificultad)
{
    NivelActual.Dificultad = Dificultad;
}

void ANivel2Builder::AgregarAmbientacion(UWorld* World)
{
    if (!World) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AGeneradorAmbientacion* Gen = World->SpawnActor<AGeneradorAmbientacion>(
        AGeneradorAmbientacion::StaticClass(),
        FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (!Gen) return;

    // Nivel 2 — originales + NubeIonizada
    SpawnNaves(Gen, 8);
    SpawnSatelites(Gen, 8);
    SpawnRestos(Gen, 8);
    SpawnNubeIonizada(Gen, 6);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
            TEXT("[Nivel2] Ambientación generada: 30 obstáculos."));
}

void ANivel2Builder::AgregarEnemigos(UWorld* World)
{
    if (!World) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AEnemyFactory* Factory = World->SpawnActor<AEnemyFactory>(
        AEnemyFactory::StaticClass(),
        FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (!Factory) return;

    // Ola 1: 14 Vástagos + 3 Vigías + 2 Heraldos + 2 MinadorRocoso
    SpawnVastagos(Factory, 14);
    SpawnVigias(Factory, 3);
    SpawnHeraldos(Factory, 2);
    SpawnMinadorRocoso(Factory, 2);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange,
            FString::Printf(TEXT("[Nivel2] Ola 1 generada: %d enemigos."), EnemigosOla1.Num()));
}

FNivel ANivel2Builder::ObtenerNivel()
{
    NivelActual.Enemigos = EnemigosGenerados;
    return NivelActual;
}

// ─────────────────────────────────────────────────────────────────────────────
// Pasos atómicos — Enemigos
// ─────────────────────────────────────────────────────────────────────────────

void ANivel2Builder::SpawnVastagos(AEnemyFactory* Factory, int32 Cantidad)
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

void ANivel2Builder::SpawnVigias(AEnemyFactory* Factory, int32 Cantidad)
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

void ANivel2Builder::SpawnHeraldos(AEnemyFactory* Factory, int32 Cantidad)
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

void ANivel2Builder::SpawnMinadorRocoso(AEnemyFactory* Factory, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-1000, 1000),
            FMath::RandRange(-1000, 1000),
            150.f);
        AActor* E = Factory->FabricarMinadorRocoso(Pos, FRotator::ZeroRotator);
        if (E)
        {
            EnemigosGenerados.Add(E);
            EnemigosOla1.Add(E);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Pasos atómicos — Ambientación
// ─────────────────────────────────────────────────────────────────────────────

void ANivel2Builder::SpawnNaves(AGeneradorAmbientacion* Gen, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-2000, 2000),
            FMath::RandRange(-2000, 2000),
            150.f);
        Gen->FabricarNave(Pos, FRotator::ZeroRotator);
    }
}

void ANivel2Builder::SpawnSatelites(AGeneradorAmbientacion* Gen, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-2000, 2000),
            FMath::RandRange(-2000, 2000),
            150.f);
        Gen->FabricarSatelite(Pos, FRotator::ZeroRotator);
    }
}

void ANivel2Builder::SpawnRestos(AGeneradorAmbientacion* Gen, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-2000, 2000),
            FMath::RandRange(-2000, 2000),
            150.f);
        Gen->FabricarRestos(Pos, FRotator::ZeroRotator);
    }
}

void ANivel2Builder::SpawnNubeIonizada(AGeneradorAmbientacion* Gen, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-2000, 2000),
            FMath::RandRange(-2000, 2000),
            150.f);
        Gen->FabricarNubeIonizada(Pos, FRotator::ZeroRotator);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Monitoreo de ola
// ─────────────────────────────────────────────────────────────────────────────

void ANivel2Builder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bBossSpawneado || EnemigosOla1.Num() == 0) return;

    UWorld* World = GetWorld();
    if (!World) return;

    VerificarYSpawnearBoss(World);
}

void ANivel2Builder::VerificarYSpawnearBoss(UWorld* World)
{
    int32 Vivos = 0;
    for (const TWeakObjectPtr<AActor>& Ref : EnemigosOla1)
    {
        if (Ref.IsValid()) Vivos++;
    }

    if (Vivos > 0) return;

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

    UGameplayStatics::GetAllActorsOfClass(World, ANubeIonizada::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    UGameplayStatics::GetAllActorsOfClass(World, AGeneradorAmbientacion::StaticClass(), Encontrados);
    for (AActor* A : Encontrados) A->Destroy();

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            TEXT("[Nivel2] Ambientación eliminada."));

    // 2. Arena de combate
    World->SpawnActor<AArenaCombate>(
        AArenaCombate::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    // 3. Boss — TribAsedio
    ATribAsedio* Boss = World->SpawnActor<ATribAsedio>(
        ATribAsedio::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (Boss)
    {
        EnemigosGenerados.Add(Boss);
        NivelActual.Enemigos.Add(Boss);

        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                TEXT("[Nivel2] OLA 1 COMPLETADA — ¡Arena y TribAsedio aparecen!"));
    }
}