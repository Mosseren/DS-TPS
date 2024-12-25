// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProj : ModuleRules
{
	public MyProj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateIncludePaths.Add("MyProj");
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" ,"UMG"});
	}
}
