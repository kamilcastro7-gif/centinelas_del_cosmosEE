// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EclipseSilencioso.generated.h"

class UParticleSystemComponent; // NUEVO: Para el sistema de partículas Cascade

UCLASS()
class CENTCOSMOS_API AEclipseSilencioso : public AActor
{
	GENERATED_BODY()

public:
	AEclipseSilencioso();
	virtual void BeginPlay() override;

	// --- HITBOX INVISIBLE ---
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* EclipseMesh;

	// --- EFECTO VISUAL AURA ---
	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	UParticleSystemComponent* EfectoAura;

	FTimerHandle DashTimerHandle;

	UPROPERTY(EditAnywhere)
	float TiempoEntreDash = 3.0f;

	UPROPERTY(EditAnywhere)
	float DistanciaPorDash = 400.0f;

	void RealizarDash();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
