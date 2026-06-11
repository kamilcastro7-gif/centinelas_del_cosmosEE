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


ANivel1Builder::ANivel1Builder()
{
    PrimaryActorTick.bCanEverTick = true;
    bBossSpawneado = false;
}

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

    AGeneradorAmbientacion* Gen = World->SpawnActor<AGeneradorAmbientacion>(
        AGeneradorAmbientacion::StaticClass(),
        FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (!Gen) return;
    SpawnNaves(Gen, 10);
    SpawnSatelites(Gen, 10);
    SpawnRestos(Gen, 10);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
            TEXT("[Nivel1] Ambientación generada: 30 obstáculos."));
}
void ANivel1Builder::AgregarEnemigos(UWorld* World)
{
    if (!World) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AEnemyFactory* Factory = World->SpawnActor<AEnemyFactory>(
        AEnemyFactory::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

    if (!Factory) return;
    SpawnVastagos(Factory, 12);
    SpawnVigias(Factory, 2);
    SpawnHeraldos(Factory, 1);
    SpawnDronAnclaje(Factory, 1);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange,
            FString::Printf(TEXT("[Nivel1] Ola 1 generada: %d enemigos."), EnemigosOla1.Num()));
}

FNivel ANivel1Builder::ObtenerNivel()
{
    NivelActual.Enemigos = EnemigosGenerados;
    return NivelActual;
}


void ANivel1Builder::SpawnVastagos(AEnemyFactory* Factory, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-1000, 1000),
            FMath::RandRange(-1000, 1000),
            0.f);

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
            0.f);

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
            0.f);

        AActor* E = Factory->FabricarHeraldo(Pos, FRotator::ZeroRotator);
        if (E)
        {
            EnemigosGenerados.Add(E);
            EnemigosOla1.Add(E);
        }
    }
}
void ANivel1Builder::SpawnDronAnclaje(AEnemyFactory* Factory, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-1000, 1000),
            FMath::RandRange(-1000, 1000),
            0.f);
        AActor* E = Factory->FabricarDronAnclaje(Pos, FRotator::ZeroRotator);
        if (E)
        {
            EnemigosGenerados.Add(E);
            EnemigosOla1.Add(E);
        }
    }
}

void ANivel1Builder::SpawnNaves(AGeneradorAmbientacion* Gen, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-2000, 2000),
            FMath::RandRange(-2000, 2000),
            0.f);
        Gen->FabricarNave(Pos, FRotator::ZeroRotator);
    }
}

void ANivel1Builder::SpawnSatelites(AGeneradorAmbientacion* Gen, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-2000, 2000),
            FMath::RandRange(-2000, 2000),
            0.f);
        Gen->FabricarSatelite(Pos, FRotator::ZeroRotator);
    }
}

void ANivel1Builder::SpawnRestos(AGeneradorAmbientacion* Gen, int32 Cantidad)
{
    for (int32 i = 0; i < Cantidad; i++)
    {
        FVector Pos = FVector(
            FMath::RandRange(-2000, 2000),
            FMath::RandRange(-2000, 2000),
            0.f);
        Gen->FabricarRestos(Pos, FRotator::ZeroRotator);
    }
}

void ANivel1Builder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bBossSpawneado || EnemigosOla1.Num() == 0) return;

    UWorld* World = GetWorld();
    if (!World) return;

    VerificarYSpawnearBoss(World);
}

void ANivel1Builder::VerificarYSpawnearBoss(UWorld* World)
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
    World->SpawnActor<AArenaCombate>(
        AArenaCombate::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
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