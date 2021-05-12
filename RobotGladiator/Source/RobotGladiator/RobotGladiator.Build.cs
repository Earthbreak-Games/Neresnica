// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RobotGladiator : ModuleRules
{
	public RobotGladiator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
