// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EstadoNivel.h"
#include "Sound/SoundBase.h"
#include "EstadoNivel1.generated.h"

UCLASS()
class CENTCOSMOS_API AEstadoNivel1 : public AActor, public IEstadoNivel
{
	GENERATED_BODY()

public:
	AEstadoNivel1();
	virtual void IniciarNivel(class AGestorNiveles* Gestor) override;
	virtual void EjecutarTick(float DeltaTime) override;

protected:
	UPROPERTY()
	USoundBase* MusicaNivel;
};
