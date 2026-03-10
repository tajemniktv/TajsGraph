#include "TajsGraphModuleInternal.h"
#include "TajsGraphSmlSettings.h"

using namespace TajsGraphInternal;

bool FTajsGraphModule::SetBoolSetting(const FString& Section, const FString& Key, bool bValue, bool bApplyImmediately) {
	return SetConfigBoolValue(Section, Key, bValue, bApplyImmediately);
}

bool FTajsGraphModule::SetIntSetting(const FString& Section, const FString& Key, int32 Value, bool bApplyImmediately) {
	return SetConfigIntValue(Section, Key, Value, bApplyImmediately);
}

bool FTajsGraphModule::SetFloatSetting(const FString& Section, const FString& Key, float Value, bool bApplyImmediately) {
	return SetConfigFloatValue(Section, Key, Value, bApplyImmediately);
}

bool FTajsGraphModule::SetStringSetting(const FString& Section, const FString& Key, const FString& Value, bool bApplyImmediately) {
	return SetConfigStringValue(Section, Key, Value, bApplyImmediately);
}

bool FTajsGraphModule::GetBoolSetting(const FString& Section, const FString& Key, bool& OutValue) {
	return GetConfigBoolValue(Section, Key, OutValue);
}

bool FTajsGraphModule::GetIntSetting(const FString& Section, const FString& Key, int32& OutValue) {
	return GetConfigIntValue(Section, Key, OutValue);
}

bool FTajsGraphModule::GetFloatSetting(const FString& Section, const FString& Key, float& OutValue) {
	return GetConfigFloatValue(Section, Key, OutValue);
}

bool FTajsGraphModule::GetStringSetting(const FString& Section, const FString& Key, FString& OutValue) {
	return GetConfigStringValue(Section, Key, OutValue);
}

bool FTajsGraphModule::ForceApplySettings() {
	RefreshRuntimeConfig(GetAnyGameWorld(), true);
	return true;
}

FString FTajsGraphModule::GetSettingsConfigPath() {
	return TajsGraphSmlSettings::GetConfigFilePath();
}

int32 FTajsGraphModule::GetSettingsSchemaVersion() {
	return GetSettingSchemaVersion();
}

TArray<FString> FTajsGraphModule::GetSettingSchemaKeys() {
	return GetKnownSettingSchemaKeys();
}

TArray<FTajsGraphSettingDescriptor> FTajsGraphModule::GetSettingDescriptors() {
	return GetKnownSettingDescriptors();
}

FTajsGraphSettingChangedDelegate& FTajsGraphModule::OnSettingChanged() {
	return GOnSettingChangedDelegate;
}
