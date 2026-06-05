// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EExclusivo.h"
#include "BombarderoFuego.generated.h"

UCLASS()
class CENTCOSMOS_API ABombarderoFuego : public AEExclusivo
{
	GENERATED_BODY()

public:
	ABombarderoFuego();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	float VelocidadMovimiento;
	FVector PosicionInicial; // Guardaremos su X y Z originales

	FTimerHandle TimerAtaqueHandle;
	void EjecutarDisparoFuego();
};
