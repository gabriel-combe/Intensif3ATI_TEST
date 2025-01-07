// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Intensif3ATI_TEST : ModuleRules
{
	public Intensif3ATI_TEST(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}
