// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EstadoNivel.h"
#include "Sound/SoundBase.h"
#include "EstadoNivel3.generated.h"

UCLASS()
class CENTCOSMOS_API AEstadoNivel3 : public AActor, public IEstadoNivel
{
	GENERATED_BODY()

public:
	AEstadoNivel3();
	virtual void IniciarNivel(class AGestorNiveles* Gestor) override;
	virtual void EjecutarTick(float DeltaTime) override;

protected:
	UPROPERTY()
	USoundBase* MusicaNivel;
};
