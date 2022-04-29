// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyFirst : ModuleRules
{
	public MyFirst(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
