// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Lucas : ModuleRules
{
	public Lucas(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Blutility", "Niagara", "UMG", "HoudiniEngineRuntime", "MediaAssets", "UMGEditor", "EditorScriptingUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] { System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "Source/Editor/Blutility/Private" });
	}
}
