// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GhostForHire : ModuleRules
{
	public GhostForHire(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule", "GameplayTasks", "NavigationSystem", "UMG", "Slate", "SlateCore", "Niagara" });
	}
}
