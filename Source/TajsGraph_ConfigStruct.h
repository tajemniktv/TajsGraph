#pragma once
#include "CoreMinimal.h"
#include "Configuration/ConfigManager.h"
#include "Engine/Engine.h"
#include "TajsGraph_ConfigStruct.generated.h"

struct FTajsGraph_ConfigStruct_Shadows;
struct FTajsGraph_ConfigStruct_Lumen;

USTRUCT(BlueprintType)
struct FTajsGraph_ConfigStruct_Shadows {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool enableVSM{};
};

USTRUCT(BlueprintType)
struct FTajsGraph_ConfigStruct_Lumen {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool enableLUMEN{};
};

/* Struct generated from Mod Configuration Asset '/TajsGraph/Settings/TajsGraph_Config' */
USTRUCT(BlueprintType)
struct FTajsGraph_ConfigStruct {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ConfigStruct_Shadows Shadows{};

    UPROPERTY(BlueprintReadWrite)
    FTajsGraph_ConfigStruct_Lumen Lumen{};

    /* Retrieves active configuration value and returns object of this struct containing it */
    static FTajsGraph_ConfigStruct GetActiveConfig(UObject* WorldContext) {
        FTajsGraph_ConfigStruct ConfigStruct{};
        FConfigId ConfigId{"TajsGraph", ""};
        if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull)) {
            UConfigManager* ConfigManager = World->GetGameInstance()->GetSubsystem<UConfigManager>();
            ConfigManager->FillConfigurationStruct(ConfigId, FDynamicStructInfo{FTajsGraph_ConfigStruct::StaticStruct(), &ConfigStruct});
        }
        return ConfigStruct;
    }
};

