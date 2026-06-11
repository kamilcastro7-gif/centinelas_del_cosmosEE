// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enjambre.h"
#include "EVigia_Del_Vacio.generated.h"


UCLASS()
class CENTINELA_DEL_COSMOS_API AEVigia_Del_Vacio : public AEnjambre
{
	GENERATED_BODY()
public:
	AEVigia_Del_Vacio();
	void moverVigia();
	void VigAtacar();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float VelocidadVigia; // Ahora controlará la frecuencia de oscilación

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float AmplitudPatrulla; // Cuánto se mueve hacia los lados (en centímetros)

private:
	FVector PosicionInicialSpawn;
};
