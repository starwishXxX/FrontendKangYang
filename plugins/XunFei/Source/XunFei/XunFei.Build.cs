// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

using System.IO;

public class XunFei : ModuleRules
{

	private string ModulePath
	{
		get { return ModuleDirectory; }
	}

	public XunFei(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "Public")
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "Private"),
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Json"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		string XunFeiSDKIncludePath = ModulePath + "/XunFeiSDK/include/";

		string XunFeiSDKLibraryPath = ModulePath + "/XunFeiSDK/libs/";

		string XunFeiSDKImportLibraryName = Path.Combine(XunFeiSDKLibraryPath, "msc_x64.lib");

		PublicIncludePaths.Add(XunFeiSDKIncludePath);

		//PublicLibraryPaths.Add(XunFeiSDKLibraryPath);

		PublicAdditionalLibraries.Add(XunFeiSDKImportLibraryName);

	}
}
