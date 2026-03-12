#pragma once

#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "InstancedFoliageActor.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/Scene.h"
#include "Modules/ModuleManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "FGColoredInstanceMeshProxy.h"
#include "FGAtmosphereVolume.h"
#include "FGWaterVolume.h"
#include "FGGasPillar.h"
#include "FGDamageOverTimeVolume.h"
#include "FGBuildable.h"

#include "Patching/NativeHookManager.h"

#include "TajsGraphModule.h"
#include "TajsGraphFoliageAccess.h"

class AActor;
class UStaticMeshComponent;

namespace TajsGraphInternal {

    struct FPPVConfig {
        bool bEnabled = true;

        bool bOverrideLumenMethods = true;
        bool bOverrideLumenValues = true;
        bool bOverrideLumenRayLightingMode = true;
        bool bOverrideLumenFrontLayerTranslucencyReflections = true;
        bool bOverrideLumenMaxReflectionBounces = true;
        bool bOverrideIndirectLightingColor = false;
        bool bOverrideIndirectLightingIntensity = true;

        bool bForceInstancing = true;
        bool bForceLumenInstancing = true;
        bool bForceNanite = false;
        bool bForceNaniteForMasked = false;
        bool bFixBuildableFoliage = false;
        bool bEnableAssetRemap = false;
        bool bEnableLogging = false;
        bool bEnableDebugLogging = false;
        bool bPollExternalConfigChanges = false;

        bool bLumen = true;
        bool bUnoptimizedLumen = false;
        bool bAsyncIndirectLighting = true;
        bool bAsyncReflections = true;
        bool bAsyncSceneLighting = true;
        bool bLumenScreenTraces = true;
        bool bLumenReflectionsScreenSpaceReconstruction = true;
        bool bTranslucentSurfaceReflections = true;

        bool bVirtualShadowMaps = true;
        bool bVSMFarShadowCulling = false;
        bool bContactShadows = true;
        bool bDistanceFieldShadows = true;
        bool bCapsuleShadows = true;
        bool bCascadedShadows = true;

        bool bFogEnabled = true;
        bool bSkyAtmosphereEnabled = true;
        bool bGPUOcclusionQueries = true;
        bool bEnhancedAutoExposure = true;
        bool bEnableDLSSFG = true;
        bool bEnableReflex = true;
        bool bForceHighestMipOnUI = false;
        bool bFullyLoadUsedTextures = false;
        bool bTickAllowAsyncTickCleanup = true;
        bool bTickAllowAsyncTickDispatch = true;
        float LightPoolRelevancy = 1.0f;
        int32 MaximumActiveLights = 100;

        float LumenSceneLightingQuality = 1.0f;
        float LumenSceneDetail = 8.0f;
        float LumenSceneViewDistance = 20000.0f;
        float LumenSceneLightingUpdateSpeed = 0.5f;
        float LumenFinalGatherQuality = 1.0f;
        float LumenFinalGatherLightingUpdateSpeed = 0.5f;
        float LumenMaxTraceDistance = 20000.0f;
        float LumenDiffuseColorBoost = 1.5f;
        float LumenSkylightLeaking = 0.25f;
        float LumenFullSkylightLeakingDistance = 1000.0f;
        float LumenSurfaceCacheResolution = 1.0f;
        float LumenReflectionQuality = 1.0f;
        float IndirectLightingIntensity = 1.5f;
        FLinearColor IndirectLightingColor = FLinearColor::White;
        int32 LumenRayLightingMode = 0;
        int32 LumenFrontLayerTranslucencyReflections = 1;
        int32 LumenMaxReflectionBounces = 1;

        TMap<FString, FString> MeshRemap;
        TMap<FString, FString> MaterialRemap;
        TMap<FString, FString> MeshRemapNormalized;
        TMap<FString, FString> MaterialRemapNormalized;
    };

    extern FPPVConfig GPPVConfig;
    extern FCriticalSection GPPVConfigMutex;
    extern double GLastRuntimeConfigRefreshSeconds;
    extern bool bLoggedMissingCVarWarnings;
    extern double GStartupTimeSeconds;
    extern bool bLoggedCVarDelay;
    extern bool bPendingRuntimeApply;
    extern FTajsGraphSettingChangedDelegate GOnSettingChangedDelegate;

    bool ShouldGeneralLog();
    bool ShouldDebugLog();

    bool IsBuildableOwnedActor(const AActor* Owner);
    void ApplyNaniteOverrides(UStaticMeshComponent* Component, const FPPVConfig& Config, const TCHAR* LogContext);

    int32 GetSettingSchemaVersion();
    const TArray<FTajsGraphSettingDescriptor>& GetKnownSettingDescriptors();

    FString GetBundledSettingsConfigPath();
    FString GetPPVConfigPath();

    void LoadPersistentPPVConfig(FPPVConfig& Config);
    void ApplyPPVOverrides(FPostProcessSettings& Settings, const FPPVConfig& Config);
    void ApplyRuntimeAssetRemap(UStaticMeshComponent* Component, const FPPVConfig& Config);
    void RefreshRuntimeConfig(UWorld* World, bool bForce = false);

    UWorld* GetAnyGameWorld();

    bool SetConfigBoolValue(const FString& Section, const FString& Key, bool bValue, bool bApplyImmediately);
    bool SetConfigIntValue(const FString& Section, const FString& Key, int32 Value, bool bApplyImmediately);
    bool SetConfigFloatValue(const FString& Section, const FString& Key, float Value, bool bApplyImmediately);
    bool SetConfigStringValue(const FString& Section, const FString& Key, const FString& Value, bool bApplyImmediately);

    bool GetConfigBoolValue(const FString& Section, const FString& Key, bool& OutValue);
    bool GetConfigIntValue(const FString& Section, const FString& Key, int32& OutValue);
    bool GetConfigFloatValue(const FString& Section, const FString& Key, float& OutValue);
    bool GetConfigStringValue(const FString& Section, const FString& Key, FString& OutValue);

    void InstallPPVHooks();
    void InstallStaticMeshRemapHook();
    void InstallColoredInstanceMeshProxyHook();

    constexpr double GConfigRefreshIntervalSeconds = 1.0;
    constexpr double GCVarApplyDelaySeconds = 5.0;

}
