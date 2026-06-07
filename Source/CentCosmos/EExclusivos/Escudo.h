// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Escudo.generated.h"

class UParticleSystemComponent; // NUEVO: Declaración para el componente de partículas

UCLASS()
class CENTCOSMOS_API AEscudo : public AActor
{
	GENERATED_BODY()

public:
	AEscudo();

protected:
	virtual void BeginPlay() override;

public:
	// --- HITBOX DEL ESCUDO ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Escudo")
	UStaticMeshComponent* EscudoMesh;

	// --- COMPONENTE VISUAL (EL AURA) ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Escudo")
	UParticleSystemComponent* EscudoParticulas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Escudo")
	float VidaEscudo;

	void RecibirDanoEscudo(float CantidadDano);
};
