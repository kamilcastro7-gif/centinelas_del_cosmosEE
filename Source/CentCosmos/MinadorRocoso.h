// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EExclusivo.h"
#include "MinadorRocoso.generated.h"

// Estados del enemigo para evitar que se mezclen las lógicas de movimiento
UENUM(BlueprintType)
enum class EMinadorEstado : uint8
{
	Posicionandose,
	EsperandoTimer,
	Embestiendo,
	Muerto
};

UCLASS()
class CENTCOSMOS_API AMinadorRocoso : public AEExclusivo
{
	GENERATED_BODY()

public:
	AMinadorRocoso();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle TimerCargaHandle;

	EMinadorEstado EstadoActual;

	FVector PosicionObjetivoEmbestida;

	UPROPERTY(EditAnywhere, Category = "Configuracion Minador")
	float DistanciaLargaFrenado;

	UPROPERTY(EditAnywhere, Category = "Configuracion Minador")
	float VelocidadEmbestida;

	void IniciarEmbestida();
	void EstallarEnCruz();
};
