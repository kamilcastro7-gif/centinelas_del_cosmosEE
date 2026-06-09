// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Facade.generated.h"

class AGestorNiveles; // Forward declaration para mantener limpio el .h

UCLASS()
class CENTCOSMOS_API AFacade : public AActor
{
    GENERATED_BODY()

public:
    AFacade();

protected:
    virtual void BeginPlay() override; // Agregamos BeginPlay

public:
    // =======================================================
    // INTERFAZ UNIFICADA PARA GENERAR NIVELES (Patrón Facade)
    // =======================================================
    void GenerarNivel1();
    void GenerarNivel2();
    void GenerarNivel3();
    void GenerarNivel4();
    void GenerarNivel5();
    void GenerarNivel6();

private:
    // El Facade ahora es dueño absoluto del sistema de música (State)
    UPROPERTY()
    AGestorNiveles* GestorNiveles;
};
