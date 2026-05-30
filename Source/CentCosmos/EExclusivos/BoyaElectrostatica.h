// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EExclusivo.h"
#include "BoyaElectrostatica.generated.h"

UCLASS()
class CENTCOSMOS_API ABoyaElectrostatica : public AEExclusivo
{
	GENERATED_BODY()

public:
	ABoyaElectrostatica();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle TimerElevacionHandle;

	FVector PosicionBaseZ;
	FVector DestinoErratico;

	float TiempoParaNuevoDestino;
	float RangoDistanciaJugador;

	bool bEstaElevandose;
	float TiempoElevacion;

	void IniciarFaseElevacion();
	void SpawnChispasAlRededor();
};