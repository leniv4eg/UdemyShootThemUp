// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class UdemyShootThemUp : ModuleRules
{
	public UdemyShootThemUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] 
		{ 
			"UdemyShootThemUp/Public/Player", 
			"UdemyShootThemUp/Public/Components", 
			"UdemyShootThemUp/Public/Dev", 
			"UdemyShootThemUp/Public/Weapon",
            "UdemyShootThemUp/Public/UI",
			"UdemyShootThemUp/Public/Animations"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
