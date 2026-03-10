    static FString StripWrappingQuotes(FString Value) {
        Value = Value.TrimStartAndEnd();
        bool bChanged = true;
        while (bChanged && Value.Len() >= 2) {
            bChanged = false;
            const TCHAR First = Value[0];
            const TCHAR Last = Value[Value.Len() - 1];
            if ((First == TEXT('"') && Last == TEXT('"')) || (First == TEXT('\'') && Last == TEXT('\''))) {
                Value = Value.Mid(1, Value.Len() - 2).TrimStartAndEnd();
                bChanged = true;
            }
        }
        return Value;
    }

    static FString NormalizeRemapPath(const FString& RawPath) {
        FString Result = StripWrappingQuotes(RawPath);

        const int32 FirstQuote = Result.Find(TEXT("'"), ESearchCase::CaseSensitive, ESearchDir::FromStart);
        const int32 LastQuote = Result.Find(TEXT("'"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
        if (FirstQuote != INDEX_NONE && LastQuote != INDEX_NONE && LastQuote > FirstQuote) {
            Result = Result.Mid(FirstQuote + 1, LastQuote - FirstQuote - 1).TrimStartAndEnd();
        }

        return StripWrappingQuotes(Result);
    }

    enum class ESettingValueType : uint8 {
        Bool,
        Int,
        Float,
        String
    };

    struct FSettingKeySpec {
        const TCHAR* Section;
        const TCHAR* Key;
        ESettingValueType Type;
    };

    static const TArray<FSettingKeySpec>& GetSettingKeySpecs() {
        static const TArray<FSettingKeySpec> Specs = {
            { TEXT("Debug"), TEXT("EnableLogging"), ESettingValueType::Bool },
            { TEXT("Debug"), TEXT("EnableDebugLogging"), ESettingValueType::Bool },
            { TEXT("Debug"), TEXT("PollExternalConfigChanges"), ESettingValueType::Bool },

            { TEXT("DebugHub"), TEXT("Enabled"), ESettingValueType::Bool },
            { TEXT("DebugHub"), TEXT("OverlayEnabled"), ESettingValueType::Bool },
            { TEXT("DebugHub"), TEXT("PerfSampleInterval"), ESettingValueType::Float },
            { TEXT("DebugHub"), TEXT("RenderSampleInterval"), ESettingValueType::Float },
            { TEXT("DebugHub"), TEXT("SceneProbeInterval"), ESettingValueType::Float },
            { TEXT("DebugHub"), TEXT("LastStateWriteInterval"), ESettingValueType::Float },
            { TEXT("DebugHub"), TEXT("HitchThresholdMs"), ESettingValueType::Float },
            { TEXT("DebugHub"), TEXT("HitchPerSignatureCooldownSec"), ESettingValueType::Float },
            { TEXT("DebugHub"), TEXT("HistorySecondsForSnapshot"), ESettingValueType::Int },
            { TEXT("DebugHub"), TEXT("MaxPerfSamples"), ESettingValueType::Int },
            { TEXT("DebugHub"), TEXT("MaxRenderSamples"), ESettingValueType::Int },
            { TEXT("DebugHub"), TEXT("MaxProbeSamples"), ESettingValueType::Int },
            { TEXT("DebugHub"), TEXT("MaxEvents"), ESettingValueType::Int },
            { TEXT("DebugHub"), TEXT("MaxCopiedHitchSnapshots"), ESettingValueType::Int },
            { TEXT("DebugHub"), TEXT("EnableGpuTiming"), ESettingValueType::Bool },
            { TEXT("DebugHub"), TEXT("EnableToastNotifications"), ESettingValueType::Bool },
            { TEXT("DebugHub"), TEXT("WriteExpandedReportArtifacts"), ESettingValueType::Bool },
            { TEXT("DebugHub"), TEXT("HubWidgetClass"), ESettingValueType::String },

            { TEXT("Visualization"), TEXT("EnableHotkeys"), ESettingValueType::Bool },
            { TEXT("Visualization"), TEXT("EnableEngineViewAttempt"), ESettingValueType::Bool },
            { TEXT("Visualization"), TEXT("EnableRuntimeVisCVars"), ESettingValueType::Bool },
            { TEXT("Visualization"), TEXT("EnablePostProcessFallback"), ESettingValueType::Bool },
            { TEXT("Visualization"), TEXT("ModeSwitchCooldownMs"), ESettingValueType::Int },

            { TEXT("PPV"), TEXT("Enabled"), ESettingValueType::Bool },
            { TEXT("PPV"), TEXT("OverrideLumenMethods"), ESettingValueType::Bool },
            { TEXT("PPV"), TEXT("OverrideLumenValues"), ESettingValueType::Bool },
            { TEXT("PPV"), TEXT("OverrideLumenRayLightingMode"), ESettingValueType::Bool },
            { TEXT("PPV"), TEXT("OverrideLumenFrontLayerTranslucencyReflections"), ESettingValueType::Bool },
            { TEXT("PPV"), TEXT("OverrideLumenMaxReflectionBounces"), ESettingValueType::Bool },
            { TEXT("PPV"), TEXT("OverrideIndirectLightingColor"), ESettingValueType::Bool },
            { TEXT("PPV"), TEXT("OverrideIndirectLightingIntensity"), ESettingValueType::Bool },

            { TEXT("PPV"), TEXT("LumenSceneLightingQuality"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenSceneDetail"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenSceneViewDistance"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenSceneLightingUpdateSpeed"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenFinalGatherQuality"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenFinalGatherLightingUpdateSpeed"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenMaxTraceDistance"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenDiffuseColorBoost"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenSkylightLeaking"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenFullSkylightLeakingDistance"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenSurfaceCacheResolution"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("LumenReflectionQuality"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("IndirectLightingIntensity"), ESettingValueType::Float },
            { TEXT("PPV"), TEXT("IndirectLightingColor"), ESettingValueType::String },

            { TEXT("PPV"), TEXT("LumenRayLightingMode"), ESettingValueType::Int },
            { TEXT("PPV"), TEXT("LumenFrontLayerTranslucencyReflections"), ESettingValueType::Int },
            { TEXT("PPV"), TEXT("LumenMaxReflectionBounces"), ESettingValueType::Int },

            { TEXT("Instancing"), TEXT("ForceInstancing"), ESettingValueType::Bool },
            { TEXT("Instancing"), TEXT("ForceLumenInstancing"), ESettingValueType::Bool },

            { TEXT("Nanite"), TEXT("ForceNanite"), ESettingValueType::Bool },
            { TEXT("Nanite"), TEXT("ForceNaniteForMasked"), ESettingValueType::Bool },

            { TEXT("Foliage"), TEXT("FixBuildableFoliage"), ESettingValueType::Bool },

            { TEXT("Lumen"), TEXT("Lumen"), ESettingValueType::Bool },
            { TEXT("Lumen"), TEXT("UnoptimizedLumen"), ESettingValueType::Bool },
            { TEXT("Lumen"), TEXT("AsyncIndirectLighting"), ESettingValueType::Bool },
            { TEXT("Lumen"), TEXT("AsyncReflections"), ESettingValueType::Bool },
            { TEXT("Lumen"), TEXT("AsyncSceneLighting"), ESettingValueType::Bool },
            { TEXT("Lumen"), TEXT("LumenScreenTraces"), ESettingValueType::Bool },
            { TEXT("Lumen"), TEXT("LumenReflectionsScreenSpaceReconstruction"), ESettingValueType::Bool },
            { TEXT("Lumen"), TEXT("TranslucentSurfaceReflections"), ESettingValueType::Bool },

            { TEXT("Shadows"), TEXT("VirtualShadowMaps"), ESettingValueType::Bool },
            { TEXT("Shadows"), TEXT("VSMFarShadowCulling"), ESettingValueType::Bool },
            { TEXT("Shadows"), TEXT("CapsuleShadows"), ESettingValueType::Bool },
            { TEXT("Shadows"), TEXT("CascadedShadows"), ESettingValueType::Bool },
            { TEXT("Shadows"), TEXT("ContactShadows"), ESettingValueType::Bool },
            { TEXT("Shadows"), TEXT("DistanceFieldShadows"), ESettingValueType::Bool },

            { TEXT("Rendering"), TEXT("FogEnabled"), ESettingValueType::Bool },
            { TEXT("Rendering"), TEXT("SkyAtmosphereEnabled"), ESettingValueType::Bool },
            { TEXT("Rendering"), TEXT("GPUOcclusionQueries"), ESettingValueType::Bool },
            { TEXT("Rendering"), TEXT("EnhancedAutoExposure"), ESettingValueType::Bool },
            { TEXT("Rendering"), TEXT("EnableDLSSFG"), ESettingValueType::Bool },
            { TEXT("Rendering"), TEXT("EnableReflex"), ESettingValueType::Bool },
            { TEXT("Rendering"), TEXT("ForceHighestMipOnUI"), ESettingValueType::Bool },
            { TEXT("Rendering"), TEXT("FullyLoadUsedTextures"), ESettingValueType::Bool },
            { TEXT("Rendering"), TEXT("TickAllowAsyncTickCleanup"), ESettingValueType::Bool },
            { TEXT("Rendering"), TEXT("TickAllowAsyncTickDispatch"), ESettingValueType::Bool },

            { TEXT("Remap"), TEXT("EnableAssetRemap"), ESettingValueType::Bool }
        };
        return Specs;
    }

    static bool ResolveSettingKeySpec(const FString& Section, const FString& Key, FString& OutSection, FString& OutKey, ESettingValueType& OutType) {
        const FString TrimmedSection = Section.TrimStartAndEnd();
        const FString TrimmedKey = Key.TrimStartAndEnd();
        for (const FSettingKeySpec& Spec : GetSettingKeySpecs()) {
            if (TrimmedSection.Equals(Spec.Section, ESearchCase::IgnoreCase) && TrimmedKey.Equals(Spec.Key, ESearchCase::IgnoreCase)) {
                OutSection = Spec.Section;
                OutKey = Spec.Key;
                OutType = Spec.Type;
                return true;
            }
        }
        return false;
    }

    static bool TryParseLinearColorValue(const FString& RawValue, FLinearColor& OutColor) {
        FString Normalized = RawValue.TrimStartAndEnd();
        if (Normalized.StartsWith(TEXT("(")) && Normalized.EndsWith(TEXT(")")) && Normalized.Len() >= 2) {
            Normalized = Normalized.Mid(1, Normalized.Len() - 2).TrimStartAndEnd();
        }

        if (!OutColor.InitFromString(Normalized)) {
            return false;
        }

        OutColor.R = FMath::Clamp(OutColor.R, 0.0f, 16.0f);
        OutColor.G = FMath::Clamp(OutColor.G, 0.0f, 16.0f);
        OutColor.B = FMath::Clamp(OutColor.B, 0.0f, 16.0f);
        OutColor.A = FMath::Clamp(OutColor.A, 0.0f, 1.0f);
        return true;
    }

    static void SanitizeFloatSettingValue(const FString& Section, const FString& Key, float& InOutValue) {
        const FString FullKey = Section + TEXT(".") + Key;

        if (FullKey == TEXT("DebugHub.PerfSampleInterval")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 30.0f); return; }
        if (FullKey == TEXT("DebugHub.RenderSampleInterval")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 30.0f); return; }
        if (FullKey == TEXT("DebugHub.SceneProbeInterval")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 30.0f); return; }
        if (FullKey == TEXT("DebugHub.LastStateWriteInterval")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 60.0f); return; }
        if (FullKey == TEXT("DebugHub.HitchThresholdMs")) { InOutValue = FMath::Clamp(InOutValue, 1.0f, 1000.0f); return; }
        if (FullKey == TEXT("DebugHub.HitchPerSignatureCooldownSec")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 600.0f); return; }

        if (FullKey == TEXT("PPV.LumenSceneLightingQuality")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 8.0f); return; }
        if (FullKey == TEXT("PPV.LumenSceneDetail")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 32.0f); return; }
        if (FullKey == TEXT("PPV.LumenSceneViewDistance")) { InOutValue = FMath::Clamp(InOutValue, 100.0f, 1000000.0f); return; }
        if (FullKey == TEXT("PPV.LumenSceneLightingUpdateSpeed")) { InOutValue = FMath::Clamp(InOutValue, 0.01f, 8.0f); return; }
        if (FullKey == TEXT("PPV.LumenFinalGatherQuality")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 8.0f); return; }
        if (FullKey == TEXT("PPV.LumenFinalGatherLightingUpdateSpeed")) { InOutValue = FMath::Clamp(InOutValue, 0.01f, 8.0f); return; }
        if (FullKey == TEXT("PPV.LumenMaxTraceDistance")) { InOutValue = FMath::Clamp(InOutValue, 100.0f, 1000000.0f); return; }
        if (FullKey == TEXT("PPV.LumenDiffuseColorBoost")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 8.0f); return; }
        if (FullKey == TEXT("PPV.LumenSkylightLeaking")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 1.0f); return; }
        if (FullKey == TEXT("PPV.LumenFullSkylightLeakingDistance")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 200000.0f); return; }
        if (FullKey == TEXT("PPV.LumenSurfaceCacheResolution")) { InOutValue = FMath::Clamp(InOutValue, 0.25f, 4.0f); return; }
        if (FullKey == TEXT("PPV.LumenReflectionQuality")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 8.0f); return; }
        if (FullKey == TEXT("PPV.IndirectLightingIntensity")) { InOutValue = FMath::Clamp(InOutValue, 0.0f, 20.0f); return; }
    }

    static void SanitizeIntSettingValue(const FString& Section, const FString& Key, int32& InOutValue) {
        const FString FullKey = Section + TEXT(".") + Key;
        if (FullKey == TEXT("DebugHub.HistorySecondsForSnapshot")) { InOutValue = FMath::Clamp(InOutValue, 1, 300); return; }
        if (FullKey == TEXT("DebugHub.MaxPerfSamples")) { InOutValue = FMath::Clamp(InOutValue, 60, 200000); return; }
        if (FullKey == TEXT("DebugHub.MaxRenderSamples")) { InOutValue = FMath::Clamp(InOutValue, 60, 50000); return; }
        if (FullKey == TEXT("DebugHub.MaxProbeSamples")) { InOutValue = FMath::Clamp(InOutValue, 30, 10000); return; }
        if (FullKey == TEXT("DebugHub.MaxEvents")) { InOutValue = FMath::Clamp(InOutValue, 64, 20000); return; }
        if (FullKey == TEXT("DebugHub.MaxCopiedHitchSnapshots")) { InOutValue = FMath::Clamp(InOutValue, 0, 50); return; }
        if (FullKey == TEXT("Visualization.ModeSwitchCooldownMs")) { InOutValue = FMath::Clamp(InOutValue, 0, 5000); return; }
        if (FullKey == TEXT("PPV.LumenRayLightingMode")) { InOutValue = FMath::Clamp(InOutValue, 0, 1); return; }
        if (FullKey == TEXT("PPV.LumenFrontLayerTranslucencyReflections")) { InOutValue = FMath::Clamp(InOutValue, 0, 1); return; }
        if (FullKey == TEXT("PPV.LumenMaxReflectionBounces")) { InOutValue = FMath::Clamp(InOutValue, 1, 64); return; }
    }

    static void ValidateAndClampRuntimeConfig(FPPVConfig& Config) {
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenSceneLightingQuality"), Config.LumenSceneLightingQuality);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenSceneDetail"), Config.LumenSceneDetail);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenSceneViewDistance"), Config.LumenSceneViewDistance);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenSceneLightingUpdateSpeed"), Config.LumenSceneLightingUpdateSpeed);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenFinalGatherQuality"), Config.LumenFinalGatherQuality);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenFinalGatherLightingUpdateSpeed"), Config.LumenFinalGatherLightingUpdateSpeed);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenMaxTraceDistance"), Config.LumenMaxTraceDistance);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenDiffuseColorBoost"), Config.LumenDiffuseColorBoost);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenSkylightLeaking"), Config.LumenSkylightLeaking);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenFullSkylightLeakingDistance"), Config.LumenFullSkylightLeakingDistance);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenSurfaceCacheResolution"), Config.LumenSurfaceCacheResolution);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("LumenReflectionQuality"), Config.LumenReflectionQuality);
        SanitizeFloatSettingValue(TEXT("PPV"), TEXT("IndirectLightingIntensity"), Config.IndirectLightingIntensity);

        SanitizeIntSettingValue(TEXT("PPV"), TEXT("LumenRayLightingMode"), Config.LumenRayLightingMode);
        SanitizeIntSettingValue(TEXT("PPV"), TEXT("LumenFrontLayerTranslucencyReflections"), Config.LumenFrontLayerTranslucencyReflections);
        SanitizeIntSettingValue(TEXT("PPV"), TEXT("LumenMaxReflectionBounces"), Config.LumenMaxReflectionBounces);

        Config.IndirectLightingColor.R = FMath::Clamp(Config.IndirectLightingColor.R, 0.0f, 16.0f);
        Config.IndirectLightingColor.G = FMath::Clamp(Config.IndirectLightingColor.G, 0.0f, 16.0f);
        Config.IndirectLightingColor.B = FMath::Clamp(Config.IndirectLightingColor.B, 0.0f, 16.0f);
        Config.IndirectLightingColor.A = FMath::Clamp(Config.IndirectLightingColor.A, 0.0f, 1.0f);
    }

    static void LoadStringRemapSection(const FString& ConfigPath, const TCHAR* SectionName, TMap<FString, FString>& OutMap, TMap<FString, FString>* OutNormalizedMap = nullptr) {
        OutMap.Reset();
        if (OutNormalizedMap) {
            OutNormalizedMap->Reset();
        }
        if (!GConfig || !SectionName) {
            return;
        }

        TArray<FString> Entries;
        if (!GConfig->GetSection(SectionName, Entries, ConfigPath)) {
            return;
        }

        for (const FString& Entry : Entries) {
            FString Source;
            FString Target;
            if (Entry.Split(TEXT("="), &Source, &Target, ESearchCase::CaseSensitive)) {
                Source = NormalizeRemapPath(Source);
                Target = NormalizeRemapPath(Target);
                if (!Source.IsEmpty() && !Target.IsEmpty()) {
                    OutMap.Add(Source, Target);
                    if (OutNormalizedMap) {
                        OutNormalizedMap->Add(Source.ToLower(), Target);
                    }
                }
            }
        }
    }

    static FString GetPPVConfigPath() {
        return TajsGraphSmlSettings::GetConfigFilePath();
    }

    static void LoadPersistentPPVConfig(FPPVConfig& Config) {
        FTajsGraphModule::GetBoolSetting(TEXT("Debug"), TEXT("EnableLogging"), Config.bEnableLogging);
        FTajsGraphModule::GetBoolSetting(TEXT("Debug"), TEXT("EnableDebugLogging"), Config.bEnableDebugLogging);
        FTajsGraphModule::GetBoolSetting(TEXT("Debug"), TEXT("PollExternalConfigChanges"), Config.bPollExternalConfigChanges);

        FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("Enabled"), Config.bEnabled);
        FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenMethods"), Config.bOverrideLumenMethods);
        FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenValues"), Config.bOverrideLumenValues);
        FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenRayLightingMode"), Config.bOverrideLumenRayLightingMode);
        FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenFrontLayerTranslucencyReflections"), Config.bOverrideLumenFrontLayerTranslucencyReflections);
        FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideLumenMaxReflectionBounces"), Config.bOverrideLumenMaxReflectionBounces);
        FTajsGraphModule::GetBoolSetting(TEXT("Instancing"), TEXT("ForceInstancing"), Config.bForceInstancing);
        FTajsGraphModule::GetBoolSetting(TEXT("Instancing"), TEXT("ForceLumenInstancing"), Config.bForceLumenInstancing);
        FTajsGraphModule::GetBoolSetting(TEXT("Nanite"), TEXT("ForceNanite"), Config.bForceNanite);
        FTajsGraphModule::GetBoolSetting(TEXT("Nanite"), TEXT("ForceNaniteForMasked"), Config.bForceNaniteForMasked);
        FTajsGraphModule::GetBoolSetting(TEXT("Foliage"), TEXT("FixBuildableFoliage"), Config.bFixBuildableFoliage);
        const bool bHasSmlRemapFlag = FTajsGraphModule::GetBoolSetting(TEXT("Remap"), TEXT("EnableAssetRemap"), Config.bEnableAssetRemap);

        const FString StaticConfigPath = GetBundledSettingsConfigPath();
        if (!bHasSmlRemapFlag && GConfig && !StaticConfigPath.IsEmpty()) {
            GConfig->GetBool(TEXT("Remap"), TEXT("EnableAssetRemap"), Config.bEnableAssetRemap, StaticConfigPath);
        }
        LoadStringRemapSection(StaticConfigPath, TEXT("MeshRemap"), Config.MeshRemap, &Config.MeshRemapNormalized);
        LoadStringRemapSection(StaticConfigPath, TEXT("MaterialRemap"), Config.MaterialRemap, &Config.MaterialRemapNormalized);

        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSceneLightingQuality"), Config.LumenSceneLightingQuality);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSceneDetail"), Config.LumenSceneDetail);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSceneViewDistance"), Config.LumenSceneViewDistance);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSceneLightingUpdateSpeed"), Config.LumenSceneLightingUpdateSpeed);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenFinalGatherQuality"), Config.LumenFinalGatherQuality);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenFinalGatherLightingUpdateSpeed"), Config.LumenFinalGatherLightingUpdateSpeed);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenMaxTraceDistance"), Config.LumenMaxTraceDistance);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenDiffuseColorBoost"), Config.LumenDiffuseColorBoost);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSkylightLeaking"), Config.LumenSkylightLeaking);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenFullSkylightLeakingDistance"), Config.LumenFullSkylightLeakingDistance);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenSurfaceCacheResolution"), Config.LumenSurfaceCacheResolution);
        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("LumenReflectionQuality"), Config.LumenReflectionQuality);
        FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideIndirectLightingColor"), Config.bOverrideIndirectLightingColor);
        FTajsGraphModule::GetBoolSetting(TEXT("PPV"), TEXT("OverrideIndirectLightingIntensity"), Config.bOverrideIndirectLightingIntensity);
        FTajsGraphModule::GetBoolSetting(TEXT("Shadows"), TEXT("VirtualShadowMaps"), Config.bVirtualShadowMaps);
        FTajsGraphModule::GetBoolSetting(TEXT("Shadows"), TEXT("VSMFarShadowCulling"), Config.bVSMFarShadowCulling);
        FTajsGraphModule::GetBoolSetting(TEXT("Shadows"), TEXT("CapsuleShadows"), Config.bCapsuleShadows);
        FTajsGraphModule::GetBoolSetting(TEXT("Shadows"), TEXT("CascadedShadows"), Config.bCascadedShadows);
        FTajsGraphModule::GetBoolSetting(TEXT("Shadows"), TEXT("ContactShadows"), Config.bContactShadows);
        FTajsGraphModule::GetBoolSetting(TEXT("Shadows"), TEXT("DistanceFieldShadows"), Config.bDistanceFieldShadows);

        FTajsGraphModule::GetBoolSetting(TEXT("Lumen"), TEXT("Lumen"), Config.bLumen);
        FTajsGraphModule::GetBoolSetting(TEXT("Lumen"), TEXT("AsyncIndirectLighting"), Config.bAsyncIndirectLighting);
        FTajsGraphModule::GetBoolSetting(TEXT("Lumen"), TEXT("AsyncReflections"), Config.bAsyncReflections);
        FTajsGraphModule::GetBoolSetting(TEXT("Lumen"), TEXT("AsyncSceneLighting"), Config.bAsyncSceneLighting);
        FTajsGraphModule::GetBoolSetting(TEXT("Lumen"), TEXT("LumenScreenTraces"), Config.bLumenScreenTraces);
        FTajsGraphModule::GetBoolSetting(TEXT("Lumen"), TEXT("UnoptimizedLumen"), Config.bUnoptimizedLumen);
        FTajsGraphModule::GetBoolSetting(TEXT("Lumen"), TEXT("TranslucentSurfaceReflections"), Config.bTranslucentSurfaceReflections);
        FTajsGraphModule::GetBoolSetting(TEXT("Lumen"), TEXT("LumenReflectionsScreenSpaceReconstruction"), Config.bLumenReflectionsScreenSpaceReconstruction);

        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("FogEnabled"), Config.bFogEnabled);
        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("SkyAtmosphereEnabled"), Config.bSkyAtmosphereEnabled);
        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("GPUOcclusionQueries"), Config.bGPUOcclusionQueries);
        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("EnhancedAutoExposure"), Config.bEnhancedAutoExposure);
        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("EnableDLSSFG"), Config.bEnableDLSSFG);
        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("EnableReflex"), Config.bEnableReflex);
        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("ForceHighestMipOnUI"), Config.bForceHighestMipOnUI);
        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("FullyLoadUsedTextures"), Config.bFullyLoadUsedTextures);
        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("TickAllowAsyncTickCleanup"), Config.bTickAllowAsyncTickCleanup);
        FTajsGraphModule::GetBoolSetting(TEXT("Rendering"), TEXT("TickAllowAsyncTickDispatch"), Config.bTickAllowAsyncTickDispatch);

        FTajsGraphModule::GetFloatSetting(TEXT("PPV"), TEXT("IndirectLightingIntensity"), Config.IndirectLightingIntensity);

        FString IndirectLightingColorString;
        if (FTajsGraphModule::GetStringSetting(TEXT("PPV"), TEXT("IndirectLightingColor"), IndirectLightingColorString)) {
            FLinearColor ParsedColor;
            if (TryParseLinearColorValue(IndirectLightingColorString, ParsedColor)) {
                Config.IndirectLightingColor = ParsedColor;
            } else if (ShouldGeneralLog()) {
                UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] Invalid PPV.IndirectLightingColor value in SML config: %s"), *IndirectLightingColorString);
            }
        }
        FTajsGraphModule::GetIntSetting(TEXT("PPV"), TEXT("LumenRayLightingMode"), Config.LumenRayLightingMode);
        FTajsGraphModule::GetIntSetting(TEXT("PPV"), TEXT("LumenFrontLayerTranslucencyReflections"), Config.LumenFrontLayerTranslucencyReflections);
        FTajsGraphModule::GetIntSetting(TEXT("PPV"), TEXT("LumenMaxReflectionBounces"), Config.LumenMaxReflectionBounces);

        ValidateAndClampRuntimeConfig(Config);
    }

    static bool TryGetRemapTarget(const TMap<FString, FString>& RemapMap, const TMap<FString, FString>& NormalizedRemapMap, const FString& SourcePath, FString& OutTargetPath) {
        const FString NormalizedSource = NormalizeRemapPath(SourcePath);

        if (const FString* DirectMatch = RemapMap.Find(NormalizedSource)) {
            OutTargetPath = *DirectMatch;
            return true;
        }

        if (const FString* LowercaseMatch = NormalizedRemapMap.Find(NormalizedSource.ToLower())) {
            OutTargetPath = *LowercaseMatch;
            return true;
        }

        for (const TPair<FString, FString>& Pair : RemapMap) {
            if (NormalizedSource.Equals(Pair.Key, ESearchCase::IgnoreCase)) {
                OutTargetPath = Pair.Value;
                return true;
            }
        }

        return false;
    }

    template <typename T>
    static T* LoadAssetByPath(const FString& AssetPath) {
        if (AssetPath.IsEmpty()) {
            return nullptr;
        }

        TSoftObjectPtr<T> SoftPtr{FSoftObjectPath(AssetPath)};
        return SoftPtr.LoadSynchronous();
    }

    static void ApplyRuntimeAssetRemap(UStaticMeshComponent* Component, const FPPVConfig& Config) {
        if (!Component || !Config.bEnableAssetRemap) {
            return;
        }

        static TSet<FString> LoggedMessages;

        if (UStaticMesh* CurrentMesh = Component->GetStaticMesh()) {
            FString RemappedMeshPath;
            if (TryGetRemapTarget(Config.MeshRemap, Config.MeshRemapNormalized, CurrentMesh->GetPathName(), RemappedMeshPath)) {
                if (UStaticMesh* NewMesh = LoadAssetByPath<UStaticMesh>(RemappedMeshPath)) {
                    if (NewMesh != CurrentMesh) {
                        Component->SetStaticMesh(NewMesh);

                        const FString LogKey = FString::Printf(TEXT("MeshSwap|%s|%s"), *CurrentMesh->GetPathName(), *RemappedMeshPath);
                        if (!LoggedMessages.Contains(LogKey)) {
                            LoggedMessages.Add(LogKey);
                            if (ShouldDebugLog()) {
                                UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug][Remap] Mesh %s -> %s"), *CurrentMesh->GetPathName(), *RemappedMeshPath);
                            }
                        }
                    }
                } else {
                    const FString LogKey = FString::Printf(TEXT("MeshMissing|%s"), *RemappedMeshPath);
                    if (!LoggedMessages.Contains(LogKey)) {
                        LoggedMessages.Add(LogKey);
                        if (ShouldGeneralLog()) {
                            UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph][Remap] Failed to load remapped mesh: %s"), *RemappedMeshPath);
                        }
                    }
                }
            }
        }

        const int32 MaterialCount = Component->GetNumMaterials();
        for (int32 MaterialIndex = 0; MaterialIndex < MaterialCount; ++MaterialIndex) {
            UMaterialInterface* CurrentMaterial = Component->GetMaterial(MaterialIndex);
            if (!CurrentMaterial) {
                continue;
            }

            FString RemappedMaterialPath;
            if (!TryGetRemapTarget(Config.MaterialRemap, Config.MaterialRemapNormalized, CurrentMaterial->GetPathName(), RemappedMaterialPath)) {
                continue;
            }

            if (UMaterialInterface* NewMaterial = LoadAssetByPath<UMaterialInterface>(RemappedMaterialPath)) {
                if (NewMaterial != CurrentMaterial) {
                    Component->SetMaterial(MaterialIndex, NewMaterial);
                    const FString LogKey = FString::Printf(TEXT("MatSwap|%s|%s|%d"), *CurrentMaterial->GetPathName(), *RemappedMaterialPath, MaterialIndex);
                    if (!LoggedMessages.Contains(LogKey)) {
                        LoggedMessages.Add(LogKey);
                        if (ShouldDebugLog()) {
                            UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug][Remap] Material[%d] %s -> %s"), MaterialIndex, *CurrentMaterial->GetPathName(), *RemappedMaterialPath);
                        }
                    }
                }
            } else {
                const FString LogKey = FString::Printf(TEXT("MatMissing|%s"), *RemappedMaterialPath);
                    if (!LoggedMessages.Contains(LogKey)) {
                        LoggedMessages.Add(LogKey);
                        if (ShouldGeneralLog()) {
                            UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph][Remap] Failed to load remapped material: %s"), *RemappedMaterialPath);
                        }
                    }
            }
        }
    }

    static void SetConsoleBool(const TCHAR* Name, const bool bValue) {
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name)) {
            const int32 Desired = bValue ? 1 : 0;
            if (CVar->GetInt() == Desired) {
                return;
            }

            CVar->Set(Desired, ECVF_SetByCode);
            if (ShouldDebugLog()) {
                UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug] CVar bool %s=%d"), Name, Desired);
            }
            return;
        }

        if (!bLoggedMissingCVarWarnings && ShouldGeneralLog()) {
            UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] Missing console variable: %s"), Name);
        }
    }

    static void SetConsoleInt(const TCHAR* Name, const int32 Value) {
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name)) {
            if (CVar->GetInt() == Value) {
                return;
            }

            CVar->Set(Value, ECVF_SetByCode);
            if (ShouldDebugLog()) {
                UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug] CVar int %s=%d"), Name, Value);
            }
            return;
        }

        if (!bLoggedMissingCVarWarnings && ShouldGeneralLog()) {
            UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] Missing console variable: %s"), Name);
        }
    }

    static void SetConsoleFloat(const TCHAR* Name, const float Value) {
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name)) {
            if (FMath::IsNearlyEqual(CVar->GetFloat(), Value, KINDA_SMALL_NUMBER)) {
                return;
            }

            CVar->Set(Value, ECVF_SetByCode);
            if (ShouldDebugLog()) {
                UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug] CVar float %s=%f"), Name, Value);
            }
            return;
        }

        if (!bLoggedMissingCVarWarnings && ShouldGeneralLog()) {
            UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] Missing console variable: %s"), Name);
        }
    }

    static void ApplyRuntimeCVarOverrides(const FPPVConfig& Config) {
        if (Config.bEnableLogging || Config.bEnableDebugLogging) {
            UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph] Applying runtime CVar overrides (debug=%s)"), Config.bEnableDebugLogging ? TEXT("true") : TEXT("false"));
        }

        // Baseline commands from Blueprint startup sequence.
        SetConsoleInt(TEXT("r.AOGlobalDistanceField.FastCameraMode"), 0);
        SetConsoleFloat(TEXT("r.DFShadow.TwoSidedMeshDistanceBiasScale"), 0.2f);

        // Lumen enable/disable command chain.
        SetConsoleInt(TEXT("r.ReflectionMethod"), Config.bLumen ? 1 : 0);
        SetConsoleInt(TEXT("r.DynamicGlobalIlluminationMethod"), Config.bLumen ? 1 : 0);
        SetConsoleInt(TEXT("r.Lumen.ScreenProbeGather"), Config.bLumen ? 1 : 0);
        SetConsoleInt(TEXT("r.Lumen.DiffuseIndirect.Allow"), Config.bLumen ? 1 : 0);
        SetConsoleInt(TEXT("r.Lumen.Reflections.Allow"), Config.bLumen ? 1 : 0);

        SetConsoleBool(TEXT("r.Shadow.Virtual.Enable"), Config.bVirtualShadowMaps);
        SetConsoleBool(TEXT("r.Shadow.Virtual.UseFarShadowCulling"), Config.bVSMFarShadowCulling);
        SetConsoleBool(TEXT("r.Lumen.DiffuseIndirect.AsyncCompute"), Config.bAsyncIndirectLighting);
        SetConsoleBool(TEXT("r.Lumen.Reflections.AsyncCompute"), Config.bAsyncReflections);
        SetConsoleBool(TEXT("r.LumenScene.Lighting.AsyncCompute"), Config.bAsyncSceneLighting);
        SetConsoleBool(TEXT("r.Lumen.Reflections.ScreenTraces"), Config.bLumenScreenTraces);
        SetConsoleBool(TEXT("r.Lumen.Reflections.ScreenSpaceReconstruction"), Config.bLumenReflectionsScreenSpaceReconstruction);
        SetConsoleBool(TEXT("r.Lumen.TranslucencyReflections.FrontLayer.Enable"), Config.bTranslucentSurfaceReflections);
        SetConsoleBool(TEXT("r.Lumen.TranslucencyReflections.FrontLayer.EnableForProject"), Config.bTranslucentSurfaceReflections);

        SetConsoleBool(TEXT("r.CapsuleShadows"), Config.bCapsuleShadows);
        SetConsoleBool(TEXT("r.CapsuleDirectShadows"), Config.bCapsuleShadows);
        SetConsoleBool(TEXT("r.CapsuleIndirectShadows"), Config.bCapsuleShadows);
        SetConsoleInt(TEXT("r.Shadow.CSM.MaxCascades"), Config.bCascadedShadows ? 4 : 0);
        SetConsoleInt(TEXT("r.Shadow.CSMCaching"), Config.bCascadedShadows ? 4 : 0);
        SetConsoleInt(TEXT("r.Shadow.MaxCSMResolution"), Config.bCascadedShadows ? 2048 : 0);
        SetConsoleBool(TEXT("r.ContactShadows"), Config.bContactShadows);
        SetConsoleBool(TEXT("r.DistanceFieldShadowing"), Config.bDistanceFieldShadows);
        SetConsoleBool(TEXT("r.Lumen.ScreenProbeGather.ReferenceMode"), Config.bUnoptimizedLumen);

        SetConsoleBool(TEXT("r.Fog"), Config.bFogEnabled);
        SetConsoleBool(TEXT("r.SkyAtmosphere"), Config.bSkyAtmosphereEnabled);
        SetConsoleBool(TEXT("D3D12.InsertOuterOcclusionQuery"), Config.bGPUOcclusionQueries);
        SetConsoleBool(TEXT("r.AutoExposure.IgnoreMaterials"), Config.bEnhancedAutoExposure);
        SetConsoleBool(TEXT("r.AutoExposure.IgnoreMaterials.UsePrecalculatedIlluminance"), Config.bEnhancedAutoExposure);
        SetConsoleFloat(TEXT("r.AutoExposure.IgnoreMaterials.LuminanceScale"), Config.bEnhancedAutoExposure ? 0.25f : 0.18f);
        SetConsoleBool(TEXT("r.Streamline.DLSSG.Enable"), Config.bEnableDLSSFG);
        SetConsoleBool(TEXT("t.Streamline.Reflex.Enable"), Config.bEnableReflex);
        SetConsoleBool(TEXT("r.ForceHighestMipOnUITextures"), Config.bForceHighestMipOnUI);
        SetConsoleBool(TEXT("r.Streaming.FullyLoadUsedTextures"), Config.bFullyLoadUsedTextures);
        SetConsoleBool(TEXT("tick.AllowAsyncTickCleanup"), Config.bTickAllowAsyncTickCleanup);
        SetConsoleBool(TEXT("tick.AllowAsyncTickDispatch"), Config.bTickAllowAsyncTickDispatch);
        bLoggedMissingCVarWarnings = true;
    }

    static void RefreshRuntimeConfig(UWorld* World, const bool bForce = false) {
        if (!IsInGameThread()) {
            return;
        }

        const double NowSeconds = FPlatformTime::Seconds();
        if (!bForce && !bPendingRuntimeApply && (NowSeconds - GLastRuntimeConfigRefreshSeconds) < GConfigRefreshIntervalSeconds) {
            return;
        }

        GLastRuntimeConfigRefreshSeconds = NowSeconds;

        FPPVConfig UpdatedConfig;
        {
            FScopeLock Lock(&GPPVConfigMutex);
            UpdatedConfig = GPPVConfig;
        }

        PrivateConfigRuntime::LoadPersistentPPVConfig(UpdatedConfig);

        if (!UpdatedConfig.bEnableAssetRemap) {
            UpdatedConfig.MeshRemap.Reset();
            UpdatedConfig.MaterialRemap.Reset();
            UpdatedConfig.MeshRemapNormalized.Reset();
            UpdatedConfig.MaterialRemapNormalized.Reset();
        }

        {
            FScopeLock Lock(&GPPVConfigMutex);
            GPPVConfig = MoveTemp(UpdatedConfig);
        }

        bPendingRuntimeApply = true;

        FPPVConfig ConfigSnapshot;
        {
            FScopeLock Lock(&GPPVConfigMutex);
            ConfigSnapshot = GPPVConfig;
        }

        if (GStartupTimeSeconds <= 0.0) {
            GStartupTimeSeconds = NowSeconds;
        }

        const double ElapsedSeconds = NowSeconds - GStartupTimeSeconds;
        if (ElapsedSeconds < GCVarApplyDelaySeconds) {
            if (!bLoggedCVarDelay && ShouldGeneralLog()) {
                UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph] Delaying CVar apply for %.1fs (elapsed %.2fs)"), GCVarApplyDelaySeconds, ElapsedSeconds);
                bLoggedCVarDelay = true;
            }
            return;
        }

        ApplyRuntimeCVarOverrides(ConfigSnapshot);
        bPendingRuntimeApply = false;
    }

    static UWorld* GetAnyGameWorld() {
        if (!GEngine) {
            return nullptr;
        }

        for (const FWorldContext& Context : GEngine->GetWorldContexts()) {
            if (UWorld* World = Context.World()) {
                if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE) {
                    return World;
                }
            }
        }
        return nullptr;
    }

    static bool TryResolveSettingForAccess(const FString& Section, const FString& Key, const ESettingValueType ExpectedType, FString& OutSection, FString& OutKey) {
        ESettingValueType ActualType = ESettingValueType::String;
        if (!ResolveSettingKeySpec(Section, Key, OutSection, OutKey, ActualType)) {
            if (ShouldGeneralLog()) {
                UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] Rejected unknown setting key: %s.%s"), *Section, *Key);
            }
            return false;
        }

        if (ActualType != ExpectedType) {
            if (ShouldGeneralLog()) {
                UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] Rejected setting type mismatch for %s.%s"), *OutSection, *OutKey);
            }
            return false;
        }

        return true;
    }

    static bool SetConfigBoolValue(const FString& Section, const FString& Key, const bool bValue, const bool bApplyImmediately) {
        if (Section.IsEmpty() || Key.IsEmpty()) {
            return false;
        }

        FString CanonicalSection;
        FString CanonicalKey;
        if (!TryResolveSettingForAccess(Section, Key, ESettingValueType::Bool, CanonicalSection, CanonicalKey)) {
            return false;
        }

        if (!TajsGraphSmlSettings::SetBool(CanonicalSection, CanonicalKey, bValue)) {
            return false;
        }

        GOnSettingChangedDelegate.Broadcast(CanonicalSection, CanonicalKey, bValue ? TEXT("true") : TEXT("false"));
        if (ShouldDebugLog()) {
            UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug] Set bool setting %s.%s=%s"), *CanonicalSection, *CanonicalKey, bValue ? TEXT("true") : TEXT("false"));
        }

        if (bApplyImmediately) {
            RefreshRuntimeConfig(GetAnyGameWorld(), true);
        }
        return true;
    }

    static bool SetConfigIntValue(const FString& Section, const FString& Key, const int32 Value, const bool bApplyImmediately) {
        if (Section.IsEmpty() || Key.IsEmpty()) {
            return false;
        }

        FString CanonicalSection;
        FString CanonicalKey;
        if (!TryResolveSettingForAccess(Section, Key, ESettingValueType::Int, CanonicalSection, CanonicalKey)) {
            return false;
        }

        int32 SanitizedValue = Value;
        SanitizeIntSettingValue(CanonicalSection, CanonicalKey, SanitizedValue);

        if (!TajsGraphSmlSettings::SetInt(CanonicalSection, CanonicalKey, SanitizedValue)) {
            return false;
        }

        GOnSettingChangedDelegate.Broadcast(CanonicalSection, CanonicalKey, FString::FromInt(SanitizedValue));
        if (ShouldDebugLog()) {
            UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug] Set int setting %s.%s=%d"), *CanonicalSection, *CanonicalKey, SanitizedValue);
        }

        if (bApplyImmediately) {
            RefreshRuntimeConfig(GetAnyGameWorld(), true);
        }
        return true;
    }

    static bool SetConfigFloatValue(const FString& Section, const FString& Key, const float Value, const bool bApplyImmediately) {
        if (Section.IsEmpty() || Key.IsEmpty()) {
            return false;
        }

        FString CanonicalSection;
        FString CanonicalKey;
        if (!TryResolveSettingForAccess(Section, Key, ESettingValueType::Float, CanonicalSection, CanonicalKey)) {
            return false;
        }

        float SanitizedValue = Value;
        SanitizeFloatSettingValue(CanonicalSection, CanonicalKey, SanitizedValue);

        if (!TajsGraphSmlSettings::SetFloat(CanonicalSection, CanonicalKey, SanitizedValue)) {
            return false;
        }

        GOnSettingChangedDelegate.Broadcast(CanonicalSection, CanonicalKey, FString::SanitizeFloat(SanitizedValue));
        if (ShouldDebugLog()) {
            UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug] Set float setting %s.%s=%f"), *CanonicalSection, *CanonicalKey, SanitizedValue);
        }

        if (bApplyImmediately) {
            RefreshRuntimeConfig(GetAnyGameWorld(), true);
        }
        return true;
    }

    static bool SetConfigStringValue(const FString& Section, const FString& Key, const FString& Value, const bool bApplyImmediately) {
        if (Section.IsEmpty() || Key.IsEmpty()) {
            return false;
        }

        FString CanonicalSection;
        FString CanonicalKey;
        if (!TryResolveSettingForAccess(Section, Key, ESettingValueType::String, CanonicalSection, CanonicalKey)) {
            return false;
        }

        FString SanitizedValue = Value;
        if (CanonicalSection == TEXT("PPV") && CanonicalKey == TEXT("IndirectLightingColor")) {
            FLinearColor ParsedColor;
            if (!TryParseLinearColorValue(Value, ParsedColor)) {
                if (ShouldGeneralLog()) {
                    UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] Rejected invalid color for %s.%s: %s"), *CanonicalSection, *CanonicalKey, *Value);
                }
                return false;
            }
            SanitizedValue = ParsedColor.ToString();
        }

        if (!TajsGraphSmlSettings::SetString(CanonicalSection, CanonicalKey, SanitizedValue)) {
            return false;
        }

        GOnSettingChangedDelegate.Broadcast(CanonicalSection, CanonicalKey, SanitizedValue);
        if (ShouldDebugLog()) {
            UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug] Set string setting %s.%s=%s"), *CanonicalSection, *CanonicalKey, *SanitizedValue);
        }

        if (bApplyImmediately) {
            RefreshRuntimeConfig(GetAnyGameWorld(), true);
        }
        return true;
    }

    static bool GetConfigBoolValue(const FString& Section, const FString& Key, bool& OutValue) {
        if (Section.IsEmpty() || Key.IsEmpty()) {
            return false;
        }

        FString CanonicalSection;
        FString CanonicalKey;
        if (!TryResolveSettingForAccess(Section, Key, ESettingValueType::Bool, CanonicalSection, CanonicalKey)) {
            return false;
        }

        return TajsGraphSmlSettings::GetBool(CanonicalSection, CanonicalKey, OutValue);
    }

    static bool GetConfigIntValue(const FString& Section, const FString& Key, int32& OutValue) {
        if (Section.IsEmpty() || Key.IsEmpty()) {
            return false;
        }

        FString CanonicalSection;
        FString CanonicalKey;
        if (!TryResolveSettingForAccess(Section, Key, ESettingValueType::Int, CanonicalSection, CanonicalKey)) {
            return false;
        }

        return TajsGraphSmlSettings::GetInt(CanonicalSection, CanonicalKey, OutValue);
    }

    static bool GetConfigFloatValue(const FString& Section, const FString& Key, float& OutValue) {
        if (Section.IsEmpty() || Key.IsEmpty()) {
            return false;
        }

        FString CanonicalSection;
        FString CanonicalKey;
        if (!TryResolveSettingForAccess(Section, Key, ESettingValueType::Float, CanonicalSection, CanonicalKey)) {
            return false;
        }

        return TajsGraphSmlSettings::GetFloat(CanonicalSection, CanonicalKey, OutValue);
    }

    static bool GetConfigStringValue(const FString& Section, const FString& Key, FString& OutValue) {
        if (Section.IsEmpty() || Key.IsEmpty()) {
            return false;
        }

        FString CanonicalSection;
        FString CanonicalKey;
        if (!TryResolveSettingForAccess(Section, Key, ESettingValueType::String, CanonicalSection, CanonicalKey)) {
            return false;
        }

        return TajsGraphSmlSettings::GetString(CanonicalSection, CanonicalKey, OutValue);
    }

    static void ApplyPPVOverrides(FPostProcessSettings& Settings, const FPPVConfig& Config) {
        if (!Config.bEnabled) {
            return;
        }

        if (Config.bOverrideLumenMethods) {
            Settings.bOverride_DynamicGlobalIlluminationMethod = true;
            Settings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Lumen;

            Settings.bOverride_ReflectionMethod = true;
            Settings.ReflectionMethod = EReflectionMethod::Lumen;
        }

        if (Config.bOverrideLumenValues) {
            Settings.bOverride_LumenSceneLightingQuality = true;
            Settings.bOverride_LumenSceneDetail = true;
            Settings.bOverride_LumenSceneViewDistance = true;
            Settings.bOverride_LumenSceneLightingUpdateSpeed = true;
            Settings.bOverride_LumenFinalGatherQuality = true;
            Settings.bOverride_LumenFinalGatherLightingUpdateSpeed = true;
            Settings.bOverride_LumenMaxTraceDistance = true;
            Settings.bOverride_LumenDiffuseColorBoost = true;
            Settings.bOverride_LumenSkylightLeaking = true;
            Settings.bOverride_LumenFullSkylightLeakingDistance = true;
            Settings.bOverride_LumenSurfaceCacheResolution = true;
            Settings.bOverride_LumenReflectionQuality = true;

            Settings.LumenSceneLightingQuality = Config.LumenSceneLightingQuality;
            Settings.LumenSceneDetail = Config.LumenSceneDetail;
            Settings.LumenSceneViewDistance = Config.LumenSceneViewDistance;
            Settings.LumenSceneLightingUpdateSpeed = Config.LumenSceneLightingUpdateSpeed;
            Settings.LumenFinalGatherQuality = Config.LumenFinalGatherQuality;
            Settings.LumenFinalGatherLightingUpdateSpeed = Config.LumenFinalGatherLightingUpdateSpeed;
            Settings.LumenMaxTraceDistance = Config.LumenMaxTraceDistance;
            Settings.LumenDiffuseColorBoost = Config.LumenDiffuseColorBoost;
            Settings.LumenSkylightLeaking = Config.LumenSkylightLeaking;
            Settings.LumenFullSkylightLeakingDistance = Config.LumenFullSkylightLeakingDistance;
            Settings.LumenSurfaceCacheResolution = Config.LumenSurfaceCacheResolution;
            Settings.LumenReflectionQuality = Config.LumenReflectionQuality;
        }

        if (Config.bOverrideIndirectLightingColor) {
            Settings.bOverride_IndirectLightingColor = true;
            Settings.IndirectLightingColor = Config.IndirectLightingColor;
        }

        if (Config.bOverrideIndirectLightingIntensity) {
            Settings.bOverride_IndirectLightingIntensity = true;
            Settings.IndirectLightingIntensity = Config.IndirectLightingIntensity;
        }

        if (Config.bOverrideLumenRayLightingMode) {
            Settings.bOverride_LumenRayLightingMode = true;
            Settings.LumenRayLightingMode = static_cast<ELumenRayLightingModeOverride>(Config.LumenRayLightingMode);
        }

        if (Config.bOverrideLumenFrontLayerTranslucencyReflections) {
            Settings.bOverride_LumenFrontLayerTranslucencyReflections = true;
            Settings.LumenFrontLayerTranslucencyReflections = Config.LumenFrontLayerTranslucencyReflections;
        }

        if (Config.bOverrideLumenMaxReflectionBounces) {
            Settings.bOverride_LumenMaxReflectionBounces = true;
            Settings.LumenMaxReflectionBounces = Config.LumenMaxReflectionBounces;
        }
    }


