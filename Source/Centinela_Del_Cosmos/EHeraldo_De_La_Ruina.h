// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Enjambre.h"
#include "EHeraldo_De_La_Ruina.generated.h"

UCLASS()
class CENTINELA_DEL_COSMOS_API AEHeraldo_De_La_Ruina : public AEnjambre
{
	GENERATED_BODY()
public:
	AEHeraldo_De_La_Ruina();
	void moverHeraldo();

	UPROPERTY(EditAnywhere, Category = "Combate")
	float VelocidadHeraldo;
};
