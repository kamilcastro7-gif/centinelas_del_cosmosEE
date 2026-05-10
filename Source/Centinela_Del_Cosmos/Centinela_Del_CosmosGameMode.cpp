// Copyright Epic Games, Inc. All Rights Reserved.

#include "Centinela_Del_CosmosGameMode.h"
#include "Centinela_Del_CosmosPawn.h"

ACentinela_Del_CosmosGameMode::ACentinela_Del_CosmosGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ACentinela_Del_CosmosPawn::StaticClass();
}

