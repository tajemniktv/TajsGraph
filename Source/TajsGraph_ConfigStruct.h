#pragma once
#include "CoreMinimal.h"
#include "Configuration/ConfigManager.h"
#include "Engine/Engine.h"
#include "TajsGraph_ConfigStruct.generated.h"

/* Struct generated from Mod Configuration Asset '/TajsGraph/TajsGraph_Config' */
USTRUCT(BlueprintType)
struct FTajsGraph_ConfigStruct {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool enableVSM{};

    UPROPERTY(BlueprintReadWrite)
    bool enableLumen{};

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

