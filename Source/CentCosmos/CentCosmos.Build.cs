// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CentCosmos : ModuleRules
{
	public CentCosmos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PublicIncludePaths.AddRange(new string[] {
			System.IO.Path.Combine(ModuleDirectory),
			System.IO.Path.Combine(ModuleDirectory, "Patterns"),
			System.IO.Path.Combine(ModuleDirectory, "Patterns/FactoryMethod"),
			System.IO.Path.Combine(ModuleDirectory, "Patterns/Facade")
		});
	}
}
