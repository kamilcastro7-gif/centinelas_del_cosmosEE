// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Patterns/Decorator/EnemDecorador.h"
#include "EnemSobrecargaApex.generated.h"

UCLASS()
class CENTCOSMOS_API UEnemSobrecargaApex : public UEnemDecorador
{
	GENERATED_BODY()

public:
	virtual float GetVelocidad(float BaseSpeed) const override;
	virtual float GetCadencia(float BaseRate)   const override;
};