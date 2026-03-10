#include "TajsGraphSmlSettings.h"

#include "Configuration/ConfigManager.h"
#include "Configuration/ModConfiguration.h"
#include "Configuration/Properties/ConfigPropertyBool.h"
#include "Configuration/Properties/ConfigPropertyFloat.h"
#include "Configuration/Properties/ConfigPropertyInteger.h"
#include "Configuration/Properties/ConfigPropertySection.h"
#include "Configuration/Properties/ConfigPropertyString.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Misc/Paths.h"
#include "UObject/SoftObjectPtr.h"
#include "TajsGraphModule.h"

namespace
{
    constexpr TCHAR ConfigClassPath[] = TEXT("/TajsGraph/Config/TajsGraph_ModConfig.TajsGraph_ModConfig_C");
    TSubclassOf<UModConfiguration> LoadConfigClass();

    FConfigId MakeConfigId()
    {
        if (TSubclassOf<UModConfiguration> ConfigClass = LoadConfigClass())
        {
            if (const UModConfiguration* DefaultConfig = ConfigClass.GetDefaultObject())
            {
                return DefaultConfig->ConfigId;
            }
        }

        FConfigId FallbackConfigId;
        FallbackConfigId.ModReference = TEXT("TajsGraph");
        FallbackConfigId.ConfigCategory = TEXT("");
        return FallbackConfigId;
    }

    UGameInstance* GetAnyGameInstance()
    {
        if (!GEngine)
        {
            return nullptr;
        }

        for (const FWorldContext& Context : GEngine->GetWorldContexts())
        {
            UWorld* World = Context.World();
            if (!World || (Context.WorldType != EWorldType::Game && Context.WorldType != EWorldType::PIE))
            {
                continue;
            }

            if (UGameInstance* GameInstance = World->GetGameInstance())
            {
                return GameInstance;
            }
        }

        return nullptr;
    }

    TSubclassOf<UModConfiguration> LoadConfigClass()
    {
        static TSoftClassPtr<UModConfiguration> SoftClass{FSoftObjectPath(ConfigClassPath)};
        return SoftClass.LoadSynchronous();
    }

    UConfigManager* GetConfigManager(const UGameInstance* GameInstance)
    {
        if (const UGameInstance* Resolved = GameInstance ? GameInstance : GetAnyGameInstance())
        {
            return const_cast<UGameInstance*>(Resolved)->GetSubsystem<UConfigManager>();
        }
        return nullptr;
    }

    UConfigPropertySection* ResolveSection(UConfigPropertySection* RootSection, const FString& Section)
    {
        if (!RootSection)
        {
            return nullptr;
        }

        if (Section.IsEmpty())
        {
            return RootSection;
        }

        if (UConfigProperty* const* Property = RootSection->SectionProperties.Find(Section))
        {
            return Cast<UConfigPropertySection>(*Property);
        }

        return nullptr;
    }
}

namespace TajsGraphSmlSettings
{
    FString GetConfigFilePath()
    {
        return FPaths::Combine(UConfigManager::GetConfigurationFolderPath(), TEXT("TajsGraph.cfg"));
    }

    UConfigPropertySection* GetRootSection(const UGameInstance* GameInstance, bool bRegisterIfNeeded)
    {
        UConfigManager* ConfigManager = GetConfigManager(GameInstance);
        if (!ConfigManager)
        {
            if (bRegisterIfNeeded)
            {
                if (TSubclassOf<UModConfiguration> ConfigClass = LoadConfigClass())
                {
                    if (const UModConfiguration* DefaultConfig = ConfigClass.GetDefaultObject())
                    {
                        return DefaultConfig->RootSection;
                    }
                }
            }
            return nullptr;
        }

        const FConfigId ConfigId = MakeConfigId();
        UConfigPropertySection* RootSection = ConfigManager->GetConfigurationRootSection(ConfigId);
        if (!RootSection && bRegisterIfNeeded)
        {
            if (TSubclassOf<UModConfiguration> ConfigClass = LoadConfigClass())
            {
                ConfigManager->RegisterModConfiguration(ConfigClass);
                RootSection = ConfigManager->GetConfigurationRootSection(ConfigId);
            }
            else
            {
                UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph][Settings] Failed to load SML config class '%s'"), ConfigClassPath);
            }
        }

        return RootSection;
    }

    UConfigProperty* FindProperty(const UGameInstance* GameInstance, const FString& Section, const FString& Key, bool bRegisterIfNeeded)
    {
        UConfigPropertySection* RootSection = GetRootSection(GameInstance, bRegisterIfNeeded);
        UConfigPropertySection* SectionProperty = ResolveSection(RootSection, Section);
        if (!SectionProperty)
        {
            return nullptr;
        }

        if (UConfigProperty* const* Property = SectionProperty->SectionProperties.Find(Key))
        {
            return *Property;
        }

        return nullptr;
    }

    bool SetBool(const FString& Section, const FString& Key, bool bValue)
    {
        if (UConfigPropertyBool* Property = Cast<UConfigPropertyBool>(FindProperty(nullptr, Section, Key, true)))
        {
            if (Property->Value != bValue)
            {
                Property->Value = bValue;
                Property->MarkDirty();
            }
            return true;
        }
        return false;
    }

    bool SetInt(const FString& Section, const FString& Key, int32 Value)
    {
        if (UConfigPropertyInteger* Property = Cast<UConfigPropertyInteger>(FindProperty(nullptr, Section, Key, true)))
        {
            if (Property->Value != Value)
            {
                Property->Value = Value;
                Property->MarkDirty();
            }
            return true;
        }
        return false;
    }

    bool SetFloat(const FString& Section, const FString& Key, float Value)
    {
        if (UConfigPropertyFloat* Property = Cast<UConfigPropertyFloat>(FindProperty(nullptr, Section, Key, true)))
        {
            if (!FMath::IsNearlyEqual(Property->Value, Value))
            {
                Property->Value = Value;
                Property->MarkDirty();
            }
            return true;
        }
        return false;
    }

    bool SetString(const FString& Section, const FString& Key, const FString& Value)
    {
        if (UConfigPropertyString* Property = Cast<UConfigPropertyString>(FindProperty(nullptr, Section, Key, true)))
        {
            if (Property->Value != Value)
            {
                Property->Value = Value;
                Property->MarkDirty();
            }
            return true;
        }
        return false;
    }

    bool GetBool(const FString& Section, const FString& Key, bool& OutValue)
    {
        if (const UConfigPropertyBool* Property = Cast<UConfigPropertyBool>(FindProperty(nullptr, Section, Key, true)))
        {
            OutValue = Property->Value;
            return true;
        }
        return false;
    }

    bool GetInt(const FString& Section, const FString& Key, int32& OutValue)
    {
        if (const UConfigPropertyInteger* Property = Cast<UConfigPropertyInteger>(FindProperty(nullptr, Section, Key, true)))
        {
            OutValue = Property->Value;
            return true;
        }
        return false;
    }

    bool GetFloat(const FString& Section, const FString& Key, float& OutValue)
    {
        if (const UConfigPropertyFloat* Property = Cast<UConfigPropertyFloat>(FindProperty(nullptr, Section, Key, true)))
        {
            OutValue = Property->Value;
            return true;
        }
        return false;
    }

    bool GetString(const FString& Section, const FString& Key, FString& OutValue)
    {
        if (const UConfigPropertyString* Property = Cast<UConfigPropertyString>(FindProperty(nullptr, Section, Key, true)))
        {
            OutValue = Property->Value;
            return true;
        }
        return false;
    }
}





