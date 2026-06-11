// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enjambre.h"
#include "EVastago_Del_infierno.generated.h"

class AProyectilBase;
class USkeletalMeshComponent; // Para la abeja animada
class UAnimSequence;          // Para las animaciones

UCLASS()
class CENTCOSMOS_API AEVastago_Del_infierno : public AEnjambre
{
    GENERATED_BODY()
public:
    AEVastago_Del_infierno();

    virtual void Tick(float DeltaTime) override;
    void moverVastago();

    void CargarAtaque();
    void EjecutarDisparo();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Combate")
    float VelocidadVastago;

public:
    // --- COMPONENTES DE LA AVISPA ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    USkeletalMeshComponent* MallaAvispa;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animaciones")
    UAnimSequence* AnimAdelante;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animaciones")
    UAnimSequence* AnimAtras;

private:
    UPROPERTY(VisibleAnywhere, Category = "Componentes")
    class UStaticMeshComponent* ProyectilFalso;

    float TimerEstado;
    bool bEstaRodeando;
    FVector DireccionDispersion;

    bool bOlaCargaIniciada;

    // Control para no reiniciar la animación en cada frame
    int32 EstadoAnimacionActual;

    void IntentarLiderarCarga();
};
