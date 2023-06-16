// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TriggerCPPv3 : ModuleRules
{
	public TriggerCPPv3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "OnlineSubsystem", "OnlineSubsystemUtils", "Sockets" });
        
		DynamicallyLoadedModuleNames.AddRange(new string[] { "OnlineSubsystemNull", "OnlineSubsystemSteam" });

    }
}
