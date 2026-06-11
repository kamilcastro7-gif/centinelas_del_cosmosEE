// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "TribunalBase.h"
#include "Components/ChildActorComponent.h"
#include "TribunalVigilante.generated.h"

UCLASS()
class CENTCOSMOS_API ATribunalVigilante : public ATribunalBase
{
	GENERATED_BODY()

public:
	ATribunalVigilante();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:
	virtual void Tick(float DeltaTime) override;

	void ActivarJefe();

	UChildActorComponent* ComponenteRayo;
	float VelocidadGiro;
	float RangoDeteccion;

	UPROPERTY(EditAnywhere, Category = "Invocacion")
	TSubclassOf<AActor> ClaseVastago;

	UPROPERTY(EditAnywhere, Category = "Invocacion")
	TSubclassOf<AActor> ClaseHeraldo;

	void InvocacionEnCirculo(TSubclassOf<AActor> ClaseEnemigo, int32 Cantidad, float Radio);

private:
	bool bVastagosInvocados;
	bool bHeraldosInvocados;
	bool bJefeDerrotado; 
};