#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "TajsGraphSettingsSchema.h"
// Log category used by TajsGraph (matches SML blueprint log category naming)
DECLARE_LOG_CATEGORY_EXTERN(LogTajsGraph, Verbose, All);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FTajsGraphSettingChangedDelegate, const FString& /*Section*/, const FString& /*Key*/, const FString& /*Value*/);

class FTajsGraphModule : public FDefaultGameModuleImpl {
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static bool ForceApplySettings();
    static FString GetSettingsConfigPath();
    static int32 GetSettingsSchemaVersion();
    static TArray<FTajsGraphSettingDescriptor> GetSettingDescriptors();
    static FTajsGraphSettingChangedDelegate& OnSettingChanged();

private:
    void InstallBuildableFoliageHook();
};
