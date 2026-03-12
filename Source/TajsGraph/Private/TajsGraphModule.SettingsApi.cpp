#include "TajsGraphModuleInternal.h"

using namespace TajsGraphInternal;

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

TArray<FTajsGraphSettingDescriptor> FTajsGraphModule::GetSettingDescriptors() {
	return GetKnownSettingDescriptors();
}

FTajsGraphSettingChangedDelegate& FTajsGraphModule::OnSettingChanged() {
	return GOnSettingChangedDelegate;
}
