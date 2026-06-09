// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EstadoNivel.h"
#include "Components/AudioComponent.h"
#include "GestorNiveles.generated.h"

UCLASS()
class CENTCOSMOS_API AGestorNiveles : public AActor
{
	GENERATED_BODY()

public:
	AGestorNiveles();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void CambiarEstado(IEstadoNivel* NuevoEstado);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Central")
	UAudioComponent* AudioComponentCentral;

	// Getters para que el GameMode u otros sistemas accedan a los estados
	IEstadoNivel* ObtenerEstadoNivel1() { return EstadoNivel1; }
	IEstadoNivel* ObtenerEstadoNivel2() { return EstadoNivel2; }
	IEstadoNivel* ObtenerEstadoNivel3() { return EstadoNivel3; }
	IEstadoNivel* ObtenerEstadoNivel4() { return EstadoNivel4; }
	IEstadoNivel* ObtenerEstadoNivel5() { return EstadoNivel5; }
	IEstadoNivel* ObtenerEstadoNivel6() { return EstadoNivel6; }

private:
	IEstadoNivel* EstadoActual;

	// Instancias en memoria para todos los niveles
	IEstadoNivel* EstadoNivel1;
	IEstadoNivel* EstadoNivel2;
	IEstadoNivel* EstadoNivel3;
	IEstadoNivel* EstadoNivel4;
	IEstadoNivel* EstadoNivel5;
	IEstadoNivel* EstadoNivel6;
};
