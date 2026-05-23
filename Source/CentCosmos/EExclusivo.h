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

	// El componente de malla que van a heredar todos tus enemigos exclusivos
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UStaticMeshComponent* EnemigoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion de Nivel")
	int32 NivelExclusivo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float Velocidad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float RangoMovimiento;

protected:
	FVector DireccionObjetivo;
	virtual void CalcularNuevaDireccion();
};
