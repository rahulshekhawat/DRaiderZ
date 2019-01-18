// Copyright 2018 Moikkai Games. All Rights Reserved.

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class EODLoadingScreen : ModuleRules
{
    public EODLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] { "EODLoadingScreen/Public" });

        PrivateIncludePaths.AddRange(
            new string[] { "EODLoadingScreen/Private" });

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"MoviePlayer",
				"Slate",
				"SlateCore",
				"InputCore"
			}
		);
	}
}

