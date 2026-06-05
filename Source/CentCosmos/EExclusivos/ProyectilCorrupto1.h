// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProyectilCorrupto1.generated.h"

UCLASS()
class CENTCOSMOS_API AProyectilCorrupto1 : public AActor
{
    GENERATED_BODY()
public:
    AProyectilCorrupto1();
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Mesh;
    UPROPERTY(VisibleAnywhere) UProjectileMovementComponent* Movement;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
