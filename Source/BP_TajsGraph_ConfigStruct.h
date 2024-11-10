#pragma once
#include "CoreMinimal.h"
#include "Configuration/ConfigManager.h"
#include "Engine/Engine.h"
#include "BP_TajsGraph_ConfigStruct.generated.h"

/* Struct generated from Mod Configuration Asset '/TajsGraph/Config/BP_TajsGraph_Config' */
USTRUCT(BlueprintType)
struct FBP_TajsGraph_ConfigStruct {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool enableVSM{};

    UPROPERTY(BlueprintReadWrite)
    bool enableLumen{};

    UPROPERTY(BlueprintReadWrite)
    bool enableAsyncLumenDI{};

    UPROPERTY(BlueprintReadWrite)
    bool enableAsyncLumenReflections{};

    UPROPERTY(BlueprintReadWrite)
    bool enableAsyncLumenLighting{};

    UPROPERTY(BlueprintReadWrite)
    bool enableVSMfarshadowculling{};

    UPROPERTY(BlueprintReadWrite)
    bool enableScreenTraces{};

    UPROPERTY(BlueprintReadWrite)
    bool enableHardwareRayTracing{};

    UPROPERTY(BlueprintReadWrite)
    bool enableVRS{};

    UPROPERTY(BlueprintReadWrite)
    bool enableMeshStreaming{};

    UPROPERTY(BlueprintReadWrite)
    bool enableTransFrontLayer{};

    UPROPERTY(BlueprintReadWrite)
    float reflectionSmoothness{};

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

