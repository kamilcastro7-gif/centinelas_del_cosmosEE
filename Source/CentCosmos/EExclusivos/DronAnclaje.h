// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EExclusivo.h"
#include "DronAnclaje.generated.h"

UCLASS()
class CENTCOSMOS_API ADronAnclaje : public AEExclusivo
{
	GENERATED_BODY()

public:
	ADronAnclaje();

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle TimerEscudoHandle;
	void OtorgarEscudoAEnemigo();
};
