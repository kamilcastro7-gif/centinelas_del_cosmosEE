// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CentCosmosGameMode.generated.h"

UCLASS()
class CENTCOSMOS_API ACentCosmosGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACentCosmosGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	class ANivelDirector* Director;

	UPROPERTY()
	class ANivelFacilBuilder* BuilderFacil;

	UPROPERTY()
	class AFacade* ManejadorHorda;
};