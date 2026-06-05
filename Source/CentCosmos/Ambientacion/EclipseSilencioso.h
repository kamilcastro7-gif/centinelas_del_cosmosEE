// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EclipseSilencioso.generated.h"

UCLASS()
class CENTCOSMOS_API AEclipseSilencioso : public AActor
{
	GENERATED_BODY()

public:
	AEclipseSilencioso();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* EclipseMesh;

	FTimerHandle DashTimerHandle;

	UPROPERTY(EditAnywhere)
	float TiempoEntreDash = 3.0f;

	UPROPERTY(EditAnywhere)
	float DistanciaPorDash = 400.0f;

	void RealizarDash();

	// Cambiamos el Tick por el Overlap dinámico
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
