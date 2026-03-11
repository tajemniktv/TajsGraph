#pragma once
#include "CoreMinimal.h"
#include "Configuration/ConfigManager.h"
#include "Engine/Engine.h"
#include "TajsGraph_ModConfigStruct.generated.h"

struct FTajsGraph_ModConfigStruct_Debug;
struct FTajsGraph_ModConfigStruct_DebugHub;
struct FTajsGraph_ModConfigStruct_Foliage;
struct FTajsGraph_ModConfigStruct_Instancing;
struct FTajsGraph_ModConfigStruct_Lumen;
struct FTajsGraph_ModConfigStruct_Nanite;
struct FTajsGraph_ModConfigStruct_Remap;
struct FTajsGraph_ModConfigStruct_Visualization;
struct FTajsGraph_ModConfigStruct_Shadows;
struct FTajsGraph_ModConfigStruct_Rendering;
struct FTajsGraph_ModConfigStruct_PPV;

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_Debug {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool EnableLogging{};

    UPROPERTY(BlueprintReadWrite)
    bool EnableDebugLogging{};

    UPROPERTY(BlueprintReadWrite)
    bool PollExternalConfigChanges{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_DebugHub {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool Enabled{};

    UPROPERTY(BlueprintReadWrite)
    bool OverlayEnabled{};

    UPROPERTY(BlueprintReadWrite)
    float PerfSampleInterval{};

    UPROPERTY(BlueprintReadWrite)
    float RenderSampleInterval{};

    UPROPERTY(BlueprintReadWrite)
    float SceneProbeInterval{};

    UPROPERTY(BlueprintReadWrite)
    float LastStateWriteInterval{};

    UPROPERTY(BlueprintReadWrite)
    float HitchThresholdMs{};

    UPROPERTY(BlueprintReadWrite)
    float HitchPerSignatureCooldownSec{};

    UPROPERTY(BlueprintReadWrite)
    int32 HistorySecondsForSnapshot{};

    UPROPERTY(BlueprintReadWrite)
    int32 MaxPerfSamples{};

    UPROPERTY(BlueprintReadWrite)
    int32 MaxRenderSamples{};

    UPROPERTY(BlueprintReadWrite)
    int32 MaxProbeSamples{};

    UPROPERTY(BlueprintReadWrite)
    int32 MaxEvents{};

    UPROPERTY(BlueprintReadWrite)
    int32 MaxCopiedHitchSnapshots{};

    UPROPERTY(BlueprintReadWrite)
    bool EnableGpuTiming{};

    UPROPERTY(BlueprintReadWrite)
    bool EnableToastNotifications{};

    UPROPERTY(BlueprintReadWrite)
    bool WriteExpandedReportArtifacts{};

    UPROPERTY(BlueprintReadWrite)
    FString HubWidgetClass{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_Foliage {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool FixBuildableFoliage{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_Instancing {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool ForceInstancing{};

    UPROPERTY(BlueprintReadWrite)
    bool ForceLumenInstancing{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_Lumen {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool AsyncIndirectLighting{};

    UPROPERTY(BlueprintReadWrite)
    bool AsyncReflections{};

    UPROPERTY(BlueprintReadWrite)
    bool AsyncSceneLighting{};

    UPROPERTY(BlueprintReadWrite)
    bool Lumen{};

    UPROPERTY(BlueprintReadWrite)
    bool LumenReflectionsScreenSpaceReconstruction{};

    UPROPERTY(BlueprintReadWrite)
    bool LumenScreenTraces{};

    UPROPERTY(BlueprintReadWrite)
    bool TranslucentSurfaceReflections{};

    UPROPERTY(BlueprintReadWrite)
    bool UnoptimizedLumen{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_Nanite {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool ForceNanite{};

    UPROPERTY(BlueprintReadWrite)
    bool ForceNaniteForMasked{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_Remap {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool EnableAssetRemap{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_Visualization {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool EnableEngineViewAttempt{};

    UPROPERTY(BlueprintReadWrite)
    bool EnableHotkeys{};

    UPROPERTY(BlueprintReadWrite)
    bool EnablePostProcessFallback{};

    UPROPERTY(BlueprintReadWrite)
    bool EnableRuntimeVisCVars{};

    UPROPERTY(BlueprintReadWrite)
    int32 ModeSwitchCooldownMs{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_Shadows {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool CapsuleShadows{};

    UPROPERTY(BlueprintReadWrite)
    bool CascadedShadows{};

    UPROPERTY(BlueprintReadWrite)
    bool ContactShadows{};

    UPROPERTY(BlueprintReadWrite)
    bool DistanceFieldShadows{};

    UPROPERTY(BlueprintReadWrite)
    bool VirtualShadowMaps{};

    UPROPERTY(BlueprintReadWrite)
    bool VSMFarShadowCulling{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_Rendering {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool EnableDLSSFG{};

    UPROPERTY(BlueprintReadWrite)
    bool EnableReflex{};

    UPROPERTY(BlueprintReadWrite)
    bool EnhancedAutoExposure{};

    UPROPERTY(BlueprintReadWrite)
    bool FogEnabled{};

    UPROPERTY(BlueprintReadWrite)
    bool ForceHighestMipOnUI{};

    UPROPERTY(BlueprintReadWrite)
    bool FullyLoadUsedTextures{};

    UPROPERTY(BlueprintReadWrite)
    bool GPUOcclusionQueries{};

    UPROPERTY(BlueprintReadWrite)
    bool SkyAtmosphereEnabled{};

    UPROPERTY(BlueprintReadWrite)
    bool TickAllowAsyncTickCleanup{};

    UPROPERTY(BlueprintReadWrite)
    bool TickAllowAsyncTickDispatch{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct_PPV {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool Enabled{};

    UPROPERTY(BlueprintReadWrite)
    FString IndirectLightingColor{};

    UPROPERTY(BlueprintReadWrite)
    float IndirectLightingIntensity{};

    UPROPERTY(BlueprintReadWrite)
    float LumenDiffuseColorBoost{};

    UPROPERTY(BlueprintReadWrite)
    float LumenFinalGatherLightingUpdateSpeed{};

    UPROPERTY(BlueprintReadWrite)
    float LumenFinalGatherQuality{};

    UPROPERTY(BlueprintReadWrite)
    int32 LumenFrontLayerTranslucencyReflections{};

    UPROPERTY(BlueprintReadWrite)
    float LumenFullSkylightLeakingDistance{};

    UPROPERTY(BlueprintReadWrite)
    int32 LumenMaxReflectionBounces{};

    UPROPERTY(BlueprintReadWrite)
    float LumenMaxTraceDistance{};

    UPROPERTY(BlueprintReadWrite)
    int32 LumenRayLightingMode{};

    UPROPERTY(BlueprintReadWrite)
    float LumenReflectionQuality{};

    UPROPERTY(BlueprintReadWrite)
    float LumenSceneDetail{};

    UPROPERTY(BlueprintReadWrite)
    float LumenSceneLightingQuality{};

    UPROPERTY(BlueprintReadWrite)
    float LumenSceneLightingUpdateSpeed{};

    UPROPERTY(BlueprintReadWrite)
    float LumenSceneViewDistance{};

    UPROPERTY(BlueprintReadWrite)
    float LumenSkylightLeaking{};

    UPROPERTY(BlueprintReadWrite)
    float LumenSurfaceCacheResolution{};

    UPROPERTY(BlueprintReadWrite)
    bool OverrideIndirectLightingColor{};

    UPROPERTY(BlueprintReadWrite)
    bool OverrideIndirectLightingIntensity{};

    UPROPERTY(BlueprintReadWrite)
    bool OverrideLumenFrontLayerTranslucencyReflections{};

    UPROPERTY(BlueprintReadWrite)
    bool OverrideLumenMaxReflectionBounces{};

    UPROPERTY(BlueprintReadWrite)
    bool OverrideLumenMethods{};

    UPROPERTY(BlueprintReadWrite)
    bool OverrideLumenRayLightingMode{};

    UPROPERTY(BlueprintReadWrite)
    bool OverrideLumenValues{};
};

/* Struct generated from Mod Configuration Asset '/TajsGraph/Config/TajsGraph_ModConfig' */
USTRUCT(BlueprintType)
struct FTajsGraph_ModConfigStruct {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_Debug Debug{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_DebugHub DebugHub{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_Foliage Foliage{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_Instancing Instancing{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_Lumen Lumen{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_Nanite Nanite{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_Remap Remap{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_Visualization Visualization{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_Shadows Shadows{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_Rendering Rendering{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ModConfigStruct_PPV PPV{};

    /* Retrieves active configuration value and returns object of this struct containing it */
    static FTajsGraph_ModConfigStruct GetActiveConfig(UObject* WorldContext) {
        FTajsGraph_ModConfigStruct ConfigStruct{};
        FConfigId ConfigId{"", ""};
        if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull)) {
            UConfigManager* ConfigManager = World->GetGameInstance()->GetSubsystem<UConfigManager>();
            ConfigManager->FillConfigurationStruct(ConfigId, FDynamicStructInfo{FTajsGraph_ModConfigStruct::StaticStruct(), &ConfigStruct});
        }
        return ConfigStruct;
    }
};

