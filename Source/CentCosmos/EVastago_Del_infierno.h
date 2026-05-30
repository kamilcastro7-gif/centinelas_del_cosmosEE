// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enjambre.h"
#include "EVastago_Del_infierno.generated.h"

class AProyectilBase;

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

private:
    UPROPERTY(VisibleAnywhere, Category = "Componentes")
    class UStaticMeshComponent* ProyectilFalso;

    float TimerEstado;
    bool bEstaRodeando;
    FVector DireccionDispersion;

    // Bandera para no repetir la orden de carga
    bool bOlaCargaIniciada;

    void IntentarLiderarCarga();
};
