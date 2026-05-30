// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "EExclusivo.h"
#include "EcoCorrupto.generated.h"

UCLASS()
class CENTCOSMOS_API AEcoCorrupto : public AEExclusivo
{
	GENERATED_BODY()

public:
	AEcoCorrupto();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	bool bEsInvulnerable;
	TSubclassOf<AActor> ProyectilEstandarClass;

private:
	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	class UStaticMeshComponent* MallaPiramide;

	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	class UStaticMeshComponent* MallaNaveEco;

	FTimerHandle TimerTransformacionHandle;
	FTimerHandle TimerCicloAtaqueHandle;

	void TransformarEnNave();
	void EjecutarAccionCorrupta();

	FVector DireccionDash;
	bool bEstaHaciendoDash;
	float TiempoDashRestante;
	float VelocidadBase;
};
