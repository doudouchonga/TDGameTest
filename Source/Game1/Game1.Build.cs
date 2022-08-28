// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Game1 : ModuleRules
{
	public Game1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });

        PublicIncludePaths.AddRange(new string[]
        {
                "Game1/AI",
        });

    }
}
