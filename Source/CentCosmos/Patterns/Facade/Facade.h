// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Facade.generated.h"

UCLASS()
class CENTCOSMOS_API AFacade : public AActor
{
    GENERATED_BODY()
public:
    AFacade();
    void MoverHorda();
};
