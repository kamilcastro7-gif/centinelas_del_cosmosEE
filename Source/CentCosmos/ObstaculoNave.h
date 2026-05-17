// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ambientacion.h"
#include "ObstaculoNave.generated.h"

UCLASS()
class CENTCOSMOS_API AObstaculoNave : public AAmbientacion
{
	GENERATED_BODY()

public:
	AObstaculoNave();
	virtual void InicializarObstaculo() override;
};
