// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FragmentoErupcion.generated.h"

UCLASS()
class CENTCOSMOS_API AFragmentoErupcion : public AActor
{
    GENERATED_BODY()

public:
    AFragmentoErupcion();
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    void SoltarRastro();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* FragmentoMesh;

    FVector DireccionVuelo;
    float Velocidad;

    // Timer para dejar rastro (como en tu proyectil)
    FTimerHandle TimerRastrosHandle;
    
};
