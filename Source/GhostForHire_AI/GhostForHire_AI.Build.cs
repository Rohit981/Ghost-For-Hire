// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GhostForHire_AI : ModuleRules
{
	public GhostForHire_AI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule", "GameplayTasks", "NavigationSystem", "UMG", "Slate", "SlateCore" });
	}
}
