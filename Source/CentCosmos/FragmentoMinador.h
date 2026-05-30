// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FragmentoMinador.generated.h"

UCLASS()
class CENTCOSMOS_API AFragmentoMinador : public AActor
{
	GENERATED_BODY()

public:
	AFragmentoMinador();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Componente de malla independiente para este fragmento
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UStaticMeshComponent* FragmentoMesh;

	// Dirección del eje cartesiano asignada al spawnear
	FVector DireccionMovimiento;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float VelocidadFragmento;
};
