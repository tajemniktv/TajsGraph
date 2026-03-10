#include "TajsGraphRuntimeBlueprintLibrary.h"

#include "Debug/TajsGraphDebugSubsystem.h"
#include "Debug/TajsGraphVisualizationSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "TajsGraphModule.h"

namespace
{
    const FName ManagedPpvTag(TEXT("TajsGraph.ManagedPPV"));

    static FString StripWrappingQuotes(FString Value)
    {
        Value = Value.TrimStartAndEnd();
        bool bChanged = true;
        while (bChanged && Value.Len() >= 2)
        {
            bChanged = false;
            const TCHAR First = Value[0];
            const TCHAR Last = Value[Value.Len() - 1];
            if ((First == TEXT('"') && Last == TEXT('"')) || (First == TEXT('\'') && Last == TEXT('\'')))
            {
                Value = Value.Mid(1, Value.Len() - 2).TrimStartAndEnd();
                bChanged = true;
            }
        }
        return Value;
    }

    static bool TryParseConfigColor(const FString& RawValue, FLinearColor& OutColor)
    {
        FString Normalized = StripWrappingQuotes(RawValue);
        if (Normalized.StartsWith(TEXT("(")) && Normalized.EndsWith(TEXT(")")) && Normalized.Len() >= 2)
        {
            Normalized = Normalized.Mid(1, Normalized.Len() - 2).TrimStartAndEnd();
        }
        return OutColor.InitFromString(Normalized);
    }

    static APostProcessVolume* FindManagedPPV(UWorld* World)
    {
        if (!World)
        {
            return nullptr;
        }

        for (TActorIterator<APostProcessVolume> It(World); It; ++It)
        {
            if (It->Tags.Contains(ManagedPpvTag))
            {
                return *It;
            }
        }
        return nullptr;
    }

    static APostProcessVolume* FindOrSpawnManagedPPV(UWorld* World)
    {
        if (!World)
        {
            return nullptr;
        }

        if (APostProcessVolume* Existing = FindManagedPPV(World))
        {
            return Existing;
        }

        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("TajsGraph_ManagedPPV");
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.ObjectFlags |= RF_Transient;

        APostProcessVolume* Volume = World->SpawnActor<APostProcessVolume>(APostProcessVolume::StaticClass(), FTransform::Identity, SpawnParams);
        if (Volume)
        {
            Volume->Tags.AddUnique(ManagedPpvTag);
            Volume->bUnbound = true;
            Volume->Priority = 5000.0f;
            Volume->BlendWeight = 1.0f;
            Volume->SetActorHiddenInGame(true);
        }
        return Volume;
    }
}

void UTajsGraphRuntimeBlueprintLibrary::RefreshAllFromModConfig(const UObject* WorldContextObject, const bool bForceRuntimeApply)
{
    if (WorldContextObject)
    {
        if (const UWorld* World = WorldContextObject->GetWorld())
        {
            if (UGameInstance* GameInstance = World->GetGameInstance())
            {
                if (UTajsGraphDebugSubsystem* DebugSubsystem = GameInstance->GetSubsystem<UTajsGraphDebugSubsystem>())
                {
                    DebugSubsystem->ReloadConfigFromSettings();
                }

                if (UTajsGraphVisualizationSubsystem* VisualizationSubsystem = GameInstance->GetSubsystem<UTajsGraphVisualizationSubsystem>())
                {
                    VisualizationSubsystem->ReloadVisualizationConfig();
                }
            }
        }
    }

    if (bForceRuntimeApply)
    {
        FTajsGraphModule::ForceApplySettings();
    }
}

void UTajsGraphRuntimeBlueprintLibrary::ApplyManagedPPVFromModConfig(const UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        return;
    }

    APostProcessVolume* Volume = FindOrSpawnManagedPPV(World);
    if (!Volume)
    {
        return;
    }

    bool bEnabled = true;
    FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("Enabled"), bEnabled);

    FPostProcessSettings Settings;
    if (!bEnabled)
    {
        Volume->Settings = Settings;
        Volume->BlendWeight = 0.0f;
        return;
    }

    Volume->bUnbound = true;
    Volume->Priority = 5000.0f;
    Volume->BlendWeight = 1.0f;

    bool bOverrideLumenMethods = true;
    bool bOverrideLumenValues = true;
    bool bOverrideLumenRayLightingMode = true;
    bool bOverrideLumenFrontLayerTranslucencyReflections = true;
    bool bOverrideLumenMaxReflectionBounces = true;
    bool bOverrideIndirectLightingColor = false;
    bool bOverrideIndirectLightingIntensity = true;

    FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenMethods"), bOverrideLumenMethods);
    FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenValues"), bOverrideLumenValues);
    FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenRayLightingMode"), bOverrideLumenRayLightingMode);
    FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenFrontLayerTranslucencyReflections"), bOverrideLumenFrontLayerTranslucencyReflections);
    FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenMaxReflectionBounces"), bOverrideLumenMaxReflectionBounces);
    FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideIndirectLightingColor"), bOverrideIndirectLightingColor);
    FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideIndirectLightingIntensity"), bOverrideIndirectLightingIntensity);

    if (bOverrideLumenMethods)
    {
        Settings.bOverride_DynamicGlobalIlluminationMethod = true;
        Settings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Lumen;
        Settings.bOverride_ReflectionMethod = true;
        Settings.ReflectionMethod = EReflectionMethod::Lumen;
    }

    if (bOverrideLumenValues)
    {
        float FloatValue = 0.0f;

        Settings.bOverride_LumenSceneLightingQuality = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSceneLightingQuality"), FloatValue);
        Settings.LumenSceneLightingQuality = FloatValue;

        Settings.bOverride_LumenSceneDetail = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSceneDetail"), FloatValue);
        Settings.LumenSceneDetail = FloatValue;

        Settings.bOverride_LumenSceneViewDistance = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSceneViewDistance"), FloatValue);
        Settings.LumenSceneViewDistance = FloatValue;

        Settings.bOverride_LumenSceneLightingUpdateSpeed = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSceneLightingUpdateSpeed"), FloatValue);
        Settings.LumenSceneLightingUpdateSpeed = FloatValue;

        Settings.bOverride_LumenFinalGatherQuality = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenFinalGatherQuality"), FloatValue);
        Settings.LumenFinalGatherQuality = FloatValue;

        Settings.bOverride_LumenFinalGatherLightingUpdateSpeed = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenFinalGatherLightingUpdateSpeed"), FloatValue);
        Settings.LumenFinalGatherLightingUpdateSpeed = FloatValue;

        Settings.bOverride_LumenMaxTraceDistance = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenMaxTraceDistance"), FloatValue);
        Settings.LumenMaxTraceDistance = FloatValue;

        Settings.bOverride_LumenDiffuseColorBoost = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenDiffuseColorBoost"), FloatValue);
        Settings.LumenDiffuseColorBoost = FloatValue;

        Settings.bOverride_LumenSkylightLeaking = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSkylightLeaking"), FloatValue);
        Settings.LumenSkylightLeaking = FloatValue;

        Settings.bOverride_LumenFullSkylightLeakingDistance = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenFullSkylightLeakingDistance"), FloatValue);
        Settings.LumenFullSkylightLeakingDistance = FloatValue;

        Settings.bOverride_LumenSurfaceCacheResolution = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSurfaceCacheResolution"), FloatValue);
        Settings.LumenSurfaceCacheResolution = FloatValue;

        Settings.bOverride_LumenReflectionQuality = true;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenReflectionQuality"), FloatValue);
        Settings.LumenReflectionQuality = FloatValue;
    }

    if (bOverrideIndirectLightingColor)
    {
        FString ColorValue;
        if (FTajsGraphModule::GetStringSetting(TEXT("PPV"), TEXT("IndirectLightingColor"), ColorValue))
        {
            FLinearColor ParsedColor = FLinearColor::White;
            if (TryParseConfigColor(ColorValue, ParsedColor))
            {
                Settings.bOverride_IndirectLightingColor = true;
                Settings.IndirectLightingColor = ParsedColor;
            }
        }
    }

    if (bOverrideIndirectLightingIntensity)
    {
        float IntensityValue = 1.0f;
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("IndirectLightingIntensity"), IntensityValue);
        Settings.bOverride_IndirectLightingIntensity = true;
        Settings.IndirectLightingIntensity = IntensityValue;
    }

    if (bOverrideLumenRayLightingMode)
    {
        int32 IntValue = 0;
        FTajsGraphModule::GetIntSetting(TEXT("PPV"), TEXT("LumenRayLightingMode"), IntValue);
        Settings.bOverride_LumenRayLightingMode = true;
        Settings.LumenRayLightingMode = static_cast<ELumenRayLightingModeOverride>(IntValue);
    }

    if (bOverrideLumenFrontLayerTranslucencyReflections)
    {
        int32 IntValue = 0;
        FTajsGraphModule::GetIntSetting(TEXT("PPV"), TEXT("LumenFrontLayerTranslucencyReflections"), IntValue);
        Settings.bOverride_LumenFrontLayerTranslucencyReflections = true;
        Settings.LumenFrontLayerTranslucencyReflections = IntValue;
    }

    if (bOverrideLumenMaxReflectionBounces)
    {
        int32 IntValue = 1;
        FTajsGraphModule::GetIntSetting(TEXT("PPV"), TEXT("LumenMaxReflectionBounces"), IntValue);
        Settings.bOverride_LumenMaxReflectionBounces = true;
        Settings.LumenMaxReflectionBounces = IntValue;
    }

    Volume->Settings = Settings;
}
