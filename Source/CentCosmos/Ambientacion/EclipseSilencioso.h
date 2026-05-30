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
    virtual void Tick(float DeltaTime) override;

    // Usamos EclipseMesh para evitar conflictos
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* EclipseMesh;

    FTimerHandle DashTimerHandle;

    UPROPERTY(EditAnywhere)
    float TiempoEntreDash = 3.0f;

    UPROPERTY(EditAnywhere)
    float DistanciaPorDash = 400.0f;

    void RealizarDash();
    void AplicarSilencio(AActor* OtherActor);
};
