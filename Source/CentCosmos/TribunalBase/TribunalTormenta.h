// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribunalBase.h"
#include "TribunalTormenta.generated.h"

class UStaticMeshComponent;
class AProyectilPlasma;
class AProyectilRayo;

UCLASS()
class CENTCOSMOS_API ATribunalTormenta : public ATribunalBase
{
	GENERATED_BODY()

public:
	ATribunalTormenta();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override; // NUEVO: Limpieza final

public:
	virtual void Tick(float DeltaTime) override;
	virtual void ActivarJefe() override;

	// Proyectiles asignables en el editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion de Combate")
	TSubclassOf<AProyectilPlasma> ClasePlasma;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion de Combate")
	TSubclassOf<AProyectilRayo> ClaseRayo;

	// Tiempos de recarga editables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion de Combate")
	float CadenciaRayo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion de Combate")
	float CadenciaPlasma;

	// Configuración de movimiento
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float VelocidadDesplazamiento;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float RadioMaximoMovimiento;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float TiempoEsperaEnPunto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float RangoDeteccionManual;

	// ==========================================
	// SISTEMA DE INVOCACIÓN DE FASES
	// ==========================================
	UPROPERTY(EditAnywhere, Category = "Invocacion")
	TSubclassOf<AActor> ClaseBoya;

	UPROPERTY(EditAnywhere, Category = "Invocacion")
	TSubclassOf<AActor> ClaseHeraldo;

	UPROPERTY(EditAnywhere, Category = "Invocacion")
	TSubclassOf<AActor> ClaseVastago;

	void InvocacionEnCirculo(TSubclassOf<AActor> ClaseEnemigo, int32 Cantidad, float Radio);

private:
	void CalcularNuevoPuntoAleatorio();
	void DispararRayo();
	void SpawnPlasma();

	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	UStaticMeshComponent* MiMalla;

	FTimerHandle TimerEsperaMovimiento;

	FVector PosicionInicialAnclaje;
	FVector PuntoDestinoActual;
	bool bPuedeMoverse;

	float CronometroRayo;
	float CronometroPlasma;

	// Banderas para controlar las 3 fases
	bool bBoyas120Invocadas;
	bool bHeraldos80Invocados;
	bool bVastagos40Invocados;
};
