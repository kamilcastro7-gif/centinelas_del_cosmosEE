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

	// --- NUEVO: DETECCIÓN DE IMPACTO ---
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	UStaticMeshComponent* FragmentoMesh;

	FVector DireccionMovimiento;

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float VelocidadFragmento;
};


