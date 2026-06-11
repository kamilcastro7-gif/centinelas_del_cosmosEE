// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EExclusivo.generated.h"

UCLASS()
class CENTCOSMOS_API AEExclusivo : public AActor
{
	GENERATED_BODY()

public:
	AEExclusivo();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	UStaticMeshComponent* EnemigoMesh;

	UPROPERTY(EditAnywhere, Category = "Configuracion de Nivel")
	int32 NivelExclusivo;

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float Velocidad;

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float RangoMovimiento;

	// --- NUEVO: ATRIBUTO BASE DE VIDA ---
	UPROPERTY(EditAnywhere, Category = "Atributos")
	float Vida;

	// --- NUEVO: FUNCIÓN PARA RECIBIR DAÑO ---
	virtual void RecibirDanoEnemigo(float CantidadDano);

protected:
	FVector DireccionObjetivo;
	virtual void CalcularNuevaDireccion();
};


