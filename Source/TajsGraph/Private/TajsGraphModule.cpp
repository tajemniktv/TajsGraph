#include "TajsGraphModule.h"
#include "TajsGraphModuleInternal.h"
#include "Interfaces/IPluginManager.h"

// Define the TajsGraph log category
DEFINE_LOG_CATEGORY(LogTajsGraph);

namespace TajsGraphInternal
{

    FPPVConfig GPPVConfig;
    FCriticalSection GPPVConfigMutex;
    double GLastRuntimeConfigRefreshSeconds = 0.0;
    bool bLoggedMissingCVarWarnings = false;
    double GStartupTimeSeconds = 0.0;
    bool bLoggedCVarDelay = false;
    bool bPendingRuntimeApply = true;
    FTajsGraphSettingChangedDelegate GOnSettingChangedDelegate;

    bool ShouldGeneralLog()
    {
        FScopeLock Lock(&GPPVConfigMutex);
        return GPPVConfig.bEnableLogging || GPPVConfig.bEnableDebugLogging;
    }

    bool ShouldDebugLog()
    {
        FScopeLock Lock(&GPPVConfigMutex);
        return GPPVConfig.bEnableDebugLogging;
    }

    FString GetBundledSettingsConfigPath()
    {
        const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("TajsGraph"));
        if (Plugin.IsValid())
        {
            const FString CanonicalSettingsPath = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Config"), TEXT("TajsGraphSettings.ini"));
            if (FPaths::FileExists(CanonicalSettingsPath))
            {
                return CanonicalSettingsPath;
            }

            return FPaths::Combine(Plugin->GetBaseDir(), TEXT("Config"), TEXT("PluginSettings.ini"));
        }

        const FString ProjectCanonicalSettingsPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Mods"), TEXT("TajsGraph"), TEXT("Config"), TEXT("TajsGraphSettings.ini"));
        if (FPaths::FileExists(ProjectCanonicalSettingsPath))
        {
            return ProjectCanonicalSettingsPath;
        }

        return FPaths::Combine(FPaths::ProjectDir(), TEXT("Mods"), TEXT("TajsGraph"), TEXT("Config"), TEXT("PluginSettings.ini"));
    }

}




