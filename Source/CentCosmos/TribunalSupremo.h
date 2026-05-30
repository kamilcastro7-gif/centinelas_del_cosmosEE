// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribunalBase.h"
#include "TribunalSupremo.generated.h"

UCLASS()
class CENTCOSMOS_API ATribunalSupremo : public ATribunalBase
{
    GENERATED_BODY()
public:
    ATribunalSupremo();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void ActivarJefe() override;

private:
    FVector PosicionInicial;
    float DireccionMovimiento;

    UPROPERTY(EditAnywhere, Category = "Supremo | Movimiento")
    float VelocidadPatrulla;

    UPROPERTY(EditAnywhere, Category = "Supremo | Movimiento")
    float AmplitudMovimiento;

    UPROPERTY(EditAnywhere, Category = "Supremo | Configuracion")
    float RangoDeteccionManual;

    UPROPERTY(BlueprintReadOnly, Category = "Supremo | Estado", meta = (AllowPrivateAccess = "true"))
    bool bEstaDisparando;

    FTimerHandle TimerCicloAtaque;
    FTimerHandle TimerDisparoPunta;
    FTimerHandle TimerDisparoSupremoContinua;
    FTimerHandle TimerDescongelarRayo;

    int32 PuntaActualEstrella;
    int32 RafagasDisparadasSupremo;
    int32 DireccionActualSupremo;

    FRotator RotacionHaciaJugadorBase;

    void ControlarFases();
    void DispararPuntaSecuencial();
    void DispararPSupremoContinuo();
    void TerminarAtaqueRayo();
    void DesactivarJefe();
};
