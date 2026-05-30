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
			System.IO.Path.Combine(ModuleDirectory, "Core"),
			System.IO.Path.Combine(ModuleDirectory, "Player"),
			System.IO.Path.Combine(ModuleDirectory, "Enemies"),
			System.IO.Path.Combine(ModuleDirectory, "Projectiles"),
			System.IO.Path.Combine(ModuleDirectory, "Obstacles"),
			System.IO.Path.Combine(ModuleDirectory, "Environment"),
			System.IO.Path.Combine(ModuleDirectory, "Patterns"),
			System.IO.Path.Combine(ModuleDirectory, "Patterns/FactoryMethod"),
			System.IO.Path.Combine(ModuleDirectory, "Patterns/Facade"),
			System.IO.Path.Combine(ModuleDirectory, "Patterns/Observer"),
			System.IO.Path.Combine(ModuleDirectory, "Patterns/Decorator"),
			System.IO.Path.Combine(ModuleDirectory, "EExclusivos"),
			System.IO.Path.Combine(ModuleDirectory, "Ambientacion"),
			System.IO.Path.Combine(ModuleDirectory, "TribunalBase")
		});
	}
}
