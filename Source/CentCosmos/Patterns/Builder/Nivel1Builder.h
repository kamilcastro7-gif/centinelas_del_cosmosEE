// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INivelBuilder.h"
#include "EnemyFactory.h"          // include completo — reemplaza el forward declaration
#include "Nivel1Builder.generated.h"

UCLASS()
class CENTCOSMOS_API ANivel1Builder : public AActor, public INivelBuilder
{
    GENERATED_BODY()
public:
    ANivel1Builder();

    virtual void Reset() override;
    virtual void SetMetadatos(const FString& Nombre, float TiempoLimite) override;
    virtual void SetDificultad(float Dificultad) override;
    virtual void AgregarAmbientacion(UWorld* World) override;
    // Coordina los pasos atómicos — equivale a BuildFullFeaturedProduct()
    virtual void AgregarEnemigos(UWorld* World) override;
    virtual FNivel ObtenerNivel() override;
    // Tick para monitorear si todos los enemigos de ola 1 murieron
    virtual void Tick(float DeltaTime) override;

private:
    FNivel NivelActual;
    TArray<TWeakObjectPtr<AActor>> EnemigosGenerados;
    TArray<TWeakObjectPtr<AActor>> EnemigosOla1;
    // Flag para no spawnear el boss más de una vez
    bool bBossSpawneado;

    // ── Pasos atómicos de construcción (como ProducePart* del canónico) ───────
    void SpawnVastagos(AEnemyFactory* Factory, int32 Cantidad);
    void SpawnVigias(AEnemyFactory* Factory, int32 Cantidad);
    void SpawnHeraldos(AEnemyFactory* Factory, int32 Cantidad);

    // ── Evento de fin de ola ─────────────────────────────────────────────────
    void VerificarYSpawnearBoss(UWorld* World);
};