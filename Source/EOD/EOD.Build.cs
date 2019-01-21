// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EOD : ModuleRules
{
	public EOD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {

            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
                "EOD/AI",
                "EOD/AnimInstances",
                "EOD/Characters",
                "EOD/Core",
                "EOD/Events",
                "EOD/Interactives",
                "EOD/Notifies",
                "EOD/Player",
                "EOD/SaveSystem",
                "EOD/Statics",
                "EOD/StatusEffects",
                "EOD/UI",
                "EOD/Weapons",
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "UMG",
                "AssetRegistry",
                "AIModule",
                "GameplayTasks",
                "XmlParser",
                "Json",
                "MoviePlayer",
            }
        );

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemNull" });

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
