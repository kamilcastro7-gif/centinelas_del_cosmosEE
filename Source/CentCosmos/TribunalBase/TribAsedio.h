// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribunalBase.h"
#include "PlacaMetal.h"
#include "TribAsedio.generated.h"

UCLASS()
class CENTCOSMOS_API ATribAsedio : public ATribunalBase
{
    GENERATED_BODY()

public:
    ATribAsedio();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion Shield")
    int32 TotalPlacas;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion Shield")
    float RadioOrbita;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion Shield")
    float VelocidadRotacion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion")
    float RangoDeteccion;

    UPROPERTY(BlueprintReadOnly, Category = "Estado")
    bool bEsVulnerable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float VelocidadDesplazamiento;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float RadioMaximoMovimiento;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float TiempoEsperaEnPunto;

    void EliminarPlacaDeArreglo(APlacaMetal* PlacaMuerta);

private:
    void SpawnEscudoPlacas();
    void RegenerarEscudo();
    void DesactivarYLimpiarEscudo();

    void CalcularNuevoPuntoAleatorio();
    void PermitirSiguienteMovimiento();

    UPROPERTY()
    TArray<APlacaMetal*> ArregloPlacas;

    FTimerHandle TimerVulnerabilidad;
    FTimerHandle TimerEsperaMovimiento;

    float AnguloActualPlacas;
    UStaticMeshComponent* MallaJefe;

    FVector PosicionInicialAnclaje;
    FVector PuntoDestinoActual;
    bool bPuedeMoverse;
};
