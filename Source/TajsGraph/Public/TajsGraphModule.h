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

    // Realtime settings backend API for future custom UI menu.
    static bool SetBoolSetting(const FString& Section, const FString& Key, bool bValue, bool bApplyImmediately = true);
    static bool SetIntSetting(const FString& Section, const FString& Key, int32 Value, bool bApplyImmediately = true);
    static bool SetFloatSetting(const FString& Section, const FString& Key, float Value, bool bApplyImmediately = true);
    static bool SetStringSetting(const FString& Section, const FString& Key, const FString& Value, bool bApplyImmediately = true);

    static bool GetBoolSetting(const FString& Section, const FString& Key, bool& OutValue);
    static bool GetIntSetting(const FString& Section, const FString& Key, int32& OutValue);
    static bool GetFloatSetting(const FString& Section, const FString& Key, float& OutValue);
    static bool GetStringSetting(const FString& Section, const FString& Key, FString& OutValue);

    static bool ForceApplySettings();
    static FString GetSettingsConfigPath();
    static int32 GetSettingsSchemaVersion();
    static TArray<FString> GetSettingSchemaKeys();
    static TArray<FTajsGraphSettingDescriptor> GetSettingDescriptors();
    static FTajsGraphSettingChangedDelegate& OnSettingChanged();

private:
    void InstallBuildableFoliageHook();
};
