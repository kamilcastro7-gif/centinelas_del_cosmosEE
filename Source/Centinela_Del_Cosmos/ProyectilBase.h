// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h" // Necesario para el movimiento
#include "Components/SphereComponent.h"
#include "ProyectilBase.generated.h"


UCLASS()
class CENTINELA_DEL_COSMOS_API AProyectilBase : public AActor
{
	GENERATED_BODY()

public:
	AProyectilBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Componentes principales
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UStaticMeshComponent* MallaProyectil;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movimiento")
	class UProjectileMovementComponent* MovimientoProyectil;

	// Lógica de juego
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estadisticas")
	int32 VidaProyectil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estadisticas")
	bool bEsSeguidor;

	// Función para que el jugador destruya el proyectil del Heraldo
	void RecibirImpacto();
};
