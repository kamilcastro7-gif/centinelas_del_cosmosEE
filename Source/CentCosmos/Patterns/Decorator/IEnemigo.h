// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEnemigo.generated.h"

UINTERFACE(MinimalAPI)
class UEnemigo : public UInterface
{
	GENERATED_BODY()
};


class CENTCOSMOS_API IEnemigo
{
	GENERATED_BODY()

public:
	virtual float GetVida()               const = 0;
	virtual float GetVidaMaxima()         const = 0;
	virtual void  RecibirDanio(float Cantidad) = 0;
	virtual void  RegenerarVida(float Bonus) = 0;
	virtual bool  EstaVivo()              const = 0;
};