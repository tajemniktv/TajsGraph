#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ConfigManager.h"
#include "TajsGraphSmlSettings.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTajsGraph_Persistence_SaveAndReadBack, "TajsGraph.Persistence.SaveAndReadBack",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FTajsGraph_Persistence_SaveAndReadBack::RunTest(const FString &Parameters)
{
    // Use a unique key so tests don't collide with real settings.
    const FString TestKey = FString::Printf(TEXT("Test_Persist_%u"), FPlatformTime::Cycles());
    const FString TestValue = TEXT("42");

    // Set in-memory and persist
    FConfigManager::Get().SetValue(TestKey, TestValue);
    const bool bSaved = FConfigManager::Get().SaveToIni(FString());
    TestTrue(TEXT("SaveToIni returned success"), bSaved);

    // Try to read back using GConfig first
    FString ReadBack;
    bool bFound = false;
    if (GConfig && GConfig->GetString(TEXT("TajsGraph"), *TestKey, ReadBack, GGameIni))
    {
        bFound = true;
        TestEqual(TEXT("Persisted value via GConfig matches"), ReadBack, TestValue);
    }

#if WITH_SML
    if (!bFound)
    {
        // Fall back to SML adapter read
        if (TajsGraphSmlSettings::GetConfigValue(TEXT("TajsGraph"), TestKey, ReadBack))
        {
            bFound = true;
            TestEqual(TEXT("Persisted value via SML adapter matches"), ReadBack, TestValue);
        }
    }
#endif

    TestTrue(TEXT("Persisted key was found via GConfig or SML"), bFound);
    return true;
}
