// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Facade.h"
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
	// Puntero para controlar la Fachada
	AFacade* ManejadorHorda;

	// Clases para el Spawn (se asignan en el constructor o BP)
	TSubclassOf<class AEVastago_Del_infierno> ClaseVastago;
	TSubclassOf<class AEVigia_Del_Vacio> ClaseVigia;
	TSubclassOf<class AEHeraldo_De_La_Ruina> ClaseHeraldo;
};



