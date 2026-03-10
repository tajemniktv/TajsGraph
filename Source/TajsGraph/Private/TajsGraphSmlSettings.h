#pragma once

#include "CoreMinimal.h"

class UConfigProperty;
class UConfigPropertySection;
class UGameInstance;
class UModConfiguration;

namespace TajsGraphSmlSettings
{
    FString GetConfigFilePath();

    bool SetBool(const FString& Section, const FString& Key, bool bValue);
    bool SetInt(const FString& Section, const FString& Key, int32 Value);
    bool SetFloat(const FString& Section, const FString& Key, float Value);
    bool SetString(const FString& Section, const FString& Key, const FString& Value);

    bool GetBool(const FString& Section, const FString& Key, bool& OutValue);
    bool GetInt(const FString& Section, const FString& Key, int32& OutValue);
    bool GetFloat(const FString& Section, const FString& Key, float& OutValue);
    bool GetString(const FString& Section, const FString& Key, FString& OutValue);

    UConfigPropertySection* GetRootSection(const UGameInstance* GameInstance, bool bRegisterIfNeeded);
    UConfigProperty* FindProperty(const UGameInstance* GameInstance, const FString& Section, const FString& Key, bool bRegisterIfNeeded);
}
