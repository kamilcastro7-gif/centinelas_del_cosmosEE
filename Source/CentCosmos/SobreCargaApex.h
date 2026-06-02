// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "PowerUp.h"
#include "SobreCargaApex.generated.h"

UCLASS()
class CENTCOSMOS_API APowerUpSobrecargaApex : public APowerUp
{
	GENERATED_BODY()
public:
	APowerUpSobrecargaApex();
protected:
	virtual void AplicarEfecto(class ACentCosmosPawn* Nave) override;
};