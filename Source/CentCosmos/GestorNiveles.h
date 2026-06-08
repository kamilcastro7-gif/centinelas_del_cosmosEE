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

	// El reproductor de audio centralizado
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Central")
	UAudioComponent* AudioComponentCentral;

	// Getter para que el GameMode (Cliente) acceda a este estado cuando lo necesite
	IEstadoNivel* ObtenerEstadoNivel1() { return EstadoNivel1; }

private:
	IEstadoNivel* EstadoActual;

	// Instancia en memoria para el Nivel 1 (luego agregarás los demás aquí)
	IEstadoNivel* EstadoNivel1;
};
