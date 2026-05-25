// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProyectilBoomerang.generated.h"

UCLASS()
class CENTCOSMOS_API AProyectilBoomerang : public AActor
{
    GENERATED_BODY()
public:
    AProyectilBoomerang();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    class UStaticMeshComponent* ProyectilMesh;

    UPROPERTY()
    class ACentCosmosPawn* NaveDueno;

    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    UPROPERTY()
    float TiempoEnVuelo;
    UPROPERTY()
    bool bRegresando;
    float Velocidad;
    float TiempoDeGiro;
};
