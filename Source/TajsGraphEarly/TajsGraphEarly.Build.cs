using UnrealBuildTool;
using System.IO;
using System;

public class TajsGraphEarly : ModuleRules
{
	public TajsGraphEarly(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

		// MINIMAL DEPENDENCIES FOR EARLY SHADER LOADING
		PublicDependencyModuleNames.AddRange(new string[] {
			// Core dependencies
			"Core",
			"CoreUObject",
			"Engine",
			
			// Rendering/Shader dependencies (REQUIRED for shader loading)
			"RenderCore",    // FGlobalShaderMap, FShaderCodeLibrary, FShaderCodeLibrary::OpenLibrary
			"RHI",           // GPU interface
			"Projects",      // IPluginManager
			"Json",
			"JsonUtilities",
			"SML",
		});

		// FactoryGame header stubs (for FactoryGame-specific logging categories, etc)
		PublicDependencyModuleNames.AddRange(new string[] {
			"DummyHeaders",
		});

		// ========================================
		// COMMENTED OUT - NOT NEEDED FOR THIS MODULE
		// ========================================
		// These dependencies are from the default template but are not required
		// PublicDependencyModuleNames.AddRange(new string[] {
		// 	"DeveloperSettings",      // Not needed
		// 	"PhysicsCore",            // Not needed
		// 	"InputCore",              // Not needed
		// 	"GeometryCollectionEngine", // Not needed
		// 	"AnimGraphRuntime",       // Not needed
		// 	"AssetRegistry",          // Not needed (maybe needed later for loading shader assets)
		// 	"NavigationSystem",       // Not needed
		// 	"AIModule",               // Not needed
		// 	"GameplayTasks",          // Not needed
		// 	"SlateCore", "Slate", "UMG", // UI dependencies - not needed
		// 	"ApplicationCore",        // Not needed
		// 	"CinematicCamera",        // Not needed
		// 	"Foliage",                // Not needed
		// 	"NetCore",                // Not needed
		// 	"GameplayTags",           // Not needed
		// 	"Json", "JsonUtilities"   // Might be needed for Phase 2 config loading
		// });

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			// Minimal editor dependencies
			// PublicDependencyModuleNames.AddRange(new string[] { "AnimGraph" }); // Not needed for this module
		}

		PublicIncludePaths.AddRange(new string[] {
			// ... add public include paths required here ...
		});

		PrivateIncludePaths.AddRange(new string[] {
			// ... add private include paths required here ...
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			// ... add private dependencies that you statically link with here ...	
			"Projects",
			"Json",
			"JsonUtilities",
			"SML"
		});

		DynamicallyLoadedModuleNames.AddRange(new string[] {
			// ... add any modules that your module loads dynamically here ...
		});

		// Stage all cooked shader payload files with the mod package.
		string shaderDir = Path.Combine(PluginDirectory, "Content", "Shaders");
		if (Directory.Exists(shaderDir))
		{
			foreach (string shaderFile in Directory.GetFiles(shaderDir, "*", SearchOption.AllDirectories))
			{
				RuntimeDependencies.Add(shaderFile, StagedFileType.UFS);
			}
		}
	}
}
