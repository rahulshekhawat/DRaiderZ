// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EOD : ModuleRules
{
	public EOD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "EOD/Public/Actors",
                "EOD/Public/AI",
                "EOD/Public/AI/Components",
                "EOD/Public/AnimInstances",
                "EOD/Public/Characters",
                "EOD/Public/Characters/Components",
                "EOD/Public/Characters/States",
                "EOD/Public/Core",
                "EOD/Public/Gameplay",
                "EOD/Public/Gameplay/Skills",
                "EOD/Public/Gameplay/Effects",
                "EOD/Public/Gameplay/Events",
                "EOD/Public/Interactives",
                "EOD/Public/Notifies",
                "EOD/Public/Player",
                "EOD/Public/Player/Components",
                "EOD/Public/SaveSystem",
                "EOD/Public/Statics",
                "EOD/Public/StatusEffects",
                "EOD/Public/UI",
                "EOD/Public/UI/Menu",
                "EOD/Public/Weapons",
                "EOD/Public/Weapons/Components",
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {

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
                "GameplayTags"
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
