// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NivelDirector.h"
#include "Nivel4Builder.h"
#include "CentCosmosPawn.h"
#include "LV04_GameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ALV04_GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALV04_GameMode();
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ANivelDirector* Director;

    UPROPERTY()
    ANivel4Builder* Builder;
};