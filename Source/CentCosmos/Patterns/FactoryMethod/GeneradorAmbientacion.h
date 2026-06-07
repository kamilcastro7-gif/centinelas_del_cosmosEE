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

protected:
    virtual void BeginPlay() override;

public:
    virtual AActor* FabricarObstaculo(const FVector& Posicion, const FRotator& Rotacion) override;
    virtual AActor* FabricarEnemigo(const FVector& Posicion, const FRotator& Rotacion) override;

private:
    // --- Obstáculos originales ---
    TSubclassOf<class AObstaculoNave>        ClaseNave;
    TSubclassOf<class AObstaculoSatelite>    ClaseSatelite;
    TSubclassOf<class AObstaculoRestos>      ClaseRestos;

    // --- Obstáculos nuevos ---
    TSubclassOf<class AAsteroide>            ClaseAsteroide;
    TSubclassOf<class ANubeIonizada>         ClaseNubeIonizada;
    TSubclassOf<class AFragmentoErupcion>    ClaseFragmentoErupcion;
    TSubclassOf<class AGrietaAntimateria>    ClaseGrietaAntimateria;
    TSubclassOf<class APAntimateria>         ClasePAntimateria;
    TSubclassOf<class ARastrosFuego>         ClaseRastrosFuego;
};