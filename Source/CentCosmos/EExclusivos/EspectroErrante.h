// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EExclusivo.h" 
#include "EspectroErrante.generated.h"

UCLASS()
class CENTCOSMOS_API AEspectroErrante : public AEExclusivo
{
	GENERATED_BODY()

public:
	AEspectroErrante();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// --- SOBRESCRIBIMOS RECIBIR DAÑO PARA GESTIONAR INVULNERABILIDAD ---
	virtual void RecibirDanoEnemigo(float CantidadDano) override;

	bool bEsInvulnerable;

private:
	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	class UStaticMeshComponent* MallaCuerpo;

	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	class UStaticMeshComponent* MallaNucleo;

	float VelocidadFlotacion;
	FTimerHandle TimerFaseInvisibleHandle;
	FTimerHandle TimerFaseExpuestoHandle;
	FTimerHandle TimerRumboHandle;
	FVector DireccionErrante;

	UFUNCTION()
	void CambiarDireccionAleatoria();
	void EntrarFaseInvisible();
	void EntrarFaseExpuesto();
	void EjecutarDisparoEspectral();
};


