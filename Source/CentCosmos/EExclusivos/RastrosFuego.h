// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RastrosFuego.generated.h"

UCLASS()
class CENTCOSMOS_API ARastrosFuego : public AActor
{
	GENERATED_BODY()

public:
	ARastrosFuego();

protected:
	virtual void BeginPlay() override;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	void AplicarDanoContinuo();

private:
	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	class UStaticMeshComponent* FuegoMesh;

	// --- NUEVO: Componente visual para la partícula ---
	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	class UParticleSystemComponent* EfectoFuego;

	// --- LOGICA DE DAÑO CONSTANTE ---
	FTimerHandle TimerDanoContinuo;
	class ACentCosmosPawn* JugadorEnFuego;
};


