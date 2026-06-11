// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilBoomerang.generated.h"

UCLASS()
class CENTINELA_DEL_COSMOS_API AProyectilBoomerang : public AActor
{
	GENERATED_BODY()

public:
	AProyectilBoomerang();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* ProyectilMesh;

	// Puntero para conocer la nave dueña y poder rastrearla al regresar
	UPROPERTY()
	class ACentinela_Del_CosmosPawn* NaveDueno;

	virtual void Tick(float DeltaTime) override;

	// Función encargada de gestionar los solapamientos (Overlap) con enemigos y balas
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float TiempoEnVuelo;
	bool bRegresando;
	float Velocidad;
	float TiempoDeGiro;
};
