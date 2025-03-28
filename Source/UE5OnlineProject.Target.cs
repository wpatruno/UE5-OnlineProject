// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5OnlineProjectTarget : TargetRules
{
	public UE5OnlineProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("UE5OnlineProject");
	}
}
