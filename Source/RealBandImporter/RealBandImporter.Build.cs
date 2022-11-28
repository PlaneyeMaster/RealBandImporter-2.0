// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class RealBandImporter : ModuleRules
{
	public RealBandImporter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        //var EngineDir = Path.GetFullPath(Target.RelativeEnginePath);
        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
                Path.Combine(EngineDirectory, @"Plugins/Enterprise/DatasmithGLTFImporter/Source/DatasmithGLTFTranslator/Private"),

                Path.Combine(EngineDirectory, @"Plugins/Editor/AssetManagerEditor/Source/AssetManagerEditor/Public"),

                Path.Combine(EngineDirectory, @"Source/Editor/ContentBrowser/Private"),

                Path.Combine(EngineDirectory, @"Plugins/Enterprise/DataSmithGLTFImporter/Source/DataSmithGLTFTranslator/Private"),

                Path.Combine(EngineDirectory, @"Plugins/Enterprise/VariantManagerContent/Source/VariantManagerContent/Public"),
                Path.Combine(EngineDirectory, @"Source/Editor/UnrealEd/Public"),
                Path.Combine(EngineDirectory, @"Source/Editor/UnrealEd/Classes/Factories")
               }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			    //"..\\Plugins\\Enterprise\\DatasmithGLTFImporter\\Source\\DatasmithGLTFTranslator\\Private",
               // Path.Combine(ModuleDirectory, "Private")
			   
               Path.Combine(EngineDirectory, @"Plugins/Enterprise/DatasmithGLTFImporter/Source/DatasmithGLTFTranslator/Private"),

               Path.Combine(EngineDirectory, @"Plugins/Editor/AssetManagerEditor/Source/AssetManagerEditor/Public"),
               Path.Combine(EngineDirectory, @"Plugins/Editor/AssetManagerEditor/Source/AssetManagerEditor/Private"),
               Path.Combine(EngineDirectory, @"Source/Editor/ContentBrowser/Private"),
               Path.Combine(EngineDirectory, @"Source/Editor/ContentBrowser/Public"),
               Path.Combine(EngineDirectory, @"Plugins/Enterprise/VariantManagerContent/Source/VariantManagerContent/Public"),

               Path.Combine(EngineDirectory, @"Source/Editor/UnrealEd/Public"),
               Path.Combine(EngineDirectory, @"Source/Editor/UnrealEd/Classes/Factories")

    }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "DatasmithContent",
                "DatasmithGLTFTranslator",
                "DatasmithTranslator",
				"DatasmithImporter",
                "GLTFImporter",
                "AssetManagerEditor",
				"ContentBrowser",
                "ContentBrowserData",
                "UnrealEd",
                "VariantManagerContent",
                "AssetTagsEditor",
                
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
                "AssetTagsEditor",
                "DatasmithCore",
                "DatasmithContent",
                "DatasmithGLTFTranslator",
                "ExternalSource",
                "DatasmithTranslator",
                "DatasmithImporter",
                "PropertyEditor",
                "EditorFramework",
                "GLTFCore",
                "GLTFImporter",
                "UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"Json",
                "ImageCore",        
                "EditorWidgets",
                "EditorStyle",
                "WebBrowser",
                "AssetRegistry",
                
                "AssetTools",
                "ContentBrowser",
                "ContentBrowserData",
                "AssetManagerEditor",
                "VariantManagerContent",
                "UnrealEd"
				
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
                
				// ... add any modules that your module loads dynamically here ...
				
			}
			);
       
    }
}
