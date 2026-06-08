// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseLevelGameMode.h" 
#include "NivelDirector.h"
#include "Nivel3Builder.h"
#include "CentCosmosPawn.h"
#include "LV3_GameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ALV3_GameMode : public ABaseLevelGameMode
{
    GENERATED_BODY()

public:
    ALV3_GameMode();
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ANivelDirector* Director;

    UPROPERTY()
    ANivel3Builder* Builder;
};