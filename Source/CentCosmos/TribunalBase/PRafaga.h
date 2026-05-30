// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRafaga.generated.h"

UCLASS()
class CENTCOSMOS_API APRafaga : public AActor
{
    GENERATED_BODY()

public:
    APRafaga();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaProyectil;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
    float Velocidad;
};
