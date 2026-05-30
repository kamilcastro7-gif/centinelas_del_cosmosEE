// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ArenaCombate.generated.h"

UCLASS()
class CENTCOSMOS_API AArenaCombate : public AActor
{
    GENERATED_BODY()

public:
    AArenaCombate();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* RootEscena;
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Suelo;
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ParedNorte;
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ParedSur;
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ParedEste;
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ParedOeste;
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerArea;

    UFUNCTION()
    void AlEntrarEnArena(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
