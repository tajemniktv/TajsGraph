#pragma once
#include "CoreMinimal.h"
#include "Configuration/ConfigManager.h"
#include "Engine/Engine.h"
#include "BP_TajsGraph_ConfigStruct.generated.h"

struct FBP_TajsGraph_ConfigStruct_PoolLightSection;
struct FBP_TajsGraph_ConfigStruct_ShadowsSection;
struct FBP_TajsGraph_ConfigStruct_LumenSection;
struct FBP_TajsGraph_ConfigStruct_OtherSection;
struct FBP_TajsGraph_ConfigStruct_DevSection;
struct FBP_TajsGraph_ConfigStruct_GeneralRenderingSection;
struct FBP_TajsGraph_ConfigStruct_LumenSection_AsyncLumenSection;
struct FBP_TajsGraph_ConfigStruct_LumenSection_LumenReflectionsSection;

USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct_LumenSection_AsyncLumenSection {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool AsyncIndirectLighting{};

    UPROPERTY(BlueprintReadWrite)
    bool AsyncReflections{};

    UPROPERTY(BlueprintReadWrite)
    bool AsyncSceneLighting{};
};

USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct_LumenSection_LumenReflectionsSection {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool LumenScreenTraces{};

    UPROPERTY(BlueprintReadWrite)
    bool LumenReflectionsScreenSpaceReconstruction{};

    UPROPERTY(BlueprintReadWrite)
    float LumenReflectionSmoothing{};

    UPROPERTY(BlueprintReadWrite)
    bool TranslucentSurfaceReflections{};

    UPROPERTY(BlueprintReadWrite)
    bool ScreenSpaceReconstructionTonemap{};
};

USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct_PoolLightSection {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    float LightPoolRelevancy{};

    UPROPERTY(BlueprintReadWrite)
    int32 MaximumActiveLights{};
};

USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct_ShadowsSection {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool VirtualShadowMaps{};

    UPROPERTY(BlueprintReadWrite)
    bool VSMFarShadowCulling{};

    UPROPERTY(BlueprintReadWrite)
    bool ContactShadows{};

    UPROPERTY(BlueprintReadWrite)
    bool DFShadows{};

    UPROPERTY(BlueprintReadWrite)
    bool CapsuleShadows{};

    UPROPERTY(BlueprintReadWrite)
    bool CascadedShadows{};

    UPROPERTY(BlueprintReadWrite)
    bool HeightfieldShadows{};
};

USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct_LumenSection {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    FBP_TajsGraph_ConfigStruct_LumenSection_AsyncLumenSection AsyncLumenSection{};

    UPROPERTY(BlueprintReadWrite)
    bool Lumen{};

    UPROPERTY(BlueprintReadWrite)
    bool UnoptimizedLumen{};

    UPROPERTY(BlueprintReadWrite)
    int32 TraceDistanceSDF{};

    UPROPERTY(BlueprintReadWrite)
    int32 OctRes{};

    UPROPERTY(BlueprintReadWrite)
    int32 LumenScreenProbeGatherDownsampleFactor{};

    UPROPERTY(BlueprintReadWrite)
    int32 LumenScreenProbeGatherTemporalMaxFramesAccumulated{};

    UPROPERTY(BlueprintReadWrite)
    int32 LumenScreenProbeGatherRadianceCacheProbeResolution{};

    UPROPERTY(BlueprintReadWrite)
    int32 LumenSceneSurfaceCacheCardTexelDensityScale{};

    UPROPERTY(BlueprintReadWrite)
    int32 LumenSceneSurfaceCacheCompress{};

    UPROPERTY(BlueprintReadWrite)
    FBP_TajsGraph_ConfigStruct_LumenSection_LumenReflectionsSection LumenReflectionsSection{};
};

USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct_OtherSection {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool GPUOcclusionQueries{};

    UPROPERTY(BlueprintReadWrite)
    bool EnhancedAutoExposure{};

    UPROPERTY(BlueprintReadWrite)
    bool EnableDLSSFG{};

    UPROPERTY(BlueprintReadWrite)
    bool EnableReflex{};

    UPROPERTY(BlueprintReadWrite)
    bool tickAllowAsyncTickCleanup{};

    UPROPERTY(BlueprintReadWrite)
    bool tickAllowAsyncTickDispatch{};
};

USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct_DevSection {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool ForceHighestMipOnUI{};

    UPROPERTY(BlueprintReadWrite)
    bool FullyLoadUsedTextures{};

    UPROPERTY(BlueprintReadWrite)
    bool PLACEHOLDER{};

    UPROPERTY(BlueprintReadWrite)
    int32 ResetSurfaceCacheEveryNFrames{};
};

USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct_GeneralRenderingSection {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    float GrassDensity{};
};

/* Struct generated from Mod Configuration Asset '/TajsGraph/Config/BP_TajsGraph_Config' */
USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    FBP_TajsGraph_ConfigStruct_PoolLightSection PoolLightSection{};

    UPROPERTY(BlueprintReadWrite)
    FBP_TajsGraph_ConfigStruct_ShadowsSection ShadowsSection{};

    UPROPERTY(BlueprintReadWrite)
    FBP_TajsGraph_ConfigStruct_LumenSection LumenSection{};

    UPROPERTY(BlueprintReadWrite)
    FBP_TajsGraph_ConfigStruct_OtherSection OtherSection{};

    UPROPERTY(BlueprintReadWrite)
    FBP_TajsGraph_ConfigStruct_DevSection DevSection{};

    UPROPERTY(BlueprintReadWrite)
    FBP_TajsGraph_ConfigStruct_GeneralRenderingSection GeneralRenderingSection{};

    /* Retrieves active configuration value and returns object of this struct containing it */
    static FBP_TajsGraph_ConfigStruct GetActiveConfig(UObject* WorldContext) {
        FBP_TajsGraph_ConfigStruct ConfigStruct{};
        FConfigId ConfigId{"TajsGraph", ""};
        if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull)) {
            UConfigManager* ConfigManager = World->GetGameInstance()->GetSubsystem<UConfigManager>();
            ConfigManager->FillConfigurationStruct(ConfigId, FDynamicStructInfo{FBP_TajsGraph_ConfigStruct::StaticStruct(), &ConfigStruct});
        }
        return ConfigStruct;
    }
};

