// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IAbstractFactory.h"
#include "GeneradorAmbientacion.generated.h"

UCLASS()
class CENTCOSMOS_API AGeneradorAmbientacion : public AActor, public IAbstractFactory
{
    GENERATED_BODY()

public:
    AGeneradorAmbientacion();

    // IAbstractFactory — cumplimiento de interfaz
    virtual AActor* FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion) override;
    virtual AActor* FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion) override;

    // ── Métodos individuales (igual que EnemyFactory) ─────────────────────────
    // Obstáculos originales
    AActor* FabricarNave(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarSatelite(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarRestos(const FVector& Posicion, const FRotator& Rotacion);
    // Obstáculos nuevos
    AActor* FabricarAsteroide(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarNubeIonizada(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarFragmentoErupcion(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarGrietaAntimateria(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarPAntimateria(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarRastrosFuego(const FVector& Posicion, const FRotator& Rotacion);
    AActor* FabricarEclipseSilencioso(const FVector& Posicion, const FRotator& Rotacion);

private:
    // Obstáculos originales
    TSubclassOf<class AObstaculoNave>         ClaseNave;
    TSubclassOf<class AObstaculoSatelite>     ClaseSatelite;
    TSubclassOf<class AObstaculoRestos>       ClaseRestos;
    // Obstáculos nuevos
    TSubclassOf<class AAsteroide>             ClaseAsteroide;
    TSubclassOf<class ANubeIonizada>          ClaseNubeIonizada;
    TSubclassOf<class AFragmentoErupcion>     ClaseFragmentoErupcion;
    TSubclassOf<class AGrietaAntimateria>     ClaseGrietaAntimateria;
    TSubclassOf<class APAntimateria>          ClasePAntimateria;
    TSubclassOf<class ARastrosFuego>          ClaseRastrosFuego;
    TSubclassOf<class AEclipseSilencioso>     ClaseEclipseSilencioso;
};