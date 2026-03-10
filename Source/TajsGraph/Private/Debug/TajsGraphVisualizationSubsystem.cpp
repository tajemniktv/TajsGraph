#include "Debug/TajsGraphVisualizationSubsystem.h"

#include "TajsGraphModule.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ShowFlags.h"

namespace {

static bool SetCVarStringWithBackup(const TCHAR* Name, const FString& Value, TMap<FString, FString>& BackupStore) {
    IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name);
    if (!CVar) {
        return false;
    }

    const FString Key(Name);
    if (!BackupStore.Contains(Key)) {
        BackupStore.Add(Key, CVar->GetString());
    }
    CVar->Set(*Value, ECVF_SetByCode);
    return true;
}

static bool SetCVarIntWithBackup(const TCHAR* Name, const int32 Value, TMap<FString, FString>& BackupStore) {
    IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name);
    if (!CVar) {
        return false;
    }

    const FString Key(Name);
    if (!BackupStore.Contains(Key)) {
        BackupStore.Add(Key, CVar->GetString());
    }

    CVar->Set(Value, ECVF_SetByCode);
    return true;
}

static UWorld* ResolveWorldFromSubsystem(const UGameInstanceSubsystem* Subsystem) {
    if (!Subsystem) {
        return nullptr;
    }

    if (UGameInstance* GI = Subsystem->GetGameInstance()) {
        return GI->GetWorld();
    }

    return nullptr;
}

static UGameViewportClient* ResolveGameViewport(const UGameInstanceSubsystem* Subsystem) {
    if (GEngine && GEngine->GameViewport) {
        return GEngine->GameViewport;
    }

    if (UWorld* World = ResolveWorldFromSubsystem(Subsystem)) {
        if (UGameViewportClient* GameViewport = World->GetGameViewport()) {
            return GameViewport;
        }
    }

    return nullptr;
}

static ETajsGraphVisMode WrapMode(const int32 RawMode) {
    constexpr int32 ModeCount = static_cast<int32>(ETajsGraphVisMode::ShaderComplexity) + 1;
    const int32 Normalized = (RawMode % ModeCount + ModeCount) % ModeCount;
    return static_cast<ETajsGraphVisMode>(Normalized);
}

static void LoadVisualizationConfigBool(const TCHAR* Key, const bool FallbackValue, bool& OutValue) {
    bool SettingValue = FallbackValue;
    if (FTajsGraphModule::GetBoolSetting(TEXT("Visualization"), Key, SettingValue)) {
        OutValue = SettingValue;
        return;
    }

    OutValue = FallbackValue;
}

static void LoadVisualizationConfigInt(const TCHAR* Key, const int32 FallbackValue, int32& OutValue) {
    int32 SettingValue = FallbackValue;
    if (FTajsGraphModule::GetIntSetting(TEXT("Visualization"), Key, SettingValue)) {
        OutValue = SettingValue;
        return;
    }

    OutValue = FallbackValue;
}

static bool MapModeToEngineView(const ETajsGraphVisMode InMode, EViewModeIndex& OutViewMode, FString& OutBufferTarget) {
    OutBufferTarget.Reset();

    switch (InMode) {
    case ETajsGraphVisMode::Lit:
        OutViewMode = VMI_Lit;
        return true;
    case ETajsGraphVisMode::Unlit:
        OutViewMode = VMI_Unlit;
        return true;
    case ETajsGraphVisMode::Wireframe:
        OutViewMode = VMI_Wireframe;
        return true;
    case ETajsGraphVisMode::DetailLighting:
        OutViewMode = VMI_Lit_DetailLighting;
        return true;
    case ETajsGraphVisMode::LightingOnly:
        OutViewMode = VMI_LightingOnly;
        return true;
    case ETajsGraphVisMode::Reflections:
        OutViewMode = VMI_ReflectionOverride;
        return true;
    case ETajsGraphVisMode::ShaderComplexity:
        OutViewMode = VMI_ShaderComplexity;
        return true;
    case ETajsGraphVisMode::BaseColor:
        OutViewMode = VMI_VisualizeBuffer;
        OutBufferTarget = TEXT("BaseColor");
        return true;
    case ETajsGraphVisMode::WorldNormal:
        OutViewMode = VMI_VisualizeBuffer;
        OutBufferTarget = TEXT("WorldNormal");
        return true;
    case ETajsGraphVisMode::Roughness:
        OutViewMode = VMI_VisualizeBuffer;
        OutBufferTarget = TEXT("Roughness");
        return true;
    case ETajsGraphVisMode::Metallic:
        OutViewMode = VMI_VisualizeBuffer;
        OutBufferTarget = TEXT("Metallic");
        return true;
    case ETajsGraphVisMode::Specular:
        OutViewMode = VMI_VisualizeBuffer;
        OutBufferTarget = TEXT("Specular");
        return true;
    case ETajsGraphVisMode::AmbientOcclusion:
        OutViewMode = VMI_VisualizeBuffer;
        OutBufferTarget = TEXT("AmbientOcclusion");
        return true;
    case ETajsGraphVisMode::SceneDepth:
        OutViewMode = VMI_VisualizeBuffer;
        OutBufferTarget = TEXT("SceneDepth");
        return true;
    default:
        break;
    }

    return false;
}

}

void UTajsGraphVisualizationSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
    Super::Initialize(Collection);
    RefreshVisualizationConfig();
}

void UTajsGraphVisualizationSubsystem::Deinitialize() {
    RestorePreviousState();
    Super::Deinitialize();
}

void UTajsGraphVisualizationSubsystem::ReloadVisualizationConfig() {
    if (bEnabled) {
        ApplyCurrentMode();
        return;
    }

    RefreshVisualizationConfig();
}

bool UTajsGraphVisualizationSubsystem::SetVisualizationMode(const ETajsGraphVisMode InMode) {
    RefreshVisualizationConfig();
    if (bEnabled && !IsModeSwitchAllowed(InMode)) {
        UE_LOG(LogTajsGraph, Verbose, TEXT("[TajsGraph][Visualization] Ignored mode switch due to cooldown (%d ms): %s"),
            ModeSwitchCooldownMs,
            *StaticEnum<ETajsGraphVisMode>()->GetNameStringByValue(static_cast<int64>(InMode)));
        return false;
    }

    CurrentMode = InMode;
    if (bEnabled) {
        const bool bApplied = ApplyCurrentMode();
        if (bApplied) {
            LastModeSwitchTimeSeconds = FPlatformTime::Seconds();
        }
        OnVisualizationModeChanged.Broadcast(CurrentMode);
        return bApplied;
    }

    OnVisualizationModeChanged.Broadcast(CurrentMode);
    return true;
}

void UTajsGraphVisualizationSubsystem::SetVisualizationEnabled(const bool bInEnabled) {
    if (bEnabled == bInEnabled) {
        return;
    }

    RefreshVisualizationConfig();
    if (bInEnabled && !IsModeSwitchAllowed(CurrentMode)) {
        UE_LOG(LogTajsGraph, Verbose, TEXT("[TajsGraph][Visualization] Ignored enable request due to cooldown (%d ms)."), ModeSwitchCooldownMs);
        return;
    }

    bEnabled = bInEnabled;
    if (bEnabled) {
        if (ApplyCurrentMode()) {
            LastModeSwitchTimeSeconds = FPlatformTime::Seconds();
        }
    } else {
        RestorePreviousState();
        LastAppliedMode = ETajsGraphVisMode::Default;
    }
}

bool UTajsGraphVisualizationSubsystem::ToggleVisualizationEnabled() {
    SetVisualizationEnabled(!bEnabled);
    return bEnabled;
}

ETajsGraphVisMode UTajsGraphVisualizationSubsystem::NextMode() {
    CurrentMode = WrapMode(static_cast<int32>(CurrentMode) + 1);
    SetVisualizationMode(CurrentMode);
    return CurrentMode;
}

ETajsGraphVisMode UTajsGraphVisualizationSubsystem::PreviousMode() {
    CurrentMode = WrapMode(static_cast<int32>(CurrentMode) - 1);
    SetVisualizationMode(CurrentMode);
    return CurrentMode;
}

bool UTajsGraphVisualizationSubsystem::ApplyCurrentMode() {
    RestorePreviousState();
    RefreshVisualizationConfig();

    if (CurrentMode == ETajsGraphVisMode::Default) {
        LastAppliedMode = ETajsGraphVisMode::Default;
        return true;
    }

    if (bEnableEngineViewAttempt && TryApplyEngineViewMode(CurrentMode)) {
        LastAppliedMode = CurrentMode;
        return true;
    }

    if (bEnableRuntimeVisCVars && TryApplyRuntimeVisualizationCVars(CurrentMode)) {
        LastAppliedMode = CurrentMode;
        return true;
    }

    if (bEnablePostProcessFallback && TryApplyPostProcessMaterial(CurrentMode)) {
        LastAppliedMode = CurrentMode;
        return true;
    }

    return false;
}

void UTajsGraphVisualizationSubsystem::RefreshVisualizationConfig() {
    LoadVisualizationConfigBool(TEXT("EnableEngineViewAttempt"), true, bEnableEngineViewAttempt);
    LoadVisualizationConfigBool(TEXT("EnableRuntimeVisCVars"), true, bEnableRuntimeVisCVars);
    LoadVisualizationConfigBool(TEXT("EnablePostProcessFallback"), true, bEnablePostProcessFallback);

    int32 LoadedCooldownMs = 100;
    LoadVisualizationConfigInt(TEXT("ModeSwitchCooldownMs"), 100, LoadedCooldownMs);
    ModeSwitchCooldownMs = FMath::Clamp(LoadedCooldownMs, 0, 5000);
}

bool UTajsGraphVisualizationSubsystem::IsModeSwitchAllowed(const ETajsGraphVisMode InMode) const {
    if (ModeSwitchCooldownMs <= 0 || InMode == LastAppliedMode) {
        return true;
    }

    const double Elapsed = FPlatformTime::Seconds() - LastModeSwitchTimeSeconds;
    return Elapsed >= (static_cast<double>(ModeSwitchCooldownMs) / 1000.0);
}

void UTajsGraphVisualizationSubsystem::RestorePreviousState() {
    if (UGameViewportClient* Viewport = ResolveGameViewport(this)) {
        if (bHasSavedViewportViewMode && SavedViewportViewModeIndex != INDEX_NONE) {
            Viewport->ViewModeIndex = SavedViewportViewModeIndex;
            if (FEngineShowFlags* ShowFlags = Viewport->GetEngineShowFlags()) {
                ApplyViewMode(static_cast<EViewModeIndex>(SavedViewportViewModeIndex), true, *ShowFlags);
            }
        }
    }
    bHasSavedViewportViewMode = false;
    SavedViewportViewModeIndex = INDEX_NONE;

    for (const TPair<FString, FString>& Pair : SavedCVarValues) {
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*Pair.Key)) {
            CVar->Set(*Pair.Value, ECVF_SetByCode);
        }
    }
    SavedCVarValues.Reset();

    if (APostProcessVolume* PPVolume = InjectedPPVolume.Get()) {
        PPVolume->Settings.WeightedBlendables.Array.Reset();
        PPVolume->BlendWeight = 0.0f;
    }

    ActiveMID.Reset();
    ActiveMaterial.Reset();
}

bool UTajsGraphVisualizationSubsystem::TryApplyEngineViewMode(const ETajsGraphVisMode InMode) {
    UGameViewportClient* Viewport = ResolveGameViewport(this);
    if (!Viewport) {
        return false;
    }

    EViewModeIndex TargetViewMode = VMI_Unknown;
    FString BufferTarget;
    if (!MapModeToEngineView(InMode, TargetViewMode, BufferTarget)) {
        return false;
    }

    if (!bHasSavedViewportViewMode) {
        bHasSavedViewportViewMode = true;
        SavedViewportViewModeIndex = Viewport->ViewModeIndex;
    }

    if (!BufferTarget.IsEmpty()) {
        if (!SetCVarStringWithBackup(TEXT("r.BufferVisualizationTarget"), BufferTarget, SavedCVarValues)) {
            return false;
        }
    }

    Viewport->ViewModeIndex = static_cast<int32>(TargetViewMode);
    if (FEngineShowFlags* ShowFlags = Viewport->GetEngineShowFlags()) {
        ApplyViewMode(TargetViewMode, true, *ShowFlags);
    }
    return true;
}

bool UTajsGraphVisualizationSubsystem::TryApplyRuntimeVisualizationCVars(const ETajsGraphVisMode InMode) {
    switch (InMode) {
    case ETajsGraphVisMode::LumenCards:
        return SetCVarIntWithBackup(TEXT("r.Lumen.Visualize.CardPlacement"), 1, SavedCVarValues);
    case ETajsGraphVisMode::MeshDistanceFields:
        return SetCVarIntWithBackup(TEXT("r.VisualizeMeshDistanceFields"), 1, SavedCVarValues);
    case ETajsGraphVisMode::GlobalDistanceField:
        return SetCVarIntWithBackup(TEXT("r.AOGlobalDistanceFieldVisualize"), 1, SavedCVarValues);
    default:
        break;
    }

    return false;
}

bool UTajsGraphVisualizationSubsystem::TryApplyPostProcessMaterial(const ETajsGraphVisMode InMode) {
    UMaterialInterface* Material = ResolveMaterialForMode(InMode);
    if (!Material) {
        return false;
    }

    EnsurePostProcessVolume();
    APostProcessVolume* PPVolume = InjectedPPVolume.Get();
    if (!PPVolume) {
        return false;
    }

    UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(Material, this);
    if (!MID) {
        return false;
    }

    PPVolume->bEnabled = true;
    PPVolume->bUnbound = true;
    PPVolume->Priority = 1000000.0f;
    PPVolume->BlendWeight = 1.0f;
    PPVolume->Settings.WeightedBlendables.Array.Reset();
    PPVolume->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.0f, MID));

    ActiveMID = MID;
    ActiveMaterial = Material;
    return true;
}

void UTajsGraphVisualizationSubsystem::EnsurePostProcessVolume() {
    if (InjectedPPVolume.IsValid()) {
        return;
    }

    UWorld* World = ResolveWorldFromSubsystem(this);
    if (!World) {
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = TEXT("TajsGraph_DebugVisualizationPPV");
    SpawnParams.ObjectFlags |= RF_Transient;

    APostProcessVolume* Volume = World->SpawnActor<APostProcessVolume>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    if (Volume) {
        Volume->bUnbound = true;
        Volume->Priority = 1000000.0f;
        Volume->BlendWeight = 0.0f;
        InjectedPPVolume = Volume;
    }
}

UMaterialInterface* UTajsGraphVisualizationSubsystem::ResolveMaterialForMode(const ETajsGraphVisMode InMode) const {
    FString PreferredMaterialPath;
    FString FallbackMaterialPath;
    switch (InMode) {
    case ETajsGraphVisMode::BaseColor:
        PreferredMaterialPath = TEXT("/TajsGraph/Debug/BufferVisualization/BaseColor.BaseColor");
        FallbackMaterialPath = TEXT("/Engine/BufferVisualization/BaseColor.BaseColor");
        break;
    case ETajsGraphVisMode::WorldNormal:
        PreferredMaterialPath = TEXT("/TajsGraph/Debug/BufferVisualization/WorldNormal.WorldNormal");
        FallbackMaterialPath = TEXT("/Engine/BufferVisualization/WorldNormal.WorldNormal");
        break;
    case ETajsGraphVisMode::Roughness:
        PreferredMaterialPath = TEXT("/TajsGraph/Debug/BufferVisualization/Roughness.Roughness");
        FallbackMaterialPath = TEXT("/Engine/BufferVisualization/Roughness.Roughness");
        break;
    case ETajsGraphVisMode::Metallic:
        PreferredMaterialPath = TEXT("/TajsGraph/Debug/BufferVisualization/Metallic.Metallic");
        FallbackMaterialPath = TEXT("/Engine/BufferVisualization/Metallic.Metallic");
        break;
    case ETajsGraphVisMode::Specular:
        PreferredMaterialPath = TEXT("/TajsGraph/Debug/BufferVisualization/Specular.Specular");
        FallbackMaterialPath = TEXT("/Engine/BufferVisualization/Specular.Specular");
        break;
    case ETajsGraphVisMode::AmbientOcclusion:
        PreferredMaterialPath = TEXT("/TajsGraph/Debug/BufferVisualization/AmbientOcclusion.AmbientOcclusion");
        FallbackMaterialPath = TEXT("/Engine/BufferVisualization/AmbientOcclusion.AmbientOcclusion");
        break;
    case ETajsGraphVisMode::SceneDepth:
        PreferredMaterialPath = TEXT("/TajsGraph/Debug/BufferVisualization/SceneDepth.SceneDepth");
        FallbackMaterialPath = TEXT("/Engine/BufferVisualization/SceneDepth.SceneDepth");
        break;
    case ETajsGraphVisMode::LuminanceHeatmap:
        PreferredMaterialPath = TEXT("/TajsGraph/Debug/Materials/M_TajsGraph_Vis_LuminanceHeatmap.M_TajsGraph_Vis_LuminanceHeatmap");
        break;
    case ETajsGraphVisMode::PseudoWireframe:
    case ETajsGraphVisMode::Wireframe:
        PreferredMaterialPath = TEXT("/TajsGraph/Debug/Materials/M_TajsGraph_Vis_PseudoWireframe.M_TajsGraph_Vis_PseudoWireframe");
        break;
    default:
        break;
    }

    if (PreferredMaterialPath.IsEmpty()) {
        return nullptr;
    }

    if (UMaterialInterface* Preferred = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *PreferredMaterialPath))) {
        return Preferred;
    }

    if (!FallbackMaterialPath.IsEmpty()) {
        return Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *FallbackMaterialPath));
    }

    return nullptr;
}
