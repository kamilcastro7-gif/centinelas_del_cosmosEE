// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaz.h" 
#include "Ambientacion.generated.h"

UCLASS()
class CENTCOSMOS_API AAmbientacion : public AActor, public IAInterfaz
{
	GENERATED_BODY()

public:
	AAmbientacion();

protected:
	virtual void BeginPlay() override;

	// Dejamos el componente visible para que las hijas lo usen
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ambientacion|Componentes")
	UStaticMeshComponent* MallaObstaculo;

public:
	// El método de tu interfaz
	virtual void InicializarObstaculo() override;

private:
	void EvitarColisionConJugador();
};
