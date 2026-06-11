// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Facade.generated.h"

UCLASS()
class CENTINELA_DEL_COSMOS_API AFacade : public AActor
{
	GENERATED_BODY()

public:
    // Sets default values for this actor's properties
	AFacade();
	void MoverHorda();
};
