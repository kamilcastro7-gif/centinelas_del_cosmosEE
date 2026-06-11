// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enjambre.generated.h"

UCLASS()
class CENTINELA_DEL_COSMOS_API AEnjambre : public AActor
{
    GENERATED_BODY()
public:
    AEnjambre();
protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MallaEnemigo;

    // Dirección de rebote (1 o -1)
    FVector DireccionMovimiento;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
