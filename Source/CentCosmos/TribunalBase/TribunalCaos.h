// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribunalBase.h"
#include "TribunalCaos.generated.h"

class AHologramaCaos;

UCLASS()
class CENTCOSMOS_API ATribunalCaos : public ATribunalBase
{
    GENERATED_BODY()

public:
    ATribunalCaos();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void ActivarJefe() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion")
    float RangoDeteccionManual;

private:
    FTimerHandle TimerCicloVisibilidad;
    FTimerHandle TimerDisparoSecuencial;

    AHologramaCaos* Holograma1;
    AHologramaCaos* Holograma2;

    TArray<FVector> PosicionesFijas;

    int32 PasoDisparoActual;

    void IniciarFaseOculta();
    void IniciarFaseVisible();
    void EjecutarDisparoEnOrden();
    void ReubicarActoresAlAzar();

    void DesactivarJefe();
};
