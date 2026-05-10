// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Centinela_Del_CosmosGameMode.generated.h"

UCLASS()
class CENTINELA_DEL_COSMOS_API ACentinela_Del_CosmosGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACentinela_Del_CosmosGameMode();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    class AEnemyFactory* Factory;
    class AFacade* ManejadorHorda;
};