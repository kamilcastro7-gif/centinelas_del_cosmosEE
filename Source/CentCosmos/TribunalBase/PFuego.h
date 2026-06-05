// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PFuego.generated.h"

UCLASS()
class CENTCOSMOS_API APFuego : public AActor
{
    GENERATED_BODY()

public:
    APFuego();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaEsfera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float Velocidad;

    FVector DireccionVuelo;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
