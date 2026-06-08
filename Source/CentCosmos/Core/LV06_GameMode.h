// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NivelDirector.h"
#include "Nivel6Builder.h"
#include "CentCosmosPawn.h"
#include "LV06_GameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ALV06_GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALV06_GameMode();
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ANivelDirector* Director;

    UPROPERTY()
    ANivel6Builder* Builder;
};