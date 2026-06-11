// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrietaAntimateria.generated.h"

UCLASS()
class CENTCOSMOS_API AGrietaAntimateria : public AActor
{
    GENERATED_BODY()

public:
    AGrietaAntimateria();

    // Función que llamará tu Proyectil al golpear
    void ProcesarImpacto();

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshGrieta;

    int32 ProyectilesRecibidos = 0;
};