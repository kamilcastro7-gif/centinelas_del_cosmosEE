// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Patterns/Decorator/EnemDecorador.h"
#include "EnemDisparoTriple.generated.h"

UCLASS()
class CENTCOSMOS_API UEnemDisparoTriple : public UEnemDecorador
{
	GENERATED_BODY()

public:
	// SOLO sobrescribe la función de la interfaz.
	virtual bool GetDisparoTriple() const override;
};