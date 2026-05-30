// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PAntimateria.generated.h"

UCLASS()
class CENTCOSMOS_API APAntimateria : public AActor
{
    GENERATED_BODY()

public:
    APAntimateria();

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ProyectilMesh;

    // Exponemos esto para configurar la velocidad desde la Grieta
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    class UProjectileMovementComponent* ProjectileMovement;
};