#include "Debug/TajsGraphDebugViewExtension.h"

#include "Debug/TajsGraphDebugSubsystem.h"
#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/GameUserSettings.h"
#include "HAL/IConsoleManager.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "SceneView.h"

namespace {

static float GetCVarFloatValue(const TCHAR* Name, const float DefaultValue = 0.0f) {
    if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name)) {
        return CVar->GetFloat();
    }
    return DefaultValue;
}

static int32 GetCVarIntValue(const TCHAR* Name, const int32 DefaultValue = 0) {
    if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name)) {
        return CVar->GetInt();
    }
    return DefaultValue;
}

static void CaptureWatchedPostProcessValues(const FPostProcessSettings& Settings, TMap<FString, float>& OutValues) {
    OutValues.Reset();
    OutValues.Reserve(24);

    OutValues.Add(TEXT("AutoExposureMinBrightness"), Settings.AutoExposureMinBrightness);
    OutValues.Add(TEXT("AutoExposureMaxBrightness"), Settings.AutoExposureMaxBrightness);
    OutValues.Add(TEXT("AutoExposureBias"), Settings.AutoExposureBias);
    OutValues.Add(TEXT("BloomIntensity"), Settings.BloomIntensity);
    OutValues.Add(TEXT("VignetteIntensity"), Settings.VignetteIntensity);
    OutValues.Add(TEXT("FilmShoulder"), Settings.FilmShoulder);
    OutValues.Add(TEXT("DepthOfFieldFstop"), Settings.DepthOfFieldFstop);

    OutValues.Add(TEXT("LumenSceneLightingQuality"), Settings.LumenSceneLightingQuality);
    OutValues.Add(TEXT("LumenSceneDetail"), Settings.LumenSceneDetail);
    OutValues.Add(TEXT("LumenSceneViewDistance"), Settings.LumenSceneViewDistance);
    OutValues.Add(TEXT("LumenSceneLightingUpdateSpeed"), Settings.LumenSceneLightingUpdateSpeed);
    OutValues.Add(TEXT("LumenFinalGatherQuality"), Settings.LumenFinalGatherQuality);
    OutValues.Add(TEXT("LumenFinalGatherLightingUpdateSpeed"), Settings.LumenFinalGatherLightingUpdateSpeed);
    OutValues.Add(TEXT("LumenMaxTraceDistance"), Settings.LumenMaxTraceDistance);
    OutValues.Add(TEXT("LumenSkylightLeaking"), Settings.LumenSkylightLeaking);
    OutValues.Add(TEXT("LumenFullSkylightLeakingDistance"), Settings.LumenFullSkylightLeakingDistance);
    OutValues.Add(TEXT("LumenReflectionQuality"), Settings.LumenReflectionQuality);
    OutValues.Add(TEXT("IndirectLightingIntensity"), Settings.IndirectLightingIntensity);
}

static uint32 ComputeShowFlagsHash(const FEngineShowFlags& Flags) {
    FString Buffer;
    Buffer.Reserve(128);
    Buffer += Flags.PostProcessing ? TEXT("PP1;") : TEXT("PP0;");
    Buffer += Flags.Lighting ? TEXT("L1;") : TEXT("L0;");
    Buffer += Flags.Bloom ? TEXT("B1;") : TEXT("B0;");
    Buffer += Flags.Tonemapper ? TEXT("T1;") : TEXT("T0;");
    Buffer += Flags.MotionBlur ? TEXT("MB1;") : TEXT("MB0;");
    Buffer += Flags.ScreenSpaceReflections ? TEXT("SSR1;") : TEXT("SSR0;");
    Buffer += Flags.DynamicShadows ? TEXT("DS1;") : TEXT("DS0;");
    Buffer += Flags.VisualizeBuffer ? TEXT("VB1;") : TEXT("VB0;");
    return FCrc::StrCrc32(*Buffer);
}

}

FTajsGraphDebugViewExtension::FTajsGraphDebugViewExtension(const FAutoRegister& AutoRegister, TWeakObjectPtr<UTajsGraphDebugSubsystem> InOwnerSubsystem)
    : FSceneViewExtensionBase(AutoRegister)
    , OwnerSubsystem(InOwnerSubsystem) {
    PendingSamples.Reserve(64);
}

void FTajsGraphDebugViewExtension::SetupViewFamily(FSceneViewFamily& InViewFamily) {
}

void FTajsGraphDebugViewExtension::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) {
}

void FTajsGraphDebugViewExtension::BeginRenderViewFamily(FSceneViewFamily& InViewFamily) {
    FTajsGraphRenderStateSample Sample = BuildRenderStateSample(InViewFamily);

    FScopeLock Lock(&QueueMutex);
    constexpr int32 MaxQueuedSamples = 512;
    if (PendingSamples.Num() >= MaxQueuedSamples) {
        PendingSamples.RemoveAt(0, PendingSamples.Num() - MaxQueuedSamples + 1, false);
    }
    PendingSamples.Add(MoveTemp(Sample));
}

bool FTajsGraphDebugViewExtension::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const {
    return OwnerSubsystem.IsValid() && OwnerSubsystem->IsDebugEnabled();
}

void FTajsGraphDebugViewExtension::DrainRenderStateSamples(TArray<FTajsGraphRenderStateSample>& OutSamples) {
    FScopeLock Lock(&QueueMutex);
    OutSamples.Reset(PendingSamples.Num());
    OutSamples.Append(PendingSamples);
    PendingSamples.Reset();
}

FTajsGraphRenderStateSample FTajsGraphDebugViewExtension::BuildRenderStateSample(const FSceneViewFamily& InViewFamily) const {
    FTajsGraphRenderStateSample Out;
    Out.Timestamp = FPlatformTime::Seconds();

    if (const FSceneView* View = InViewFamily.Views.Num() > 0 ? InViewFamily.Views[0] : nullptr) {
        Out.ViewRectX = View->UnscaledViewRect.Min.X;
        Out.ViewRectY = View->UnscaledViewRect.Min.Y;
        Out.ViewRectW = View->UnscaledViewRect.Width();
        Out.ViewRectH = View->UnscaledViewRect.Height();
        Out.ResX = View->UnscaledViewRect.Width();
        Out.ResY = View->UnscaledViewRect.Height();
        Out.FeatureLevel = LexToString(View->FeatureLevel);
        CaptureWatchedPostProcessValues(View->FinalPostProcessSettings, Out.FinalPPWatchedValues);

        for (const FWeightedBlendable& Blendable : View->FinalPostProcessSettings.WeightedBlendables.Array) {
            if (Blendable.Object) {
                Out.ActiveBlendables.Add(Blendable.Object->GetName());
            }
        }
    }

    Out.ShowFlagsHash = ComputeShowFlagsHash(InViewFamily.EngineShowFlags);

    Out.ScreenPercentage = GetCVarFloatValue(TEXT("r.ScreenPercentage"), 100.0f);
    Out.bUpscalerActive = GetCVarIntValue(TEXT("r.TemporalAA.Upsampling"), 0) != 0 || GetCVarIntValue(TEXT("r.AntiAliasingMethod"), 0) == 4;
    Out.bLumenActive = GetCVarIntValue(TEXT("r.Lumen.DiffuseIndirect.Allow"), 0) != 0 || GetCVarIntValue(TEXT("r.Lumen.Reflections.Allow"), 0) != 0;
    Out.bNaniteEnabled = GetCVarIntValue(TEXT("r.Nanite"), 0) != 0;
    Out.bVSMEnabled = GetCVarIntValue(TEXT("r.Shadow.Virtual.Enable"), 0) != 0;

    Out.RHIName = GDynamicRHI ? FString(GDynamicRHI->GetName()) : TEXT("Unknown");
    Out.ShaderModel = FString::Printf(TEXT("SM%d"), GetCVarIntValue(TEXT("r.ShaderModel"), 0));

    Out.WindowMode = TEXT("Unknown");
    if (UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr) {
        switch (Settings->GetFullscreenMode()) {
        case EWindowMode::Fullscreen:
            Out.WindowMode = TEXT("Fullscreen");
            break;
        case EWindowMode::WindowedFullscreen:
            Out.WindowMode = TEXT("WindowedFullscreen");
            break;
        case EWindowMode::Windowed:
            Out.WindowMode = TEXT("Windowed");
            break;
        default:
            break;
        }
    }

    return Out;
}

