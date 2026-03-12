#include "TajsGraphEarlyShared.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"
#include "Internationalization/Text.h"

namespace TajsGraphEarlyShared
{
	namespace
	{
		const TCHAR* const ConfigSection = TEXT("ShaderDiagnostics");
		const TCHAR* const EnableMaterialShaderMapDiagKey = TEXT("EnableMaterialShaderMapDiag");
		const TCHAR* const EnableMaterialShaderMapHashRemapKey = TEXT("EnableMaterialShaderMapHashRemap");
		const TCHAR* const EnableEngineDefaultMaterialOverridesKey = TEXT("EnableEngineDefaultMaterialOverrides");
		const TCHAR* const DefaultMaterialOverrideKey = TEXT("DefaultMaterialOverride");
		const TCHAR* const DefaultDeferredDecalMaterialOverrideKey = TEXT("DefaultDeferredDecalMaterialOverride");
		const TCHAR* const DefaultLightFunctionMaterialOverrideKey = TEXT("DefaultLightFunctionMaterialOverride");
		const TCHAR* const DefaultPostProcessMaterialOverrideKey = TEXT("DefaultPostProcessMaterialOverride");
		const TCHAR* const WorldGridMaterialOverrideKey = TEXT("WorldGridMaterialOverride");

		static void AddUniqueNonEmptyPath(TArray<FString>& OutPaths, const FString& CandidatePath)
		{
			if (!CandidatePath.IsEmpty())
			{
				OutPaths.AddUnique(CandidatePath);
			}
		}

		static FString ConvertPackagePathToObjectPath(const FString& PackagePath)
		{
			const FString ShortName = FPackageName::GetShortName(PackagePath);
			return ShortName.IsEmpty()
				? FString()
				: FString::Printf(TEXT("%s.%s"), *PackagePath, *ShortName);
		}
	}

	const TCHAR* GetConfigSection()
	{
		return ConfigSection;
	}

	const TCHAR* GetEnableMaterialShaderMapDiagKey()
	{
		return EnableMaterialShaderMapDiagKey;
	}

	const TCHAR* GetEnableMaterialShaderMapHashRemapKey()
	{
		return EnableMaterialShaderMapHashRemapKey;
	}

	const TCHAR* GetEnableEngineDefaultMaterialOverridesKey()
	{
		return EnableEngineDefaultMaterialOverridesKey;
	}

	const TCHAR* GetDefaultMaterialOverrideKey()
	{
		return DefaultMaterialOverrideKey;
	}

	const TCHAR* GetDefaultDeferredDecalMaterialOverrideKey()
	{
		return DefaultDeferredDecalMaterialOverrideKey;
	}

	const TCHAR* GetDefaultLightFunctionMaterialOverrideKey()
	{
		return DefaultLightFunctionMaterialOverrideKey;
	}

	const TCHAR* GetDefaultPostProcessMaterialOverrideKey()
	{
		return DefaultPostProcessMaterialOverrideKey;
	}

	const TCHAR* GetWorldGridMaterialOverrideKey()
	{
		return WorldGridMaterialOverrideKey;
	}

	void GatherConfigCandidatePaths(TArray<FString>& OutConfigPaths)
	{
		OutConfigPaths.Reset();

		if (TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("TajsGraph")))
		{
			AddUniqueNonEmptyPath(OutConfigPaths, FPaths::Combine(Plugin->GetBaseDir(), TEXT("Config"), TEXT("Engine.ini")));
		}

		AddUniqueNonEmptyPath(OutConfigPaths, FPaths::Combine(FPaths::ProjectDir(), TEXT("Mods"), TEXT("TajsGraph"), TEXT("Config"), TEXT("Engine.ini")));
		AddUniqueNonEmptyPath(OutConfigPaths, GEngineIni);
	}

	bool ReadConfigBool(const TCHAR* KeyName, bool& OutValue)
	{
		TArray<FString> ConfigCandidates;
		GatherConfigCandidatePaths(ConfigCandidates);

		for (const FString& ConfigPath : ConfigCandidates)
		{
			if (FPaths::FileExists(ConfigPath) && GConfig->GetBool(ConfigSection, KeyName, OutValue, ConfigPath))
			{
				return true;
			}
		}

		return false;
	}

	bool ReadConfigString(const TCHAR* KeyName, FString& OutValue)
	{
		TArray<FString> ConfigCandidates;
		GatherConfigCandidatePaths(ConfigCandidates);

		for (const FString& ConfigPath : ConfigCandidates)
		{
			if (FPaths::FileExists(ConfigPath) && GConfig->GetString(ConfigSection, KeyName, OutValue, ConfigPath))
			{
				return true;
			}
		}

		return false;
	}

	void GatherShaderDirCandidates(TArray<FString>& OutShaderDirs)
	{
		OutShaderDirs.Reset();

		AddUniqueNonEmptyPath(OutShaderDirs, TEXT("/TajsGraph/Shaders"));
		AddUniqueNonEmptyPath(OutShaderDirs, TEXT("../../../FactoryGame/Mods/TajsGraph/Content/Shaders"));
		AddUniqueNonEmptyPath(OutShaderDirs, FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), TEXT("Mods"), TEXT("TajsGraph"), TEXT("Content"), TEXT("Shaders"))));

		if (TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("TajsGraph")))
		{
			AddUniqueNonEmptyPath(OutShaderDirs, FPaths::ConvertRelativePathToFull(FPaths::Combine(Plugin->GetContentDir(), TEXT("Shaders"))));
		}

		AddUniqueNonEmptyPath(OutShaderDirs, FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("TajsGraph"), TEXT("Content"), TEXT("Shaders"))));
	}

	void GatherAssetInfoSearchDirs(const FString& PlatformName, TArray<FAssetInfoSearchDir>& OutSearchDirs)
	{
		OutSearchDirs.Reset();

		TArray<FString> ShaderDirs;
		GatherShaderDirCandidates(ShaderDirs);
		for (const FString& ShaderDir : ShaderDirs)
		{
			OutSearchDirs.Add({ ShaderDir, TEXT("shipped") });
		}

		OutSearchDirs.Add({
			FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Shaders"), PlatformName + TEXT("-") + PlatformName)),
			TEXT("dev_saved")
		});
		OutSearchDirs.Add({
			FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Cooked"), TEXT("Windows"), TEXT("FactoryGame"), TEXT("Content"))),
			TEXT("dev_cooked_content")
		});
		OutSearchDirs.Add({
			FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Cooked"), TEXT("Windows"), TEXT("FactoryGame"), TEXT("Metadata"), TEXT("ShaderLibrarySource"))),
			TEXT("dev_cooked_metadata")
		});

		TArray<FAssetInfoSearchDir> UniqueDirs;
		for (const FAssetInfoSearchDir& SearchDir : OutSearchDirs)
		{
			bool bAlreadyAdded = false;
			for (const FAssetInfoSearchDir& Existing : UniqueDirs)
			{
				if (Existing.Directory.Equals(SearchDir.Directory, ESearchCase::CaseSensitive))
				{
					bAlreadyAdded = true;
					break;
				}
			}

			if (!bAlreadyAdded && !SearchDir.Directory.IsEmpty())
			{
				UniqueDirs.Add(SearchDir);
			}
		}

		OutSearchDirs = MoveTemp(UniqueDirs);
	}

	FString NormalizeObjectPath(const FString& InObjectPath)
	{
		FString Result = InObjectPath.TrimStartAndEnd();
		Result.ReplaceInline(TEXT("\\"), TEXT("/"));
		FPaths::RemoveDuplicateSlashes(Result);

		const int32 DotIndex = Result.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		if (DotIndex != INDEX_NONE)
		{
			Result.LeftInline(DotIndex);
		}

		return Result;
	}

	bool NormalizeConfiguredObjectPath(const FString& RawPath, FString& OutObjectPath, FString& OutPackagePath, FString& OutError)
	{
		OutObjectPath.Reset();
		OutPackagePath.Reset();
		OutError.Reset();

		FString Candidate = RawPath.TrimStartAndEnd();
		Candidate.ReplaceInline(TEXT("\\"), TEXT("/"));
		FPaths::RemoveDuplicateSlashes(Candidate);
		if (Candidate.IsEmpty())
		{
			OutError = TEXT("path is empty");
			return false;
		}

		if (Candidate.Contains(TEXT(".")))
		{
			FText ErrorText;
			if (!FPackageName::IsValidObjectPath(Candidate, &ErrorText))
			{
				OutError = ErrorText.ToString();
				return false;
			}

			OutObjectPath = Candidate;
			OutPackagePath = FPackageName::ObjectPathToPackageName(Candidate);
			return true;
		}

		FText ErrorText;
		if (!FPackageName::IsValidLongPackageName(Candidate, false, &ErrorText))
		{
			OutError = ErrorText.ToString();
			return false;
		}

		OutPackagePath = Candidate;
		OutObjectPath = ConvertPackagePathToObjectPath(Candidate);
		if (OutObjectPath.IsEmpty())
		{
			OutError = TEXT("could not derive object name from package path");
			return false;
		}

		return true;
	}

	FString NormalizeAssetPathFromFileSystem(const FString& FileSystemPath)
	{
		FString Normalized = FileSystemPath;
		FPaths::NormalizeFilename(Normalized);

		const FString EngineMarker = TEXT("/Engine/Content/");
		const FString ProjectMarker = TEXT("/FactoryGame/Content/");
		const FString ModMarker = TEXT("/FactoryGame/Mods/TajsGraph/Content/");

		auto ConvertRelativeAssetPath = [](const FString& RelativePath, const TCHAR* RootPath)
		{
			FString NormalizedRelativePath = RelativePath;
			NormalizedRelativePath.ReplaceInline(TEXT("\\"), TEXT("/"));
			FPaths::RemoveDuplicateSlashes(NormalizedRelativePath);

			const FString BaseFilename = FPaths::GetBaseFilename(NormalizedRelativePath, false);
			const FString Directory = FPaths::GetPath(NormalizedRelativePath);
			return Directory.IsEmpty()
				? FString::Printf(TEXT("%s%s"), RootPath, *BaseFilename)
				: FString::Printf(TEXT("%s%s/%s"), RootPath, *Directory, *BaseFilename);
		};

		if (const int32 Index = Normalized.Find(ModMarker, ESearchCase::IgnoreCase); Index != INDEX_NONE)
		{
			return ConvertRelativeAssetPath(Normalized.Mid(Index + ModMarker.Len()), TEXT("/TajsGraph/"));
		}

		if (const int32 Index = Normalized.Find(EngineMarker, ESearchCase::IgnoreCase); Index != INDEX_NONE)
		{
			return ConvertRelativeAssetPath(Normalized.Mid(Index + EngineMarker.Len()), TEXT("/Engine/"));
		}

		if (const int32 Index = Normalized.Find(ProjectMarker, ESearchCase::IgnoreCase); Index != INDEX_NONE)
		{
			return ConvertRelativeAssetPath(Normalized.Mid(Index + ProjectMarker.Len()), TEXT("/Game/FactoryGame/"));
		}

		return FString();
	}

	FString ExtractShortName(const FString& AssetPath)
	{
		if (AssetPath.IsEmpty())
		{
			return FString();
		}

		FString Normalized = AssetPath;
		Normalized.ReplaceInline(TEXT("\\"), TEXT("/"));
		const int32 SlashIndex = Normalized.Find(TEXT("/"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		return SlashIndex == INDEX_NONE ? Normalized : Normalized.Mid(SlashIndex + 1);
	}
}
