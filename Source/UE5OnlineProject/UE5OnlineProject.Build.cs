// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5OnlineProject : ModuleRules
{
	public UE5OnlineProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "WebSockets", "SignalR",
			"HTTPBackend", "Json", "JsonUtilities", "OnlineSubsystem", "OnlineSubsystemUtils"
		});
	}
}
