// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EditorTools : ModuleRules
{
	public EditorTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]{ "EditorTools/Public" });

		PrivateIncludePaths.AddRange(
            new string[]{ "EditorTools/Private" });
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "MoviePlayer", "RHI",
                                                            "XmlParser", "Json", "EOD" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
