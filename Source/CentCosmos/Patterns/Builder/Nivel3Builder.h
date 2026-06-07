// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "EnemyFactory.h"
#include "GeneradorAmbientacion.h"
#include "Nivel3Builder.generated.h"

UCLASS()
class CENTCOSMOS_API ANivel3Builder : public AActor, public INivelBuilder
{
    GENERATED_BODY()

public:
    ANivel3Builder();

    virtual void Reset() override;
    virtual void SetMetadatos(const FString& Nombre, float TiempoLimite) override;
    virtual void SetDificultad(float Dificultad) override;
    virtual void AgregarAmbientacion(UWorld* World) override;
    virtual void AgregarEnemigos(UWorld* World) override;
    virtual FNivel ObtenerNivel() override;
    virtual void Tick(float DeltaTime) override;

private:
    FNivel NivelActual;
    TArray<TWeakObjectPtr<AActor>> EnemigosGenerados;
    TArray<TWeakObjectPtr<AActor>> EnemigosOla1;
    bool bBossSpawneado;

    // ── Pasos atómicos — Enemigos ─────────────────────────────────────────────
    void SpawnVastagos(AEnemyFactory* Factory, int32 Cantidad);
    void SpawnVigias(AEnemyFactory* Factory, int32 Cantidad);
    void SpawnHeraldos(AEnemyFactory* Factory, int32 Cantidad);
    void SpawnBoyaElectrostatica(AEnemyFactory* Factory, int32 Cantidad);

    // ── Pasos atómicos — Ambientación ────────────────────────────────────────
    void SpawnNaves(AGeneradorAmbientacion* Gen, int32 Cantidad);
    void SpawnSatelites(AGeneradorAmbientacion* Gen, int32 Cantidad);
    void SpawnRestos(AGeneradorAmbientacion* Gen, int32 Cantidad);
    void SpawnAsteroides(AGeneradorAmbientacion* Gen, int32 Cantidad);

    // ── Evento de fin de ola ─────────────────────────────────────────────────
    void VerificarYSpawnearBoss(UWorld* World);
};