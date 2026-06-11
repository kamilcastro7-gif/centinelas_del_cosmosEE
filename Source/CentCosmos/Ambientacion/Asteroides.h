// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Asteroides.generated.h"

UCLASS()
class CENTCOSMOS_API AAsteroide : public AActor
{
    GENERATED_BODY()

public:
    AAsteroide();
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    void CambiarDireccion();

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* Movement;
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
    FTimerHandle TimerCambioDireccion;
};
