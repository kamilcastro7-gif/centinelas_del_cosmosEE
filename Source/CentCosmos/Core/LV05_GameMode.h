// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NivelDirector.h"
#include "Nivel5Builder.h"
#include "CentCosmosPawn.h"
#include "LV05_GameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ALV05_GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALV05_GameMode();
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ANivelDirector* Director;

    UPROPERTY()
    ANivel5Builder* Builder;
};