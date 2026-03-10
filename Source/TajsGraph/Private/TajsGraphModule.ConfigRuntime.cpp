#include "TajsGraphModuleInternal.h"
#include "TajsGraphSmlSettings.h"

namespace TajsGraphInternal {
namespace PrivateConfigRuntime {

#include "TajsGraphModule.ConfigRuntime.inl"

}

FString GetPPVConfigPath() {
    return PrivateConfigRuntime::GetPPVConfigPath();
}

void LoadPersistentPPVConfig(FPPVConfig& Config) {
    PrivateConfigRuntime::LoadPersistentPPVConfig(Config);
}

void ApplyPPVOverrides(FPostProcessSettings& Settings, const FPPVConfig& Config) {
    PrivateConfigRuntime::ApplyPPVOverrides(Settings, Config);
}

void ApplyRuntimeAssetRemap(UStaticMeshComponent* Component, const FPPVConfig& Config) {
    PrivateConfigRuntime::ApplyRuntimeAssetRemap(Component, Config);
}

void RefreshRuntimeConfig(UWorld* World, bool bForce) {
    PrivateConfigRuntime::RefreshRuntimeConfig(World, bForce);
}

UWorld* GetAnyGameWorld() {
    return PrivateConfigRuntime::GetAnyGameWorld();
}

bool SetConfigBoolValue(const FString& Section, const FString& Key, bool bValue, bool bApplyImmediately) {
    return PrivateConfigRuntime::SetConfigBoolValue(Section, Key, bValue, bApplyImmediately);
}

bool SetConfigIntValue(const FString& Section, const FString& Key, int32 Value, bool bApplyImmediately) {
    return PrivateConfigRuntime::SetConfigIntValue(Section, Key, Value, bApplyImmediately);
}

bool SetConfigFloatValue(const FString& Section, const FString& Key, float Value, bool bApplyImmediately) {
    return PrivateConfigRuntime::SetConfigFloatValue(Section, Key, Value, bApplyImmediately);
}

bool SetConfigStringValue(const FString& Section, const FString& Key, const FString& Value, bool bApplyImmediately) {
    return PrivateConfigRuntime::SetConfigStringValue(Section, Key, Value, bApplyImmediately);
}

bool GetConfigBoolValue(const FString& Section, const FString& Key, bool& OutValue) {
    return PrivateConfigRuntime::GetConfigBoolValue(Section, Key, OutValue);
}

bool GetConfigIntValue(const FString& Section, const FString& Key, int32& OutValue) {
    return PrivateConfigRuntime::GetConfigIntValue(Section, Key, OutValue);
}

bool GetConfigFloatValue(const FString& Section, const FString& Key, float& OutValue) {
    return PrivateConfigRuntime::GetConfigFloatValue(Section, Key, OutValue);
}

bool GetConfigStringValue(const FString& Section, const FString& Key, FString& OutValue) {
    return PrivateConfigRuntime::GetConfigStringValue(Section, Key, OutValue);
}

}

