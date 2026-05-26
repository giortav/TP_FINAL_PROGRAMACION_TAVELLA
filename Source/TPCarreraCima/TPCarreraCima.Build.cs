// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPCarreraCima : ModuleRules
{
	public TPCarreraCima(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "AITestSuite" });

		PublicIncludePaths.AddRange(new string[] {
			"TPCarreraCima",
			"TPCarreraCima/Variant_Platforming",
			"TPCarreraCima/Variant_Platforming/Animation",
			"TPCarreraCima/Variant_Combat",
			"TPCarreraCima/Variant_Combat/AI",
			"TPCarreraCima/Variant_Combat/Animation",
			"TPCarreraCima/Variant_Combat/Gameplay",
			"TPCarreraCima/Variant_Combat/Interfaces",
			"TPCarreraCima/Variant_Combat/UI",
			"TPCarreraCima/Variant_SideScrolling",
			"TPCarreraCima/Variant_SideScrolling/AI",
			"TPCarreraCima/Variant_SideScrolling/Gameplay",
			"TPCarreraCima/Variant_SideScrolling/Interfaces",
			"TPCarreraCima/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
