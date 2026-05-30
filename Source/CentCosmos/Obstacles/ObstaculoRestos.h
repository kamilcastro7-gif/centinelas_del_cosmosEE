// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ambientacion.h"
#include "ObstaculoRestos.generated.h"

UCLASS()
class CENTCOSMOS_API AObstaculoRestos : public AAmbientacion
{
	GENERATED_BODY()

public:
	AObstaculoRestos();
	virtual void InicializarObstaculo() override;
};
