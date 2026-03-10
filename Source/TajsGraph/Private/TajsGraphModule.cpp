#include "TajsGraphModule.h"
#include "TajsGraphModuleInternal.h"
#include "Interfaces/IPluginManager.h"
#include "Configuration/ConfigManager.h"

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

#if 0
    int32 GetSettingSchemaVersion()
    {
        return 2;
    }

    const TArray<FString> &GetKnownSettingSchemaKeys()
    {
        static const TArray<FString> Keys = {
            TEXT("Debug.EnableLogging"),
            TEXT("Debug.EnableDebugLogging"),
            TEXT("Debug.PollExternalConfigChanges"),
            TEXT("DebugHub.Enabled"),
            TEXT("DebugHub.OverlayEnabled"),
            TEXT("DebugHub.PerfSampleInterval"),
            TEXT("DebugHub.RenderSampleInterval"),
            TEXT("DebugHub.SceneProbeInterval"),
            TEXT("DebugHub.LastStateWriteInterval"),
            TEXT("DebugHub.HitchThresholdMs"),
            TEXT("DebugHub.HitchPerSignatureCooldownSec"),
            TEXT("DebugHub.HistorySecondsForSnapshot"),
            TEXT("DebugHub.MaxPerfSamples"),
            TEXT("DebugHub.MaxRenderSamples"),
            TEXT("DebugHub.MaxProbeSamples"),
            TEXT("DebugHub.MaxEvents"),
            TEXT("DebugHub.MaxCopiedHitchSnapshots"),
            TEXT("DebugHub.EnableGpuTiming"),
            TEXT("DebugHub.EnableToastNotifications"),
            TEXT("DebugHub.WriteExpandedReportArtifacts"),
            TEXT("DebugHub.HubWidgetClass"),
            TEXT("Visualization.EnableHotkeys"),
            TEXT("Visualization.EnableEngineViewAttempt"),
            TEXT("Visualization.EnableRuntimeVisCVars"),
            TEXT("Visualization.EnablePostProcessFallback"),
            TEXT("Visualization.ModeSwitchCooldownMs"),
            TEXT("PPV.Enabled"),
            TEXT("PPV.OverrideLumenMethods"),
            TEXT("PPV.OverrideLumenValues"),
            TEXT("PPV.OverrideLumenRayLightingMode"),
            TEXT("PPV.OverrideLumenFrontLayerTranslucencyReflections"),
            TEXT("PPV.OverrideLumenMaxReflectionBounces"),
            TEXT("PPV.OverrideIndirectLightingColor"),
            TEXT("PPV.OverrideIndirectLightingIntensity"),
            TEXT("PPV.LumenSceneLightingQuality"),
            TEXT("PPV.LumenSceneDetail"),
            TEXT("PPV.LumenSceneViewDistance"),
            TEXT("PPV.LumenSceneLightingUpdateSpeed"),
            TEXT("PPV.LumenFinalGatherQuality"),
            TEXT("PPV.LumenFinalGatherLightingUpdateSpeed"),
            TEXT("PPV.LumenMaxTraceDistance"),
            TEXT("PPV.LumenDiffuseColorBoost"),
            TEXT("PPV.LumenSkylightLeaking"),
            TEXT("PPV.LumenFullSkylightLeakingDistance"),
            TEXT("PPV.LumenSurfaceCacheResolution"),
            TEXT("PPV.LumenReflectionQuality"),
            TEXT("PPV.IndirectLightingIntensity"),
            TEXT("PPV.IndirectLightingColor"),
            TEXT("PPV.LumenRayLightingMode"),
            TEXT("PPV.LumenFrontLayerTranslucencyReflections"),
            TEXT("PPV.LumenMaxReflectionBounces"),
            TEXT("Instancing.ForceInstancing"),
            TEXT("Instancing.ForceLumenInstancing"),
            TEXT("Nanite.ForceNanite"),
            TEXT("Nanite.ForceNaniteForMasked"),
            TEXT("Foliage.FixBuildableFoliage"),
            TEXT("Lumen.Lumen"),
            TEXT("Lumen.UnoptimizedLumen"),
            TEXT("Lumen.AsyncIndirectLighting"),
            TEXT("Lumen.AsyncReflections"),
            TEXT("Lumen.AsyncSceneLighting"),
            TEXT("Lumen.LumenScreenTraces"),
            TEXT("Lumen.LumenReflectionsScreenSpaceReconstruction"),
            TEXT("Lumen.TranslucentSurfaceReflections"),
            TEXT("Shadows.VirtualShadowMaps"),
            TEXT("Shadows.VSMFarShadowCulling"),
            TEXT("Shadows.CapsuleShadows"),
            TEXT("Shadows.CascadedShadows"),
            TEXT("Shadows.ContactShadows"),
            TEXT("Shadows.DistanceFieldShadows"),
            TEXT("Rendering.FogEnabled"),
            TEXT("Rendering.SkyAtmosphereEnabled"),
            TEXT("Rendering.GPUOcclusionQueries"),
            TEXT("Rendering.EnhancedAutoExposure"),
            TEXT("Rendering.EnableDLSSFG"),
            TEXT("Rendering.EnableReflex"),
            TEXT("Rendering.ForceHighestMipOnUI"),
            TEXT("Rendering.FullyLoadUsedTextures"),
            TEXT("Rendering.TickAllowAsyncTickCleanup"),
            TEXT("Rendering.TickAllowAsyncTickDispatch"),
            TEXT("Remap.EnableAssetRemap")};
        return Keys;
    }

    const TArray<FTajsGraphSettingDescriptor> &GetKnownSettingDescriptors()
    {
        static const TArray<FTajsGraphSettingDescriptor> Descriptors = []()
        {
            const FPPVConfig Defaults;
            const FString BundledConfigPath = GetBundledSettingsConfigPath();
            const TArray<FString> &Keys = GetKnownSettingSchemaKeys();

            auto InferType = [](const FString &Section, const FString &Key)
            {
                if (Section == TEXT("PPV") && Key == TEXT("IndirectLightingColor"))
                {
                    return ETajsGraphSettingType::Color;
                }
                if (Section == TEXT("DebugHub") && Key == TEXT("HubWidgetClass"))
                {
                    return ETajsGraphSettingType::String;
                }
                if (Section == TEXT("DebugHub") && (Key == TEXT("PerfSampleInterval") ||
                                                    Key == TEXT("RenderSampleInterval") ||
                                                    Key == TEXT("SceneProbeInterval") ||
                                                    Key == TEXT("LastStateWriteInterval") ||
                                                    Key == TEXT("HitchThresholdMs") ||
                                                    Key == TEXT("HitchPerSignatureCooldownSec")))
                {
                    return ETajsGraphSettingType::Float;
                }
                if ((Section == TEXT("DebugHub") && (Key == TEXT("HistorySecondsForSnapshot") ||
                                                     Key == TEXT("MaxPerfSamples") ||
                                                     Key == TEXT("MaxRenderSamples") ||
                                                     Key == TEXT("MaxProbeSamples") ||
                                                     Key == TEXT("MaxEvents") ||
                                                     Key == TEXT("MaxCopiedHitchSnapshots"))) ||
                    (Section == TEXT("Visualization") && Key == TEXT("ModeSwitchCooldownMs")))
                {
                    return ETajsGraphSettingType::Int;
                }
                if (Section == TEXT("PPV") && (Key == TEXT("LumenRayLightingMode") || Key == TEXT("LumenFrontLayerTranslucencyReflections") || Key == TEXT("LumenMaxReflectionBounces")))
                {
                    return ETajsGraphSettingType::Int;
                }
                if (Section == TEXT("PPV") && (Key == TEXT("LumenSceneLightingQuality") ||
                                               Key == TEXT("LumenSceneDetail") ||
                                               Key == TEXT("LumenSceneViewDistance") ||
                                               Key == TEXT("LumenSceneLightingUpdateSpeed") ||
                                               Key == TEXT("LumenFinalGatherQuality") ||
                                               Key == TEXT("LumenFinalGatherLightingUpdateSpeed") ||
                                               Key == TEXT("LumenMaxTraceDistance") ||
                                               Key == TEXT("LumenDiffuseColorBoost") ||
                                               Key == TEXT("LumenSkylightLeaking") ||
                                               Key == TEXT("LumenFullSkylightLeakingDistance") ||
                                               Key == TEXT("LumenSurfaceCacheResolution") ||
                                               Key == TEXT("LumenReflectionQuality") ||
                                               Key == TEXT("IndirectLightingIntensity")))
                {
                    return ETajsGraphSettingType::Float;
                }
                return ETajsGraphSettingType::Bool;
            };

            auto InferDefaultValue = [&Defaults, &BundledConfigPath](const FString &Section, const FString &Key, const ETajsGraphSettingType Type)
            {
                if (!BundledConfigPath.IsEmpty() && GConfig)
                {
                    FString ConfigValue;
                    if (GConfig->GetString(*Section, *Key, ConfigValue, *BundledConfigPath))
                    {
                        return ConfigValue;
                    }
                }

                if (Section == TEXT("Debug") && Key == TEXT("EnableLogging"))
                    return Defaults.bEnableLogging ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Debug") && Key == TEXT("EnableDebugLogging"))
                    return Defaults.bEnableDebugLogging ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Debug") && Key == TEXT("PollExternalConfigChanges"))
                    return Defaults.bPollExternalConfigChanges ? FString(TEXT("true")) : FString(TEXT("false"));

                if (Section == TEXT("DebugHub") && Key == TEXT("Enabled"))
                    return FString(TEXT("false"));
                if (Section == TEXT("DebugHub") && Key == TEXT("OverlayEnabled"))
                    return FString(TEXT("false"));
                if (Section == TEXT("DebugHub") && Key == TEXT("PerfSampleInterval"))
                    return FString(TEXT("0.0"));
                if (Section == TEXT("DebugHub") && Key == TEXT("RenderSampleInterval"))
                    return FString(TEXT("0.2"));
                if (Section == TEXT("DebugHub") && Key == TEXT("SceneProbeInterval"))
                    return FString(TEXT("1.0"));
                if (Section == TEXT("DebugHub") && Key == TEXT("LastStateWriteInterval"))
                    return FString(TEXT("3.0"));
                if (Section == TEXT("DebugHub") && Key == TEXT("HitchThresholdMs"))
                    return FString(TEXT("60.0"));
                if (Section == TEXT("DebugHub") && Key == TEXT("HitchPerSignatureCooldownSec"))
                    return FString(TEXT("30.0"));
                if (Section == TEXT("DebugHub") && Key == TEXT("HistorySecondsForSnapshot"))
                    return FString(TEXT("10"));
                if (Section == TEXT("DebugHub") && Key == TEXT("MaxPerfSamples"))
                    return FString(TEXT("18000"));
                if (Section == TEXT("DebugHub") && Key == TEXT("MaxRenderSamples"))
                    return FString(TEXT("3000"));
                if (Section == TEXT("DebugHub") && Key == TEXT("MaxProbeSamples"))
                    return FString(TEXT("1200"));
                if (Section == TEXT("DebugHub") && Key == TEXT("MaxEvents"))
                    return FString(TEXT("1024"));
                if (Section == TEXT("DebugHub") && Key == TEXT("MaxCopiedHitchSnapshots"))
                    return FString(TEXT("5"));
                if (Section == TEXT("DebugHub") && Key == TEXT("EnableGpuTiming"))
                    return FString(TEXT("true"));
                if (Section == TEXT("DebugHub") && Key == TEXT("EnableToastNotifications"))
                    return FString(TEXT("true"));
                if (Section == TEXT("DebugHub") && Key == TEXT("WriteExpandedReportArtifacts"))
                    return FString(TEXT("false"));
                if (Section == TEXT("DebugHub") && Key == TEXT("HubWidgetClass"))
                    return FString();

                if (Section == TEXT("Visualization") && Key == TEXT("EnableHotkeys"))
                    return FString(TEXT("true"));
                if (Section == TEXT("Visualization") && Key == TEXT("EnableEngineViewAttempt"))
                    return FString(TEXT("true"));
                if (Section == TEXT("Visualization") && Key == TEXT("EnableRuntimeVisCVars"))
                    return FString(TEXT("true"));
                if (Section == TEXT("Visualization") && Key == TEXT("EnablePostProcessFallback"))
                    return FString(TEXT("true"));
                if (Section == TEXT("Visualization") && Key == TEXT("ModeSwitchCooldownMs"))
                    return FString(TEXT("100"));

                if (Section == TEXT("PPV") && Key == TEXT("Enabled"))
                    return Defaults.bEnabled ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("PPV") && Key == TEXT("OverrideLumenMethods"))
                    return Defaults.bOverrideLumenMethods ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("PPV") && Key == TEXT("OverrideLumenValues"))
                    return Defaults.bOverrideLumenValues ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("PPV") && Key == TEXT("OverrideLumenRayLightingMode"))
                    return Defaults.bOverrideLumenRayLightingMode ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("PPV") && Key == TEXT("OverrideLumenFrontLayerTranslucencyReflections"))
                    return Defaults.bOverrideLumenFrontLayerTranslucencyReflections ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("PPV") && Key == TEXT("OverrideLumenMaxReflectionBounces"))
                    return Defaults.bOverrideLumenMaxReflectionBounces ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("PPV") && Key == TEXT("OverrideIndirectLightingColor"))
                    return Defaults.bOverrideIndirectLightingColor ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("PPV") && Key == TEXT("OverrideIndirectLightingIntensity"))
                    return Defaults.bOverrideIndirectLightingIntensity ? FString(TEXT("true")) : FString(TEXT("false"));

                if (Section == TEXT("PPV") && Key == TEXT("LumenSceneLightingQuality"))
                    return FString::SanitizeFloat(Defaults.LumenSceneLightingQuality);
                if (Section == TEXT("PPV") && Key == TEXT("LumenSceneDetail"))
                    return FString::SanitizeFloat(Defaults.LumenSceneDetail);
                if (Section == TEXT("PPV") && Key == TEXT("LumenSceneViewDistance"))
                    return FString::SanitizeFloat(Defaults.LumenSceneViewDistance);
                if (Section == TEXT("PPV") && Key == TEXT("LumenSceneLightingUpdateSpeed"))
                    return FString::SanitizeFloat(Defaults.LumenSceneLightingUpdateSpeed);
                if (Section == TEXT("PPV") && Key == TEXT("LumenFinalGatherQuality"))
                    return FString::SanitizeFloat(Defaults.LumenFinalGatherQuality);
                if (Section == TEXT("PPV") && Key == TEXT("LumenFinalGatherLightingUpdateSpeed"))
                    return FString::SanitizeFloat(Defaults.LumenFinalGatherLightingUpdateSpeed);
                if (Section == TEXT("PPV") && Key == TEXT("LumenMaxTraceDistance"))
                    return FString::SanitizeFloat(Defaults.LumenMaxTraceDistance);
                if (Section == TEXT("PPV") && Key == TEXT("LumenDiffuseColorBoost"))
                    return FString::SanitizeFloat(Defaults.LumenDiffuseColorBoost);
                if (Section == TEXT("PPV") && Key == TEXT("LumenSkylightLeaking"))
                    return FString::SanitizeFloat(Defaults.LumenSkylightLeaking);
                if (Section == TEXT("PPV") && Key == TEXT("LumenFullSkylightLeakingDistance"))
                    return FString::SanitizeFloat(Defaults.LumenFullSkylightLeakingDistance);
                if (Section == TEXT("PPV") && Key == TEXT("LumenSurfaceCacheResolution"))
                    return FString::SanitizeFloat(Defaults.LumenSurfaceCacheResolution);
                if (Section == TEXT("PPV") && Key == TEXT("LumenReflectionQuality"))
                    return FString::SanitizeFloat(Defaults.LumenReflectionQuality);
                if (Section == TEXT("PPV") && Key == TEXT("IndirectLightingIntensity"))
                    return FString::SanitizeFloat(Defaults.IndirectLightingIntensity);
                if (Section == TEXT("PPV") && Key == TEXT("IndirectLightingColor"))
                    return Defaults.IndirectLightingColor.ToString();

                if (Section == TEXT("PPV") && Key == TEXT("LumenRayLightingMode"))
                    return FString::FromInt(Defaults.LumenRayLightingMode);
                if (Section == TEXT("PPV") && Key == TEXT("LumenFrontLayerTranslucencyReflections"))
                    return FString::FromInt(Defaults.LumenFrontLayerTranslucencyReflections);
                if (Section == TEXT("PPV") && Key == TEXT("LumenMaxReflectionBounces"))
                    return FString::FromInt(Defaults.LumenMaxReflectionBounces);

                if (Section == TEXT("Instancing") && Key == TEXT("ForceInstancing"))
                    return Defaults.bForceInstancing ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Instancing") && Key == TEXT("ForceLumenInstancing"))
                    return Defaults.bForceLumenInstancing ? FString(TEXT("true")) : FString(TEXT("false"));

                if (Section == TEXT("Nanite") && Key == TEXT("ForceNanite"))
                    return Defaults.bForceNanite ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Nanite") && Key == TEXT("ForceNaniteForMasked"))
                    return Defaults.bForceNaniteForMasked ? FString(TEXT("true")) : FString(TEXT("false"));

                if (Section == TEXT("Foliage") && Key == TEXT("FixBuildableFoliage"))
                    return Defaults.bFixBuildableFoliage ? FString(TEXT("true")) : FString(TEXT("false"));

                if (Section == TEXT("Lumen") && Key == TEXT("Lumen"))
                    return Defaults.bLumen ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Lumen") && Key == TEXT("UnoptimizedLumen"))
                    return Defaults.bUnoptimizedLumen ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Lumen") && Key == TEXT("AsyncIndirectLighting"))
                    return Defaults.bAsyncIndirectLighting ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Lumen") && Key == TEXT("AsyncReflections"))
                    return Defaults.bAsyncReflections ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Lumen") && Key == TEXT("AsyncSceneLighting"))
                    return Defaults.bAsyncSceneLighting ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Lumen") && Key == TEXT("LumenScreenTraces"))
                    return Defaults.bLumenScreenTraces ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Lumen") && Key == TEXT("LumenReflectionsScreenSpaceReconstruction"))
                    return Defaults.bLumenReflectionsScreenSpaceReconstruction ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Lumen") && Key == TEXT("TranslucentSurfaceReflections"))
                    return Defaults.bTranslucentSurfaceReflections ? FString(TEXT("true")) : FString(TEXT("false"));

                if (Section == TEXT("Shadows") && Key == TEXT("VirtualShadowMaps"))
                    return Defaults.bVirtualShadowMaps ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Shadows") && Key == TEXT("VSMFarShadowCulling"))
                    return Defaults.bVSMFarShadowCulling ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Shadows") && Key == TEXT("CapsuleShadows"))
                    return Defaults.bCapsuleShadows ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Shadows") && Key == TEXT("CascadedShadows"))
                    return Defaults.bCascadedShadows ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Shadows") && Key == TEXT("ContactShadows"))
                    return Defaults.bContactShadows ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Shadows") && Key == TEXT("DistanceFieldShadows"))
                    return Defaults.bDistanceFieldShadows ? FString(TEXT("true")) : FString(TEXT("false"));

                if (Section == TEXT("Rendering") && Key == TEXT("FogEnabled"))
                    return Defaults.bFogEnabled ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Rendering") && Key == TEXT("SkyAtmosphereEnabled"))
                    return Defaults.bSkyAtmosphereEnabled ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Rendering") && Key == TEXT("GPUOcclusionQueries"))
                    return Defaults.bGPUOcclusionQueries ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Rendering") && Key == TEXT("EnhancedAutoExposure"))
                    return Defaults.bEnhancedAutoExposure ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Rendering") && Key == TEXT("EnableDLSSFG"))
                    return Defaults.bEnableDLSSFG ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Rendering") && Key == TEXT("EnableReflex"))
                    return Defaults.bEnableReflex ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Rendering") && Key == TEXT("ForceHighestMipOnUI"))
                    return Defaults.bForceHighestMipOnUI ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Rendering") && Key == TEXT("FullyLoadUsedTextures"))
                    return Defaults.bFullyLoadUsedTextures ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Rendering") && Key == TEXT("TickAllowAsyncTickCleanup"))
                    return Defaults.bTickAllowAsyncTickCleanup ? FString(TEXT("true")) : FString(TEXT("false"));
                if (Section == TEXT("Rendering") && Key == TEXT("TickAllowAsyncTickDispatch"))
                    return Defaults.bTickAllowAsyncTickDispatch ? FString(TEXT("true")) : FString(TEXT("false"));

                if (Section == TEXT("Remap") && Key == TEXT("EnableAssetRemap"))
                    return Defaults.bEnableAssetRemap ? FString(TEXT("true")) : FString(TEXT("false"));

                if (Type == ETajsGraphSettingType::Bool)
                {
                    return FString(TEXT("false"));
                }
                return FString();
            };

            auto InferBounds = [](const FString &Section, const FString &Key, float &OutMin, float &OutMax, float &OutStep)
            {
                OutMin = 0.0f;
                OutMax = 0.0f;
                OutStep = 0.0f;

                if (Section == TEXT("DebugHub"))
                {
                    if (Key == TEXT("PerfSampleInterval") || Key == TEXT("RenderSampleInterval") || Key == TEXT("SceneProbeInterval") || Key == TEXT("LastStateWriteInterval"))
                    {
                        OutMin = 0.0f;
                        OutMax = 30.0f;
                        OutStep = 0.05f;
                        return;
                    }
                    if (Key == TEXT("HitchThresholdMs"))
                    {
                        OutMin = 1.0f;
                        OutMax = 1000.0f;
                        OutStep = 1.0f;
                        return;
                    }
                    if (Key == TEXT("HitchPerSignatureCooldownSec"))
                    {
                        OutMin = 0.0f;
                        OutMax = 600.0f;
                        OutStep = 1.0f;
                        return;
                    }
                    if (Key == TEXT("HistorySecondsForSnapshot"))
                    {
                        OutMin = 1.0f;
                        OutMax = 300.0f;
                        OutStep = 1.0f;
                        return;
                    }
                    if (Key == TEXT("MaxPerfSamples"))
                    {
                        OutMin = 60.0f;
                        OutMax = 200000.0f;
                        OutStep = 60.0f;
                        return;
                    }
                    if (Key == TEXT("MaxRenderSamples"))
                    {
                        OutMin = 60.0f;
                        OutMax = 50000.0f;
                        OutStep = 60.0f;
                        return;
                    }
                    if (Key == TEXT("MaxProbeSamples"))
                    {
                        OutMin = 30.0f;
                        OutMax = 10000.0f;
                        OutStep = 30.0f;
                        return;
                    }
                    if (Key == TEXT("MaxEvents"))
                    {
                        OutMin = 64.0f;
                        OutMax = 20000.0f;
                        OutStep = 32.0f;
                        return;
                    }
                    if (Key == TEXT("MaxCopiedHitchSnapshots"))
                    {
                        OutMin = 0.0f;
                        OutMax = 50.0f;
                        OutStep = 1.0f;
                        return;
                    }
                }

                if (Section == TEXT("Visualization") && Key == TEXT("ModeSwitchCooldownMs"))
                {
                    OutMin = 0.0f;
                    OutMax = 5000.0f;
                    OutStep = 10.0f;
                    return;
                }

                if (Section != TEXT("PPV"))
                {
                    return;
                }

                if (Key == TEXT("LumenSceneLightingQuality") || Key == TEXT("LumenFinalGatherQuality") || Key == TEXT("LumenReflectionQuality"))
                {
                    OutMin = 0.0f;
                    OutMax = 8.0f;
                    OutStep = 0.1f;
                    return;
                }
                if (Key == TEXT("LumenSceneDetail"))
                {
                    OutMin = 0.0f;
                    OutMax = 32.0f;
                    OutStep = 0.1f;
                    return;
                }
                if (Key == TEXT("LumenSceneViewDistance") || Key == TEXT("LumenMaxTraceDistance"))
                {
                    OutMin = 100.0f;
                    OutMax = 1000000.0f;
                    OutStep = 100.0f;
                    return;
                }
                if (Key == TEXT("LumenSceneLightingUpdateSpeed") || Key == TEXT("LumenFinalGatherLightingUpdateSpeed"))
                {
                    OutMin = 0.01f;
                    OutMax = 8.0f;
                    OutStep = 0.01f;
                    return;
                }
                if (Key == TEXT("LumenDiffuseColorBoost"))
                {
                    OutMin = 0.0f;
                    OutMax = 8.0f;
                    OutStep = 0.01f;
                    return;
                }
                if (Key == TEXT("LumenSkylightLeaking"))
                {
                    OutMin = 0.0f;
                    OutMax = 1.0f;
                    OutStep = 0.01f;
                    return;
                }
                if (Key == TEXT("LumenFullSkylightLeakingDistance"))
                {
                    OutMin = 0.0f;
                    OutMax = 200000.0f;
                    OutStep = 10.0f;
                    return;
                }
                if (Key == TEXT("LumenSurfaceCacheResolution"))
                {
                    OutMin = 0.25f;
                    OutMax = 4.0f;
                    OutStep = 0.01f;
                    return;
                }
                if (Key == TEXT("IndirectLightingIntensity"))
                {
                    OutMin = 0.0f;
                    OutMax = 20.0f;
                    OutStep = 0.01f;
                    return;
                }

                if (Key == TEXT("LumenRayLightingMode") || Key == TEXT("LumenFrontLayerTranslucencyReflections"))
                {
                    OutMin = 0.0f;
                    OutMax = 1.0f;
                    OutStep = 1.0f;
                    return;
                }
                if (Key == TEXT("LumenMaxReflectionBounces"))
                {
                    OutMin = 1.0f;
                    OutMax = 64.0f;
                    OutStep = 1.0f;
                    return;
                }
            };

            auto InferApplyPolicy = [](const FString &Section, const FString &Key)
            {
                return ETajsGraphApplyPolicy::Live;
            };

            auto IsDangerous = [](const FString &Section, const FString &Key)
            {
                return Section == TEXT("Debug") || Section == TEXT("DebugHub") || Section == TEXT("Visualization") || Key.Contains(TEXT("Async")) || Key == TEXT("UnoptimizedLumen") || Key == TEXT("EnableDLSSFG") || Key == TEXT("EnableReflex");
            };

            TArray<FTajsGraphSettingDescriptor> Out;
            Out.Reserve(Keys.Num());

            for (const FString &Id : Keys)
            {
                FString Section;
                FString Key;
                if (!Id.Split(TEXT("."), &Section, &Key, ESearchCase::CaseSensitive))
                {
                    continue;
                }

                FTajsGraphSettingDescriptor Descriptor;
                Descriptor.Id = Id;
                Descriptor.Section = Section;
                Descriptor.Key = Key;
                Descriptor.Category = Section;
                Descriptor.Subcategory = (Section == TEXT("PPV") && Key.StartsWith(TEXT("Override"))) ? TEXT("Overrides") : TEXT("General");
                Descriptor.Type = InferType(Section, Key);
                Descriptor.DefaultValue = InferDefaultValue(Section, Key, Descriptor.Type);
                InferBounds(Section, Key, Descriptor.MinValue, Descriptor.MaxValue, Descriptor.Step);
                Descriptor.ApplyPolicy = InferApplyPolicy(Section, Key);
                Descriptor.bDangerous = IsDangerous(Section, Key);
                Descriptor.Tooltip = FString::Printf(TEXT("%s.%s"), *Section, *Key);

                Out.Add(MoveTemp(Descriptor));
            }

            return Out;
        }();

        return Descriptors;
    }

#endif
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




