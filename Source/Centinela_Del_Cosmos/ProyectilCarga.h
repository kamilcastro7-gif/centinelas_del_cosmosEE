// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilCarga.generated.h"

UCLASS()
class CENTINELA_DEL_COSMOS_API AProyectilCarga : public AActor
{
	GENERATED_BODY()

public:
	AProyectilCarga();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* ProyectilMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	// Escala el tamaño del proyectil frame a frame en base al tiempo que retienes el botón
	void InicializarCarga(float TiempoCarga);

	// Libera el proyectil inyectándole la dirección exacta de la nave para que nunca se quede estático
	void LiberarProyectil(float TiempoCargaFinal, FVector DireccionLanzamiento);

	// Función encargada de gestionar los impactos contra los enemigos u otros proyectiles
	UFUNCTION()
	void AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	float DanoBase;
};
