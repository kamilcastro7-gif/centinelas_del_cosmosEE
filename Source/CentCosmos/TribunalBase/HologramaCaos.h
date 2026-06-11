// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HologramaCaos.generated.h"

UCLASS()
class CENTCOSMOS_API AHologramaCaos : public AActor
{
    GENERATED_BODY()

public:
    AHologramaCaos();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    class UStaticMeshComponent* MallaHolograma;

    void Disparar();
};
