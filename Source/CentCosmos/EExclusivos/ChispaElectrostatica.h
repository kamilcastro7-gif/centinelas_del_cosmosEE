// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChispaElectrostatica.generated.h"

class UNiagaraComponent; // NUEVO: Para el sistema de partículas Niagara

UCLASS()
class CENTCOSMOS_API AChispaElectrostatica : public AActor
{
	GENERATED_BODY()

public:
	AChispaElectrostatica();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	// --- HITBOX INVISIBLE ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ChispaMesh;

	// --- EFECTO VISUAL NIAGARA ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* EfectoNiagara;

	FVector PosicionInicial;
	float AmplitudFlotacion;
	float VelocidadFlotacion;
	float TiempoFlotacion;

	// =========================================================================
	// ¡SOPORTE PARA LOS RESPALDOS QUE FALTABAN DE DECLARAR!
	// =========================================================================
	float FireRateOriginal;
	float VelocidadBoomerangOriginal;
	float TiempoCargaOriginal;

	void RestaurarAtributosJugador(AActor* JugadorCasteado);
};
