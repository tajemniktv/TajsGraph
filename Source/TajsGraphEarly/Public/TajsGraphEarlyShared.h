// Copyright TajemnikTV. All rights reserved.
#pragma once

#include "CoreMinimal.h"

namespace TajsGraphEarlyShared
{
    struct FAssetInfoSearchDir
    {
        FString Directory;
        FString SourceKind;
    };

    const TCHAR *GetConfigSection();
    const TCHAR *GetEnableMaterialShaderMapDiagKey();
    const TCHAR *GetEnableMaterialShaderMapHashRemapKey();
    const TCHAR *GetEnableEngineDefaultMaterialOverridesKey();
    const TCHAR *GetDefaultMaterialOverrideKey();
    const TCHAR *GetDefaultDeferredDecalMaterialOverrideKey();
    const TCHAR *GetDefaultLightFunctionMaterialOverrideKey();
    const TCHAR *GetDefaultPostProcessMaterialOverrideKey();
    const TCHAR *GetWorldGridMaterialOverrideKey();

    void GatherConfigCandidatePaths(TArray<FString> &OutConfigPaths);
    bool ReadConfigBool(const TCHAR *KeyName, bool &OutValue);
    bool ReadConfigString(const TCHAR *KeyName, FString &OutValue);

    void GatherShaderDirCandidates(TArray<FString> &OutShaderDirs);
    void GatherAssetInfoSearchDirs(const FString &PlatformName, TArray<FAssetInfoSearchDir> &OutSearchDirs);

    FString NormalizeObjectPath(const FString &InObjectPath);
    bool NormalizeConfiguredObjectPath(const FString &RawPath, FString &OutObjectPath, FString &OutPackagePath,
                                       FString &OutError);
    FString NormalizeAssetPathFromFileSystem(const FString &FileSystemPath);
    FString ExtractShortName(const FString &AssetPath);
} // namespace TajsGraphEarlyShared
