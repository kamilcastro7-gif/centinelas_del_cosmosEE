// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribunalBase.h" 
#include "Tribunal_Ira.generated.h"

class AProyectilMagma;

UCLASS()
class CENTCOSMOS_API ATribunal_Ira : public ATribunalBase
{
    GENERATED_BODY()

public:
    ATribunal_Ira();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Sobrescribimos el método de activación del padre
    virtual void ActivarJefe() override;

    void NotificarMuerteMagma();

    // Propiedad para asignar el proyectil de magma desde el Blueprint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion de Combate")
    TSubclassOf<AProyectilMagma> ClaseMagma;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float VelocidadDesplazamiento;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float RadioMaximoMovimiento;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float TiempoEsperaEnPunto;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float RangoDeteccionManual;

private:
    void CalcularNuevoPuntoAleatorio();
    FTimerHandle TimerEsperaMovimiento;
    FVector PosicionInicialAnclaje;
    FVector PuntoDestinoActual;
    bool bPuedeMoverse;

    int32 MagmaActivoEnPantalla;

    float CronometroPFuego;
    float CadenciaPFuego;

    void IntentarDispararMagma();
    void DispararPFuegoConstante();
};
