// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EExclusivo.h"
#include "DronAnclaje.generated.h"

class USkeletalMeshComponent; // NUEVO: Para la malla visual del Fighter Drone

UCLASS()
class CENTCOSMOS_API ADronAnclaje : public AEExclusivo
{
	GENERATED_BODY()

public:
	ADronAnclaje();

protected:
	virtual void BeginPlay() override;

public:
	// --- COMPONENTE VISUAL DEL DRON ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	USkeletalMeshComponent* MallaFighterDrone;
	void OtorgarEscudoAEnemigo();

private:
	FTimerHandle TimerEscudoHandle;
};
