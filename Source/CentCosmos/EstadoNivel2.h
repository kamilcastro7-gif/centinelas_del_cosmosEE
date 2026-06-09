// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EstadoNivel.h"
#include "Sound/SoundBase.h"
#include "EstadoNivel2.generated.h"

UCLASS()
class CENTCOSMOS_API AEstadoNivel2 : public AActor, public IEstadoNivel
{
	GENERATED_BODY()

public:
	AEstadoNivel2();
	virtual void IniciarNivel(class AGestorNiveles* Gestor) override;
	virtual void EjecutarTick(float DeltaTime) override;

protected:
	UPROPERTY()
	USoundBase* MusicaNivel;
};
