// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EExclusivo.h"
#include "BombarderoFuego.generated.h"

class USkeletalMeshComponent; // NUEVO: Declaración para la malla visual

UCLASS()
class CENTCOSMOS_API ABombarderoFuego : public AEExclusivo
{
	GENERATED_BODY()

public:
	ABombarderoFuego();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// --- COMPONENTE VISUAL DEL DRON ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	USkeletalMeshComponent* MallaBombardero;
	void EjecutarDisparoFuego();

private:
	float VelocidadMovimiento;
	FVector PosicionInicial; // Guardaremos su X y Z originales

	FTimerHandle TimerAtaqueHandle;
	
};
