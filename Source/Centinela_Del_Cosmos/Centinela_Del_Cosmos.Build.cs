// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Centinela_Del_Cosmos : ModuleRules
{
	public Centinela_Del_Cosmos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
