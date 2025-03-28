// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using UnrealBuildTool;
//[SupportedPlatforms(UnrealPlatformClass.Server)]
public class UE5OnlineProjectServerTarget : TargetRules
{
	public UE5OnlineProjectServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		//IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.AddRange(new string[] { "UE5OnlineProject" });
	}
}