// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "TribunalBase.generated.h"

UCLASS()
class CENTCOSMOS_API ATribunalBase : public APawn
{
    GENERATED_BODY()

public:
    ATribunalBase();

    virtual void OnConstruction(const FTransform& Transform) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
    UStaticMeshComponent* MallaTribunal;

    UPROPERTY(BlueprintReadWrite, Category = "Tribunal")
    bool bEstaActivo;

    virtual void ActivarJefe() { bEstaActivo = true; }
};
