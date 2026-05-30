// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enjambre.generated.h"

UCLASS()
class CENTCOSMOS_API AEnjambre : public AActor
{
    GENERATED_BODY()
public:
    AEnjambre();
protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MallaEnemigo;

    FVector DireccionMovimiento;

public:
    virtual void Tick(float DeltaTime) override;
};
