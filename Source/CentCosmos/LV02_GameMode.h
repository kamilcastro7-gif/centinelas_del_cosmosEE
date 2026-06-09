// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NivelDirector.h"
#include "Nivel2Builder.h"
#include "CentCosmosPawn.h"
#include "LV02_GameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ALV02_GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALV02_GameMode();
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ANivelDirector* Director;

    UPROPERTY()
    ANivel2Builder* Builder;
};