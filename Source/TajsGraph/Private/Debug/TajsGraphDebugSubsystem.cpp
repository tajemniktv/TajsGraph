#include "Debug/TajsGraphDebugSubsystem.h"

#include "Debug/TajsGraphDebugIO.h"
#include "Debug/TajsGraphDebugCommands.h"
#include "Debug/TajsGraphDebugViewExtension.h"
#include "Debug/TajsGraphVisualizationSubsystem.h"
#include "Dom/JsonObject.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "DynamicRHI.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "FGPlayerController.h"
#include "UI/FGGameUI.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformMemory.h"
#include "HAL/PlatformTime.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"
#include "Misc/App.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "UObject/SoftObjectPath.h"
#include "SceneViewExtension.h"
#include "TajsGraphModule.h"
#include "UI/TajsGraphDebugOverlayWidget.h"
#include "UI/TajsGraphDebugMenuWidget.h"

#include "Components/BrushComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/LocalLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Light.h"
#include "Engine/PointLight.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/RectLight.h"
#include "Engine/SpotLight.h"
#include "Engine/StaticMesh.h"

namespace
{

    static UWorld *ResolveWorld(const UGameInstanceSubsystem *Subsystem)
    {
        if (!Subsystem)
        {
            return nullptr;
        }

        if (UGameInstance *GI = Subsystem->GetGameInstance())
        {
            return GI->GetWorld();
        }

        return nullptr;
    }

        static FString TimestampString()
    {
        return FDateTime::UtcNow().ToString(TEXT("%Y%m%d_%H%M%S"));
    }

    static UClass* ResolveHubWidgetClass()
    {
        FString ClassPath;
        FTajsGraphModule::GetStringSetting(TEXT("DebugHub"), TEXT("HubWidgetClass"), ClassPath);
        ClassPath = ClassPath.TrimStartAndEnd();
        if (ClassPath.IsEmpty())
        {
            UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph][DebugHub] HubWidgetClass is not configured. The debug hub now requires a BP widget class."));
            return nullptr;
        }

        FSoftClassPath SoftClassPath(ClassPath);
        if (UClass* LoadedClass = SoftClassPath.TryLoadClass<UTajsGraphDebugMenuWidget>())
        {
            return LoadedClass;
        }

        UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph][DebugHub] Failed to load HubWidgetClass '%s'. No debug hub will be shown."), *ClassPath);
        return nullptr;
    }

    static float Percentile(const TArray<float> &SortedValues, const float P)
    {
        if (SortedValues.Num() <= 0)
        {
            return 0.0f;
        }

        const float Clamped = FMath::Clamp(P, 0.0f, 1.0f);
        const float FloatIndex = Clamped * static_cast<float>(SortedValues.Num() - 1);
        const int32 Index0 = FMath::Clamp(FMath::FloorToInt(FloatIndex), 0, SortedValues.Num() - 1);
        const int32 Index1 = FMath::Clamp(Index0 + 1, 0, SortedValues.Num() - 1);
        const float Alpha = FloatIndex - static_cast<float>(Index0);
        return FMath::Lerp(SortedValues[Index0], SortedValues[Index1], Alpha);
    }

    static uint32 BuildHitchSignature(const FTajsGraphRenderStateSample &RenderState, const FTajsGraphSceneProbeSample &Probe)
    {
        const FString Signature = FString::Printf(TEXT("%s|%s|%d|%d|%.2f|%d|%d|%.2f"),
                                                  *RenderState.RHIName,
                                                  *RenderState.FeatureLevel,
                                                  RenderState.ShowFlagsHash,
                                                  RenderState.bLumenActive ? 1 : 0,
                                                  RenderState.ScreenPercentage,
                                                  Probe.NearbyLights50m,
                                                  Probe.NearbyShadowCasters,
                                                  Probe.OverlapScore);
        return FCrc::StrCrc32(*Signature);
    }

    static FString KeyEnumToString(const ETajsGraphOverlayPage Page)
    {
        switch (Page)
        {
        case ETajsGraphOverlayPage::Perf:
            return TEXT("Perf");
        case ETajsGraphOverlayPage::Renderer:
            return TEXT("Renderer");
        case ETajsGraphOverlayPage::PostProcess:
            return TEXT("PostProcess");
        case ETajsGraphOverlayPage::Lights:
            return TEXT("Lights");
        case ETajsGraphOverlayPage::Warnings:
            return TEXT("Warnings");
        default:
            return TEXT("Unknown");
        }
    }

}

void UTajsGraphDebugSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
    FTajsGraphDebugCommands::Startup();

    RefreshConfigFromSettings();
    bConfigRefreshRequested = false;
    LastConfigRefreshTimeSeconds = FPlatformTime::Seconds();
    PerfRing.Reset(MaxPerfSamples);
    RenderRing.Reset(MaxRenderSamples);
    ProbeRing.Reset(MaxProbeSamples);
    EventRing.Reset(MaxEvents);

    SettingChangedHandle = FTajsGraphModule::OnSettingChanged().AddUObject(this, &UTajsGraphDebugSubsystem::HandleSettingChanged);
    TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UTajsGraphDebugSubsystem::Tick), 0.0f);

    VisualizationSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UTajsGraphVisualizationSubsystem>() : nullptr;
}

void UTajsGraphDebugSubsystem::Deinitialize()
{
    if (TickHandle.IsValid())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
        TickHandle.Reset();
    }

    if (SettingChangedHandle.IsValid())
    {
        FTajsGraphModule::OnSettingChanged().Remove(SettingChangedHandle);
        SettingChangedHandle.Reset();
    }

    ApplyOverlayVisibility(false, false);
    ApplyMenuVisibility(false);
    OverlayWidget.Reset();
    MenuWidget.Reset();
    ViewExtension.Reset();
    FTajsGraphDebugCommands::Shutdown();
    Super::Deinitialize();
}

void UTajsGraphDebugSubsystem::ReloadConfigFromSettings()
{
    RefreshConfigFromSettings();
    bConfigRefreshRequested = false;
    LastConfigRefreshTimeSeconds = FPlatformTime::Seconds();
    EnsureViewExtensionState();
}

void UTajsGraphDebugSubsystem::SetDebugEnabled(const bool bEnabled)
{
    if (bDebugEnabled == bEnabled)
    {
        return;
    }

    bDebugEnabled = bEnabled;
    FTajsGraphModule::SetBoolSetting(TEXT("DebugHub"), TEXT("Enabled"), bEnabled, false);
    if (!bDebugEnabled)
    {
        ApplyOverlayVisibility(false, false);
        ApplyMenuVisibility(false);
        if (!VisualizationSubsystem.IsValid())
        {
            VisualizationSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UTajsGraphVisualizationSubsystem>() : nullptr;
        }
        if (UTajsGraphVisualizationSubsystem *Vis = VisualizationSubsystem.Get())
        {
            Vis->SetVisualizationEnabled(false);
        }
    }
    EnsureViewExtensionState();
    RecordEvent(TEXT("Debug"), bDebugEnabled ? TEXT("Enabled") : TEXT("Disabled"), {});
    OnDebugStateChanged.Broadcast(bDebugEnabled);
}

void UTajsGraphDebugSubsystem::RecordEvent(const FString &Category, const FString &Message, const TMap<FString, FString> &KeyValues)
{
    FTajsGraphDebugEvent Event;
    Event.Timestamp = FPlatformTime::Seconds();
    Event.Category = Category;
    Event.Message = Message;
    Event.KeyValues = KeyValues;
    EventRing.Push(Event);
}

void UTajsGraphDebugSubsystem::RecordSample()
{
    CollectPerfSample(0.0f);
    FTajsGraphSceneProbeSample Probe = RunSceneProbe();
    ProbeRing.Push(Probe);
}

FTajsGraphDebugSnapshot UTajsGraphDebugSubsystem::CreateSnapshot(const FString &Reason, float IncludeHistorySeconds)
{
    FTajsGraphDebugSnapshot Snapshot;
    Snapshot.SnapshotId = FString::Printf(TEXT("%s_%s"), *Reason, *TimestampString());
    Snapshot.Reason = Reason;
    Snapshot.Timestamp = FPlatformTime::Seconds();

    if (!RenderRing.TryGetLatest(Snapshot.CurrentRenderState))
    {
        Snapshot.CurrentRenderState = BuildBasicRenderStateSample();
    }

    if (!ProbeRing.TryGetLatest(Snapshot.CurrentProbe))
    {
        Snapshot.CurrentProbe = RunSceneProbe();
    }

    const double MinTimestamp = Snapshot.Timestamp - FMath::Max(0.0f, IncludeHistorySeconds);
    PerfRing.ToArraySince(MinTimestamp, Snapshot.PerfHistory);
    RenderRing.ToArraySince(MinTimestamp, Snapshot.RenderHistory);
    ProbeRing.ToArraySince(MinTimestamp, Snapshot.ProbeHistory);

    TArray<FTajsGraphDebugEvent> Events;
    EventRing.ToArray(Events);
    for (const FTajsGraphDebugEvent &Event : Events)
    {
        if (Event.Timestamp >= MinTimestamp)
        {
            Snapshot.RecentEvents.Add(Event);
        }
    }

    Snapshot.Metadata.Add(TEXT("overlay_page"), KeyEnumToString(OverlayPage));
    Snapshot.Metadata.Add(TEXT("debug_enabled"), bDebugEnabled ? TEXT("true") : TEXT("false"));
    if (!bDebugEnabled)
    {
        Snapshot.Metadata.Add(TEXT("capture_mode"), TEXT("basic_on_demand"));
    }

    if (Snapshot.PerfHistory.Num() <= 0)
    {
        Snapshot.PerfHistory.Add(BuildBasicPerfSample());
    }
    if (Snapshot.RenderHistory.Num() <= 0)
    {
        Snapshot.RenderHistory.Add(Snapshot.CurrentRenderState);
    }
    if (Snapshot.ProbeHistory.Num() <= 0)
    {
        Snapshot.ProbeHistory.Add(Snapshot.CurrentProbe);
    }

    if (bHasBaselineRenderState)
    {
        Snapshot.PostProcessDiff = ComputePostProcessDiff(BaselineRenderState, Snapshot.CurrentRenderState);
    }
    Snapshot.PostProcessProvenance = BuildPostProcessProvenance(Snapshot.CurrentRenderState);

    return Snapshot;
}

bool UTajsGraphDebugSubsystem::WriteReport(const FTajsGraphDebugSnapshot &Snapshot, const FString &OutputDirectory)
{
    const FString RootDir = OutputDirectory.IsEmpty() ? FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("TajsGraph"), TEXT("Reports"), FString::Printf(TEXT("Report_%s"), *TimestampString())) : OutputDirectory;
    IFileManager::Get().MakeDirectory(*RootDir, true);

    const FString SnapshotPath = FPaths::Combine(RootDir, TEXT("report.json"));
    if (!FTajsGraphDebugIO::WriteSnapshotJson(Snapshot, SnapshotPath))
    {
        return false;
    }

    {
        TSharedPtr<FJsonObject> Manifest = MakeShared<FJsonObject>();
        Manifest->SetStringField(TEXT("report_version"), TEXT("1"));
        Manifest->SetStringField(TEXT("generated_utc"), FDateTime::UtcNow().ToString(TEXT("%Y-%m-%dT%H:%M:%SZ")));
        Manifest->SetStringField(TEXT("snapshot_file"), TEXT("report.json"));
        Manifest->SetBoolField(TEXT("expanded_artifacts"), bWriteExpandedReportArtifacts);
        FTajsGraphDebugIO::WriteJsonObject(Manifest, FPaths::Combine(RootDir, TEXT("manifest.json")));
    }

    if (bWriteExpandedReportArtifacts)
    {
        FTajsGraphDebugIO::WriteJsonObject(FTajsGraphDebugIO::MakeRenderStateJson(Snapshot.CurrentRenderState), FPaths::Combine(RootDir, TEXT("render_state.json")));
        FTajsGraphDebugIO::WriteJsonObject(FTajsGraphDebugIO::MakeSceneProbeJson(Snapshot.CurrentProbe), FPaths::Combine(RootDir, TEXT("scene_probe.json")));

        {
            TSharedPtr<FJsonObject> PostProcessFinal = MakeShared<FJsonObject>();
            for (const TPair<FString, float> &Pair : Snapshot.CurrentRenderState.FinalPPWatchedValues)
            {
                PostProcessFinal->SetNumberField(Pair.Key, Pair.Value);
            }
            FTajsGraphDebugIO::WriteJsonObject(PostProcessFinal, FPaths::Combine(RootDir, TEXT("postprocess_final.json")));
        }

        {
            TSharedPtr<FJsonObject> PostProcessDiff = MakeShared<FJsonObject>();
            TArray<TSharedPtr<FJsonValue>> Entries;
            for (const FTajsGraphPPDiffEntry &Entry : Snapshot.PostProcessDiff)
            {
                TSharedPtr<FJsonObject> Item = MakeShared<FJsonObject>();
                Item->SetStringField(TEXT("field"), Entry.Field);
                Item->SetNumberField(TEXT("baseline"), Entry.Baseline);
                Item->SetNumberField(TEXT("current"), Entry.Current);
                Item->SetNumberField(TEXT("delta"), Entry.Delta);
                Entries.Add(MakeShared<FJsonValueObject>(Item));
            }
            PostProcessDiff->SetArrayField(TEXT("changes"), Entries);
            FTajsGraphDebugIO::WriteJsonObject(PostProcessDiff, FPaths::Combine(RootDir, TEXT("postprocess_diff.json")));
        }

        {
            TSharedPtr<FJsonObject> Provenance = MakeShared<FJsonObject>();
            TArray<TSharedPtr<FJsonValue>> Entries;
            for (const FTajsGraphPPProvenanceEntry &Entry : Snapshot.PostProcessProvenance)
            {
                TSharedPtr<FJsonObject> Item = MakeShared<FJsonObject>();
                Item->SetStringField(TEXT("field"), Entry.Field);
                Item->SetNumberField(TEXT("requested"), Entry.RequestedValue);
                Item->SetNumberField(TEXT("final"), Entry.FinalValue);
                Item->SetStringField(TEXT("last_writer_guess"), Entry.LastWriterGuess);
                Item->SetNumberField(TEXT("confidence"), Entry.Confidence);
                Item->SetBoolField(TEXT("inferred"), Entry.bInferred);
                Entries.Add(MakeShared<FJsonValueObject>(Item));
            }
            Provenance->SetArrayField(TEXT("entries"), Entries);
            FTajsGraphDebugIO::WriteJsonObject(Provenance, FPaths::Combine(RootDir, TEXT("postprocess_provenance.json")));
        }

        FTajsGraphDebugIO::WritePerfCsv(Snapshot.PerfHistory, FPaths::Combine(RootDir, TEXT("perf_history.csv")));

        {
            TSharedPtr<FJsonObject> EventsJson = MakeShared<FJsonObject>();
            TArray<TSharedPtr<FJsonValue>> Values;
            for (const FTajsGraphDebugEvent &Event : Snapshot.RecentEvents)
            {
                Values.Add(MakeShared<FJsonValueObject>(FTajsGraphDebugIO::MakeEventJson(Event)));
            }
            EventsJson->SetArrayField(TEXT("events"), Values);
            FTajsGraphDebugIO::WriteJsonObject(EventsJson, FPaths::Combine(RootDir, TEXT("events.json")));
        }

        {
            TSharedPtr<FJsonObject> SystemJson = MakeShared<FJsonObject>();
            const FPlatformMemoryStats MemStats = FPlatformMemory::GetStats();
            SystemJson->SetStringField(TEXT("platform"), FPlatformProperties::PlatformName());
            SystemJson->SetNumberField(TEXT("physical_ram_mb"), static_cast<double>(MemStats.TotalPhysical) / (1024.0 * 1024.0));
            SystemJson->SetStringField(TEXT("rhi"), Snapshot.CurrentRenderState.RHIName);
            SystemJson->SetNumberField(TEXT("res_x"), Snapshot.CurrentRenderState.ResX);
            SystemJson->SetNumberField(TEXT("res_y"), Snapshot.CurrentRenderState.ResY);
            SystemJson->SetStringField(TEXT("window_mode"), Snapshot.CurrentRenderState.WindowMode);
            FTajsGraphDebugIO::WriteJsonObject(SystemJson, FPaths::Combine(RootDir, TEXT("system.json")));
        }

        {
            TSharedPtr<FJsonObject> ConfigJson = MakeShared<FJsonObject>();
            ConfigJson->SetBoolField(TEXT("debug_enabled"), bDebugEnabled);
            ConfigJson->SetStringField(TEXT("config_path"), FTajsGraphModule::GetSettingsConfigPath());
            FTajsGraphDebugIO::WriteJsonObject(ConfigJson, FPaths::Combine(RootDir, TEXT("mod_config.json")));
        }

        if (MaxCopiedHitches > 0)
        {
            const FString HitchSourceDir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("TajsGraph"), TEXT("Reports"), TEXT("Hitches"));
            const FString HitchDestDir = FPaths::Combine(RootDir, TEXT("hitches"));
            IFileManager::Get().MakeDirectory(*HitchDestDir, true);

            TArray<FString> HitchFiles;
            IFileManager::Get().FindFiles(HitchFiles, *FPaths::Combine(HitchSourceDir, TEXT("*.json")), true, false);
            HitchFiles.Sort();

            const int32 StartIndex = FMath::Max(0, HitchFiles.Num() - MaxCopiedHitches);
            for (int32 Index = StartIndex; Index < HitchFiles.Num(); ++Index)
            {
                const FString Src = FPaths::Combine(HitchSourceDir, HitchFiles[Index]);
                const FString Dst = FPaths::Combine(HitchDestDir, HitchFiles[Index]);
                IFileManager::Get().Copy(*Dst, *Src, true, true);
            }
        }
    }

    const FString Summary = FString::Printf(
        TEXT("Snapshot: %s\nReason: %s\nTimestamp: %.6f\nPerfSamples: %d\nRenderSamples: %d\nProbeSamples: %d\nEvents: %d\nExpandedArtifacts: %s\n"),
        *Snapshot.SnapshotId,
        *Snapshot.Reason,
        Snapshot.Timestamp,
        Snapshot.PerfHistory.Num(),
        Snapshot.RenderHistory.Num(),
        Snapshot.ProbeHistory.Num(),
        Snapshot.RecentEvents.Num(),
        bWriteExpandedReportArtifacts ? TEXT("true") : TEXT("false"));
    FFileHelper::SaveStringToFile(Summary, *FPaths::Combine(RootDir, TEXT("summary.txt")));

    OnReportGenerated.Broadcast(RootDir);
    return true;
}

FString UTajsGraphDebugSubsystem::GenerateReportBundle()
{
    if (!bDebugEnabled)
    {
        RecordEvent(TEXT("Report"), TEXT("Generating basic report while DebugHub is disabled"), {});
    }
    const FTajsGraphDebugSnapshot Snapshot = CreateSnapshot(TEXT("ManualReport"), 60.0f);
    const FString OutputDir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("TajsGraph"), TEXT("Reports"), FString::Printf(TEXT("Report_%s"), *TimestampString()));
    const bool bOk = WriteReport(Snapshot, OutputDir);

    if (bOk)
    {
        UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph][Debug] Report generated at %s"), *OutputDir);
        if (UWorld *World = ResolveWorld(this))
        {
            if (AFGPlayerController *PC = Cast<AFGPlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
            {
                if (UFGGameUI *GameUI = PC->GetGameUI())
                {
                    GameUI->ShowTextNotification(FText::FromString(FString::Printf(TEXT("TajsGraph report saved: %s"), *OutputDir)));
                }
            }
        }
        TMap<FString, FString> EventData;
        EventData.Add(TEXT("path"), OutputDir);
        RecordEvent(TEXT("Report"), TEXT("Generated report bundle"), EventData);
        return OutputDir;
    }

    UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph][Debug] Failed to generate report bundle."));
    OnWarningRaised.Broadcast(TEXT("ReportWriteFailed"), TEXT("Failed to generate TajsGraph report bundle."));
    return FString();
}

FTajsGraphSceneProbeSample UTajsGraphDebugSubsystem::RunSceneProbe()
{
    FTajsGraphSceneProbeSample Probe;
    Probe.Timestamp = FPlatformTime::Seconds();

    UWorld *World = ResolveWorld(this);
    if (!World)
    {
        return Probe;
    }

    const APlayerController *PC = UGameplayStatics::GetPlayerController(World, 0);
    const APawn *Pawn = PC ? PC->GetPawn() : nullptr;
    const FVector Origin = Pawn ? Pawn->GetActorLocation() : FVector::ZeroVector;

    TArray<FTajsGraphSceneProbeOffender> Offenders;
    Offenders.Reserve(64);

    for (TActorIterator<ALight> It(World); It; ++It)
    {
        ALight *LightActor = *It;
        if (!IsValid(LightActor))
        {
            continue;
        }

        ULightComponent *LightComp = LightActor->GetLightComponent();
        if (!LightComp)
        {
            continue;
        }

        const float Distance = FVector::Dist(Origin, LightActor->GetActorLocation());
        if (Distance <= 1000.0f)
        {
            ++Probe.NearbyLights10m;
        }
        if (Distance <= 2500.0f)
        {
            ++Probe.NearbyLights25m;
        }
        if (Distance <= 5000.0f)
        {
            ++Probe.NearbyLights50m;
        }

        if (Distance <= 5000.0f && LightComp->CastShadows)
        {
            ++Probe.NearbyShadowCasters;
        }

        if (Cast<APointLight>(LightActor))
        {
            ++Probe.NearbyPointLights;
        }
        else if (Cast<ASpotLight>(LightActor))
        {
            ++Probe.NearbySpotLights;
        }
        else if (Cast<ARectLight>(LightActor))
        {
            ++Probe.NearbyRectLights;
        }

        float Radius = 0.0f;
        if (const ULocalLightComponent *LocalLight = Cast<ULocalLightComponent>(LightComp))
        {
            Radius = LocalLight->AttenuationRadius;
            if (Distance <= Radius && LightComp->CastShadows)
            {
                Probe.OverlapScore += 1.0f + (LightComp->Intensity / 10000.0f);
            }
        }

        if (Distance <= 5000.0f)
        {
            FTajsGraphSceneProbeOffender Offender;
            Offender.ActorName = LightActor->GetName();
            Offender.ActorClass = LightActor->GetClass()->GetName();
            Offender.Distance = Distance;
            Offender.Radius = Radius;
            Offender.Intensity = LightComp->Intensity;
            Offender.bCastShadows = LightComp->CastShadows;
            Offender.Score = Radius * 0.001f + (LightComp->Intensity * 0.0001f) + (LightComp->CastShadows ? 5.0f : 0.0f);
            Offenders.Add(MoveTemp(Offender));
        }
    }

    Offenders.Sort([](const FTajsGraphSceneProbeOffender &A, const FTajsGraphSceneProbeOffender &B)
                   { return A.Score > B.Score; });

    constexpr int32 MaxOffenders = 10;
    for (int32 Index = 0; Index < FMath::Min(MaxOffenders, Offenders.Num()); ++Index)
    {
        Probe.TopLights.Add(Offenders[Index]);
    }

    int32 PrimitiveCount = 0;
    int32 NaniteApprox = 0;
    for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
    {
        UPrimitiveComponent *Primitive = *It;
        if (!IsValid(Primitive) || Primitive->GetWorld() != World || !Primitive->IsRegistered())
        {
            continue;
        }

        if (FVector::DistSquared(Primitive->Bounds.Origin, Origin) > FMath::Square(5000.0f))
        {
            continue;
        }

        ++PrimitiveCount;

        if (const UStaticMeshComponent *SMC = Cast<UStaticMeshComponent>(Primitive))
        {
            if (const UStaticMesh *Mesh = SMC->GetStaticMesh())
            {
                if (Mesh->HasValidNaniteData())
                {
                    ++NaniteApprox;
                }
            }
        }
    }
    Probe.PrimitiveCountApprox = PrimitiveCount;
    Probe.NaniteMeshCountApprox = NaniteApprox;

    if (IConsoleVariable *PoolVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Streaming.PoolSize")))
    {
        Probe.TexturePoolMB = static_cast<float>(PoolVar->GetInt());
    }

    const FPlatformMemoryStats MemStats = FPlatformMemory::GetStats();
    Probe.SystemUsedMB = static_cast<float>(MemStats.UsedPhysical / (1024.0 * 1024.0));
    Probe.SystemTotalMB = static_cast<float>(MemStats.TotalPhysical / (1024.0 * 1024.0));

    return Probe;
}

void UTajsGraphDebugSubsystem::ClearHistory()
{
    PerfRing.Reset(MaxPerfSamples);
    RenderRing.Reset(MaxRenderSamples);
    ProbeRing.Reset(MaxProbeSamples);
    EventRing.Reset(MaxEvents);
    HitchSignatureCounts.Reset();
    HitchSignatureLastWriteTimes.Reset();
    bHasBaselineRenderState = false;
    RecordEvent(TEXT("Debug"), TEXT("History cleared"), {});
}

void UTajsGraphDebugSubsystem::ToggleOverlay()
{
    SetOverlayVisible(!bOverlayVisible);
}

void UTajsGraphDebugSubsystem::SetOverlayVisible(const bool bVisible)
{
    ApplyOverlayVisibility(bVisible, true);
}

void UTajsGraphDebugSubsystem::ToggleMenu()
{
    ToggleHub();
}

void UTajsGraphDebugSubsystem::OpenHub()
{
    SetMenuVisible(true);
}

void UTajsGraphDebugSubsystem::CloseHub()
{
    SetMenuVisible(false);
}

void UTajsGraphDebugSubsystem::ToggleHub()
{
    SetMenuVisible(!bMenuVisible);
}

void UTajsGraphDebugSubsystem::SetMenuVisible(const bool bVisible)
{
    ApplyMenuVisibility(bVisible);
}

void UTajsGraphDebugSubsystem::ApplyOverlayVisibility(const bool bVisible, const bool bPersistSetting)
{
    if (bOverlayVisible == bVisible)
    {
        return;
    }

    bOverlayVisible = bVisible;
    if (bPersistSetting)
    {
        FTajsGraphModule::SetBoolSetting(TEXT("DebugHub"), TEXT("OverlayEnabled"), bVisible, false);
    }

    if (bOverlayVisible && !bDebugEnabled)
    {
        bOverlayVisible = false;
    }

    UWorld *World = ResolveWorld(this);
    if (bOverlayVisible)
    {
        if (!OverlayWidget.IsValid() && World)
        {
            OverlayWidget = CreateWidget<UTajsGraphDebugOverlayWidget>(World, UTajsGraphDebugOverlayWidget::StaticClass());
        }
        if (UTajsGraphDebugOverlayWidget *Widget = OverlayWidget.Get())
        {
            if (!Widget->IsInViewport())
            {
                Widget->AddToViewport(9999);
            }
        }
    }
    else if (UTajsGraphDebugOverlayWidget *Widget = OverlayWidget.Get())
    {
        Widget->RemoveFromParent();
    }

    RecordEvent(TEXT("Overlay"), bOverlayVisible ? TEXT("Overlay shown") : TEXT("Overlay hidden"), {});
}

void UTajsGraphDebugSubsystem::ApplyMenuVisibility(const bool bVisible)
{
    if (bMenuVisible == bVisible)
    {
        return;
    }

    bMenuVisible = bVisible;

    UWorld *World = ResolveWorld(this);
    APlayerController *PlayerController = World ? UGameplayStatics::GetPlayerController(World, 0) : nullptr;
    if (bMenuVisible)
    {
                if (!MenuWidget.IsValid() && World)
        {
            if (UClass* MenuWidgetClass = ResolveHubWidgetClass())
            {
                MenuWidget = CreateWidget<UTajsGraphDebugMenuWidget>(World, MenuWidgetClass);
            }
            else
            {
                UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph][DebugHub] Cannot open hub because no valid BP widget class is configured."));
            }
        }

        if (UTajsGraphDebugMenuWidget *Widget = MenuWidget.Get())
        {
            Widget->RefreshHubData(true);
            if (!Widget->IsInViewport())
            {
                Widget->AddToViewport(10000);
            }
        }
        else
        {
            bMenuVisible = false;
        }

        if (PlayerController)
        {
            FInputModeGameAndUI InputMode;
            InputMode.SetHideCursorDuringCapture(false);
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            if (UTajsGraphDebugMenuWidget *Widget = MenuWidget.Get())
            {
                InputMode.SetWidgetToFocus(Widget->TakeWidget());
            }
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;
        }
    }
    else
    {
        if (UTajsGraphDebugMenuWidget *Widget = MenuWidget.Get())
        {
            Widget->RemoveFromParent();
        }

        if (PlayerController)
        {
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = false;
        }
    }

    RecordEvent(TEXT("Menu"), bMenuVisible ? TEXT("Menu shown") : TEXT("Menu hidden"), {});
}

void UTajsGraphDebugSubsystem::SetOverlayPage(const ETajsGraphOverlayPage InPage)
{
    OverlayPage = InPage;
    TMap<FString, FString> EventData;
    EventData.Add(TEXT("page"), KeyEnumToString(OverlayPage));
    RecordEvent(TEXT("Overlay"), TEXT("Changed overlay page"), EventData);
}

void UTajsGraphDebugSubsystem::SetVisualizationMode(const ETajsGraphVisMode Mode)
{
    if (!VisualizationSubsystem.IsValid())
    {
        VisualizationSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UTajsGraphVisualizationSubsystem>() : nullptr;
    }

    if (UTajsGraphVisualizationSubsystem *Vis = VisualizationSubsystem.Get())
    {
        Vis->SetVisualizationMode(Mode);
        Vis->SetVisualizationEnabled(Mode != ETajsGraphVisMode::Default);
        TMap<FString, FString> EventData;
        EventData.Add(TEXT("mode"), StaticEnum<ETajsGraphVisMode>()->GetNameStringByValue(static_cast<int64>(Mode)));
        RecordEvent(TEXT("Visualization"), TEXT("Set visualization mode"), EventData);
    }
}

ETajsGraphVisMode UTajsGraphDebugSubsystem::NextVisualizationMode()
{
    if (!VisualizationSubsystem.IsValid())
    {
        VisualizationSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UTajsGraphVisualizationSubsystem>() : nullptr;
    }

    if (UTajsGraphVisualizationSubsystem *Vis = VisualizationSubsystem.Get())
    {
        const ETajsGraphVisMode Mode = Vis->NextMode();
        Vis->SetVisualizationEnabled(Mode != ETajsGraphVisMode::Default);
        TMap<FString, FString> EventData;
        EventData.Add(TEXT("mode"), StaticEnum<ETajsGraphVisMode>()->GetNameStringByValue(static_cast<int64>(Mode)));
        RecordEvent(TEXT("Visualization"), TEXT("Next visualization mode"), EventData);
        return Mode;
    }

    return ETajsGraphVisMode::Default;
}

ETajsGraphVisMode UTajsGraphDebugSubsystem::PreviousVisualizationMode()
{
    if (!VisualizationSubsystem.IsValid())
    {
        VisualizationSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UTajsGraphVisualizationSubsystem>() : nullptr;
    }

    if (UTajsGraphVisualizationSubsystem *Vis = VisualizationSubsystem.Get())
    {
        const ETajsGraphVisMode Mode = Vis->PreviousMode();
        Vis->SetVisualizationEnabled(Mode != ETajsGraphVisMode::Default);
        TMap<FString, FString> EventData;
        EventData.Add(TEXT("mode"), StaticEnum<ETajsGraphVisMode>()->GetNameStringByValue(static_cast<int64>(Mode)));
        RecordEvent(TEXT("Visualization"), TEXT("Previous visualization mode"), EventData);
        return Mode;
    }

    return ETajsGraphVisMode::Default;
}

bool UTajsGraphDebugSubsystem::ToggleVisualization()
{
    if (!VisualizationSubsystem.IsValid())
    {
        VisualizationSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UTajsGraphVisualizationSubsystem>() : nullptr;
    }

    if (UTajsGraphVisualizationSubsystem *Vis = VisualizationSubsystem.Get())
    {
        const bool bNowEnabled = Vis->ToggleVisualizationEnabled();
        RecordEvent(TEXT("Visualization"), bNowEnabled ? TEXT("Enabled") : TEXT("Disabled"), {});
        return bNowEnabled;
    }

    return false;
}

bool UTajsGraphDebugSubsystem::IsVisualizationEnabled() const
{
    if (const UTajsGraphVisualizationSubsystem *Vis = VisualizationSubsystem.Get())
    {
        return Vis->IsVisualizationEnabled();
    }

    return false;
}

ETajsGraphVisMode UTajsGraphDebugSubsystem::GetVisualizationMode() const
{
    if (const UTajsGraphVisualizationSubsystem *Vis = VisualizationSubsystem.Get())
    {
        return Vis->GetCurrentMode();
    }

    return ETajsGraphVisMode::Default;
}

TArray<FString> UTajsGraphDebugSubsystem::GetOverlayLines() const
{
    TArray<FString> Lines;

    if (!bDebugEnabled)
    {
        Lines.Add(TEXT("TajsGraph DebugHub is disabled."));
        Lines.Add(TEXT("Enable DebugHub in the SML mod settings or your settings UI."));
        return Lines;
    }

    FTajsGraphPerfSample LastPerfValue;
    FTajsGraphRenderStateSample LastRenderValue;
    FTajsGraphSceneProbeSample LastProbeValue;
    const FTajsGraphPerfSample *LastPerf = PerfRing.TryGetLatest(LastPerfValue) ? &LastPerfValue : nullptr;
    const FTajsGraphRenderStateSample *LastRender = RenderRing.TryGetLatest(LastRenderValue) ? &LastRenderValue : nullptr;
    const FTajsGraphSceneProbeSample *LastProbe = ProbeRing.TryGetLatest(LastProbeValue) ? &LastProbeValue : nullptr;

    switch (OverlayPage)
    {
    case ETajsGraphOverlayPage::Perf:
        if (LastPerf)
        {
            Lines.Add(FString::Printf(TEXT("FPS: %.1f | Frame: %.2f ms"), LastPerf->FPS, LastPerf->FrameMs));
            Lines.Add(FString::Printf(TEXT("P95/P99: %.2f / %.2f ms"), LastPerf->P95Ms, LastPerf->P99Ms));
            Lines.Add(FString::Printf(TEXT("Hitch threshold: %.1f ms"), HitchThresholdMs));
        }
        else
        {
            Lines.Add(TEXT("No perf samples yet."));
        }
        break;
    case ETajsGraphOverlayPage::Renderer:
        if (LastRender)
        {
            Lines.Add(FString::Printf(TEXT("RHI: %s | FL: %s"), *LastRender->RHIName, *LastRender->FeatureLevel));
            Lines.Add(FString::Printf(TEXT("Res: %dx%d | ScreenPct: %.1f"), LastRender->ResX, LastRender->ResY, LastRender->ScreenPercentage));
            Lines.Add(FString::Printf(TEXT("Upscaler:%s Lumen:%s Nanite:%s VSM:%s"),
                                      LastRender->bUpscalerActive ? TEXT("on") : TEXT("off"),
                                      LastRender->bLumenActive ? TEXT("on") : TEXT("off"),
                                      LastRender->bNaniteEnabled ? TEXT("on") : TEXT("off"),
                                      LastRender->bVSMEnabled ? TEXT("on") : TEXT("off")));
        }
        else
        {
            Lines.Add(TEXT("No renderer samples yet."));
        }
        break;
    case ETajsGraphOverlayPage::PostProcess:
        if (LastRender)
        {
            const auto *ExposureMin = LastRender->FinalPPWatchedValues.Find(TEXT("AutoExposureMinBrightness"));
            const auto *ExposureMax = LastRender->FinalPPWatchedValues.Find(TEXT("AutoExposureMaxBrightness"));
            const auto *Bloom = LastRender->FinalPPWatchedValues.Find(TEXT("BloomIntensity"));
            Lines.Add(FString::Printf(TEXT("Exposure Min/Max: %.3f / %.3f"), ExposureMin ? *ExposureMin : 0.0f, ExposureMax ? *ExposureMax : 0.0f));
            Lines.Add(FString::Printf(TEXT("Bloom: %.3f"), Bloom ? *Bloom : 0.0f));
            Lines.Add(FString::Printf(TEXT("Blendables: %d"), LastRender->ActiveBlendables.Num()));
        }
        else
        {
            Lines.Add(TEXT("No post-process data yet."));
        }
        break;
    case ETajsGraphOverlayPage::Lights:
        if (LastProbe)
        {
            Lines.Add(FString::Printf(TEXT("Lights 10m/25m/50m: %d / %d / %d"), LastProbe->NearbyLights10m, LastProbe->NearbyLights25m, LastProbe->NearbyLights50m));
            Lines.Add(FString::Printf(TEXT("Shadow casters: %d | Overlap: %.2f"), LastProbe->NearbyShadowCasters, LastProbe->OverlapScore));
            if (LastProbe->TopLights.Num() > 0)
            {
                const FTajsGraphSceneProbeOffender &Top = LastProbe->TopLights[0];
                Lines.Add(FString::Printf(TEXT("Top offender: %s (S=%.2f)"), *Top.ActorName, Top.Score));
            }
        }
        else
        {
            Lines.Add(TEXT("No scene probe data yet."));
        }
        break;
    case ETajsGraphOverlayPage::Warnings:
    {
        TArray<FTajsGraphDebugEvent> Events;
        EventRing.ToArrayTail(256, Events);
        for (int32 Index = Events.Num() - 1; Index >= 0 && Lines.Num() < 8; --Index)
        {
            if (Events[Index].Category.Contains(TEXT("Warning")) || Events[Index].Message.Contains(TEXT("failed"), ESearchCase::IgnoreCase))
            {
                Lines.Add(FString::Printf(TEXT("[%s] %s"), *Events[Index].Category, *Events[Index].Message));
            }
        }
        if (Lines.Num() <= 0)
        {
            Lines.Add(TEXT("No warnings recorded."));
        }
        break;
    }
    }

    return Lines;
}

void UTajsGraphDebugSubsystem::UpdateFromViewExtension()
{
    if (!ViewExtension.IsValid())
    {
        return;
    }

    TArray<FTajsGraphRenderStateSample> Samples;
    ViewExtension->DrainRenderStateSamples(Samples);
    for (FTajsGraphRenderStateSample &Sample : Samples)
    {
        RenderRing.Push(Sample);
        if (!bHasBaselineRenderState)
        {
            BaselineRenderState = Sample;
            bHasBaselineRenderState = true;
        }
    }
}

bool UTajsGraphDebugSubsystem::Tick(const float DeltaSeconds)
{
    const double Now = FPlatformTime::Seconds();

    const bool bRefreshByInterval = (Now - LastConfigRefreshTimeSeconds) >= static_cast<double>(ConfigRefreshIntervalSeconds);
    if (bConfigRefreshRequested || bRefreshByInterval)
    {
        RefreshConfigFromSettings();
        bConfigRefreshRequested = false;
        LastConfigRefreshTimeSeconds = Now;
    }
    EnsureViewExtensionState();

    if (!bDebugEnabled)
    {
        return true;
    }

    if ((PerfSampleIntervalSeconds <= 0.0f || (Now - LastPerfSampleTime) >= PerfSampleIntervalSeconds))
    {
        CollectPerfSample(DeltaSeconds);
        LastPerfSampleTime = Now;
    }

    if ((RenderSampleIntervalSeconds <= 0.0f || (Now - LastRenderDrainTime) >= RenderSampleIntervalSeconds))
    {
        UpdateFromViewExtension();
        LastRenderDrainTime = Now;
    }

    if ((SceneProbeIntervalSeconds > 0.0f) && (Now - LastProbeSampleTime) >= SceneProbeIntervalSeconds)
    {
        ProbeRing.Push(RunSceneProbe());
        LastProbeSampleTime = Now;
    }

    if ((LastStateWriteIntervalSeconds > 0.0f) && (Now - LastBreadcrumbWriteTime) >= LastStateWriteIntervalSeconds)
    {
        WriteLastStateBreadcrumb();
        LastBreadcrumbWriteTime = Now;
    }

    return true;
}

void UTajsGraphDebugSubsystem::RefreshConfigFromSettings()
{
    auto ReadBool = [](const TCHAR *Key, const bool DefaultValue)
    {
        bool Value = DefaultValue;
        FTajsGraphModule::GetBoolSetting(TEXT("DebugHub"), Key, Value);
        return Value;
    };

    auto ReadInt = [](const TCHAR *Key, const int32 DefaultValue)
    {
        int32 Value = DefaultValue;
        FTajsGraphModule::GetIntSetting(TEXT("DebugHub"), Key, Value);
        return Value;
    };

    auto ReadFloat = [](const TCHAR *Key, const float DefaultValue)
    {
        float Value = DefaultValue;
        FTajsGraphModule::GetFloatSetting(TEXT("DebugHub"), Key, Value);
        return Value;
    };

    const bool bConfigEnabled = ReadBool(TEXT("Enabled"), false);
    if (bDebugEnabled != bConfigEnabled)
    {
        bDebugEnabled = bConfigEnabled;
        OnDebugStateChanged.Broadcast(bDebugEnabled);
    }

    PerfSampleIntervalSeconds = FMath::Clamp(ReadFloat(TEXT("PerfSampleInterval"), 0.0f), 0.0f, 30.0f);
    RenderSampleIntervalSeconds = FMath::Clamp(ReadFloat(TEXT("RenderSampleInterval"), 0.2f), 0.0f, 30.0f);
    SceneProbeIntervalSeconds = FMath::Clamp(ReadFloat(TEXT("SceneProbeInterval"), 1.0f), 0.0f, 30.0f);
    LastStateWriteIntervalSeconds = FMath::Clamp(ReadFloat(TEXT("LastStateWriteInterval"), 3.0f), 0.0f, 60.0f);
    HitchThresholdMs = FMath::Clamp(ReadFloat(TEXT("HitchThresholdMs"), 60.0f), 1.0f, 1000.0f);
    HitchSignatureCooldownSeconds = FMath::Clamp(ReadFloat(TEXT("HitchPerSignatureCooldownSec"), 30.0f), 0.0f, 600.0f);
    HistorySecondsForSnapshot = FMath::Clamp(ReadInt(TEXT("HistorySecondsForSnapshot"), 10), 1, 300);
    bWriteExpandedReportArtifacts = ReadBool(TEXT("WriteExpandedReportArtifacts"), false);

    const bool bConfigOverlayEnabled = ReadBool(TEXT("OverlayEnabled"), false);
    if (bDebugEnabled)
    {
        if (bOverlayVisible != bConfigOverlayEnabled)
        {
            ApplyOverlayVisibility(bConfigOverlayEnabled, false);
        }
    }
    else
    {
        ApplyOverlayVisibility(false, false);
        ApplyMenuVisibility(false);
        if (!VisualizationSubsystem.IsValid())
        {
            VisualizationSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UTajsGraphVisualizationSubsystem>() : nullptr;
        }
        if (UTajsGraphVisualizationSubsystem *Vis = VisualizationSubsystem.Get())
        {
            Vis->SetVisualizationEnabled(false);
        }
    }

    MaxCopiedHitches = FMath::Clamp(ReadInt(TEXT("MaxCopiedHitchSnapshots"), 5), 0, 50);

    const int32 NewMaxPerf = FMath::Clamp(ReadInt(TEXT("MaxPerfSamples"), 18000), 60, 200000);
    const int32 NewMaxRender = FMath::Clamp(ReadInt(TEXT("MaxRenderSamples"), 3000), 60, 50000);
    const int32 NewMaxProbe = FMath::Clamp(ReadInt(TEXT("MaxProbeSamples"), 1200), 30, 10000);
    const int32 NewMaxEvents = FMath::Clamp(ReadInt(TEXT("MaxEvents"), 1024), 64, 20000);

    if (NewMaxPerf != MaxPerfSamples)
    {
        MaxPerfSamples = NewMaxPerf;
        PerfRing.Reset(MaxPerfSamples);
    }
    if (NewMaxRender != MaxRenderSamples)
    {
        MaxRenderSamples = NewMaxRender;
        RenderRing.Reset(MaxRenderSamples);
    }
    if (NewMaxProbe != MaxProbeSamples)
    {
        MaxProbeSamples = NewMaxProbe;
        ProbeRing.Reset(MaxProbeSamples);
    }
    if (NewMaxEvents != MaxEvents)
    {
        MaxEvents = NewMaxEvents;
        EventRing.Reset(MaxEvents);
    }
}

void UTajsGraphDebugSubsystem::HandleSettingChanged(const FString &Section, const FString &Key, const FString &Value)
{
    if (Section == TEXT("DebugHub") || Section == TEXT("Visualization"))
    {
        bConfigRefreshRequested = true;
        RecordEvent(TEXT("SettingChanged"), FString::Printf(TEXT("%s.%s=%s"), *Section, *Key, *Value), {});
    }
}

void UTajsGraphDebugSubsystem::EnsureViewExtensionState()
{
    if (bDebugEnabled)
    {
        if (!ViewExtension.IsValid())
        {
            ViewExtension = FSceneViewExtensions::NewExtension<FTajsGraphDebugViewExtension>(TWeakObjectPtr<UTajsGraphDebugSubsystem>(this));
            RecordEvent(TEXT("Debug"), TEXT("Registered view extension"), {});
        }
    }
    else
    {
        if (ViewExtension.IsValid())
        {
            ViewExtension.Reset();
            RecordEvent(TEXT("Debug"), TEXT("Unregistered view extension"), {});
        }
    }
}

void UTajsGraphDebugSubsystem::CollectPerfSample(const float DeltaSeconds)
{
    FTajsGraphPerfSample Sample;
    Sample.Timestamp = FPlatformTime::Seconds();

    const float SafeDelta = DeltaSeconds > 0.0f ? DeltaSeconds : static_cast<float>(FApp::GetDeltaTime());
    Sample.FrameMs = SafeDelta * 1000.0f;
    Sample.FPS = SafeDelta > KINDA_SMALL_NUMBER ? (1.0f / SafeDelta) : 0.0f;
    Sample.bHitch = Sample.FrameMs > HitchThresholdMs;

    if (UGameViewportClient *Viewport = GEngine ? GEngine->GameViewport : nullptr)
    {
        if (Viewport->Viewport)
        {
            const FIntPoint Size = Viewport->Viewport->GetSizeXY();
            Sample.ResX = Size.X;
            Sample.ResY = Size.Y;
        }
    }

    FTajsGraphRenderStateSample LastRenderSample;
    if (RenderRing.TryGetLatest(LastRenderSample))
    {
        Sample.ScreenPct = LastRenderSample.ScreenPercentage;
    }

    constexpr int32 WindowCount = 240;
    TArray<FTajsGraphPerfSample> PerfSamples;
    PerfRing.ToArrayTail(WindowCount - 1, PerfSamples);
    TArray<float> FrameTimes;
    FrameTimes.Reserve(PerfSamples.Num() + 1);
    for (const FTajsGraphPerfSample &PriorSample : PerfSamples)
    {
        FrameTimes.Add(PriorSample.FrameMs);
    }
    FrameTimes.Add(Sample.FrameMs);
    FrameTimes.Sort([](const float A, const float B)
                    { return A < B; });
    RollingP95Ms = Percentile(FrameTimes, 0.95f);
    RollingP99Ms = Percentile(FrameTimes, 0.99f);
    Sample.P95Ms = RollingP95Ms;
    Sample.P99Ms = RollingP99Ms;

    PerfRing.Push(Sample);

    if (Sample.bHitch)
    {
        EvaluateHitch(Sample);
    }
}

void UTajsGraphDebugSubsystem::UpdateQuantiles()
{
    constexpr int32 WindowCount = 240;
    TArray<FTajsGraphPerfSample> Samples;
    PerfRing.ToArrayTail(WindowCount, Samples);
    if (Samples.Num() <= 0)
    {
        RollingP95Ms = 0.0f;
        RollingP99Ms = 0.0f;
        return;
    }

    TArray<float> FrameTimes;
    FrameTimes.Reserve(Samples.Num());
    for (const FTajsGraphPerfSample &Entry : Samples)
    {
        FrameTimes.Add(Entry.FrameMs);
    }

    FrameTimes.Sort([](const float A, const float B)
                    { return A < B; });
    RollingP95Ms = Percentile(FrameTimes, 0.95f);
    RollingP99Ms = Percentile(FrameTimes, 0.99f);
}

void UTajsGraphDebugSubsystem::EvaluateHitch(const FTajsGraphPerfSample &PerfSample)
{
    FTajsGraphRenderStateSample LastRender;
    if (!RenderRing.TryGetLatest(LastRender))
    {
        LastRender = FTajsGraphRenderStateSample();
    }

    FTajsGraphSceneProbeSample LastProbe;
    if (!ProbeRing.TryGetLatest(LastProbe))
    {
        LastProbe = RunSceneProbe();
    }

    const uint32 Signature = BuildHitchSignature(LastRender, LastProbe);
    const int32 NewCount = HitchSignatureCounts.FindOrAdd(Signature) + 1;
    HitchSignatureCounts.Add(Signature, NewCount);

    const double Now = FPlatformTime::Seconds();
    const double LastWrite = HitchSignatureLastWriteTimes.FindRef(Signature);
    const bool bWriteNow = (LastWrite <= 0.0) || (Now - LastWrite >= HitchSignatureCooldownSeconds);

    TMap<FString, FString> HitchEventData;
    HitchEventData.Add(TEXT("frame_ms"), FString::SanitizeFloat(PerfSample.FrameMs));
    HitchEventData.Add(TEXT("signature"), FString::Printf(TEXT("%u"), Signature));
    HitchEventData.Add(TEXT("count"), FString::FromInt(NewCount));
    RecordEvent(TEXT("Hitch"), TEXT("Detected hitch"), HitchEventData);

    if (!bWriteNow)
    {
        return;
    }

    HitchSignatureLastWriteTimes.Add(Signature, Now);

    FTajsGraphDebugSnapshot Snapshot = CreateSnapshot(TEXT("Hitch"), static_cast<float>(HistorySecondsForSnapshot));
    Snapshot.Metadata.Add(TEXT("hitch_signature"), FString::Printf(TEXT("%u"), Signature));
    Snapshot.Metadata.Add(TEXT("hitch_count"), FString::FromInt(NewCount));

    const FString HitchDir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("TajsGraph"), TEXT("Reports"), TEXT("Hitches"));
    IFileManager::Get().MakeDirectory(*HitchDir, true);
    const FString FilePath = FPaths::Combine(HitchDir, FString::Printf(TEXT("Hitch_%s.json"), *TimestampString()));

    FTajsGraphDebugIO::WriteSnapshotJson(Snapshot, FilePath);
    OnSnapshotCreated.Broadcast(FilePath);
}

FTajsGraphRenderStateSample UTajsGraphDebugSubsystem::BuildBasicRenderStateSample() const
{
    FTajsGraphRenderStateSample Out;
    Out.Timestamp = FPlatformTime::Seconds();
    Out.RHIName = GDynamicRHI ? FString(GDynamicRHI->GetName()) : TEXT("Unknown");

    if (UGameUserSettings *Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
    {
        switch (Settings->GetFullscreenMode())
        {
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
            Out.WindowMode = TEXT("Unknown");
            break;
        }
    }

    if (UGameViewportClient *Viewport = GEngine ? GEngine->GameViewport : nullptr)
    {
        if (Viewport->Viewport)
        {
            const FIntPoint Size = Viewport->Viewport->GetSizeXY();
            Out.ResX = Size.X;
            Out.ResY = Size.Y;
            Out.ViewRectW = Size.X;
            Out.ViewRectH = Size.Y;
        }
    }

    auto GetCVarInt = [](const TCHAR *Name, int32 DefaultValue = 0)
    {
        if (IConsoleVariable *Var = IConsoleManager::Get().FindConsoleVariable(Name))
        {
            return Var->GetInt();
        }
        return DefaultValue;
    };
    auto GetCVarFloat = [](const TCHAR *Name, float DefaultValue = 0.0f)
    {
        if (IConsoleVariable *Var = IConsoleManager::Get().FindConsoleVariable(Name))
        {
            return Var->GetFloat();
        }
        return DefaultValue;
    };

    Out.ScreenPercentage = GetCVarFloat(TEXT("r.ScreenPercentage"), 100.0f);
    Out.bUpscalerActive = GetCVarInt(TEXT("r.TemporalAA.Upsampling"), 0) != 0 || GetCVarInt(TEXT("r.AntiAliasingMethod"), 0) == 4;
    Out.bLumenActive = GetCVarInt(TEXT("r.Lumen.DiffuseIndirect.Allow"), 0) != 0 || GetCVarInt(TEXT("r.Lumen.Reflections.Allow"), 0) != 0;
    Out.bNaniteEnabled = GetCVarInt(TEXT("r.Nanite"), 0) != 0;
    Out.bVSMEnabled = GetCVarInt(TEXT("r.Shadow.Virtual.Enable"), 0) != 0;
    Out.FeatureLevel = TEXT("Unknown");
    Out.ShaderModel = FString::Printf(TEXT("SM%d"), GetCVarInt(TEXT("r.ShaderModel"), 0));

    return Out;
}

FTajsGraphPerfSample UTajsGraphDebugSubsystem::BuildBasicPerfSample(const float DeltaSeconds) const
{
    FTajsGraphPerfSample Sample;
    const float SafeDelta = DeltaSeconds > 0.0f ? DeltaSeconds : static_cast<float>(FApp::GetDeltaTime());
    Sample.Timestamp = FPlatformTime::Seconds();
    Sample.FrameMs = SafeDelta * 1000.0f;
    Sample.FPS = SafeDelta > KINDA_SMALL_NUMBER ? (1.0f / SafeDelta) : 0.0f;
    Sample.P95Ms = RollingP95Ms;
    Sample.P99Ms = RollingP99Ms;
    Sample.bHitch = Sample.FrameMs > HitchThresholdMs;
    Sample.ScreenPct = BuildBasicRenderStateSample().ScreenPercentage;
    if (UGameViewportClient *Viewport = GEngine ? GEngine->GameViewport : nullptr)
    {
        if (Viewport->Viewport)
        {
            const FIntPoint Size = Viewport->Viewport->GetSizeXY();
            Sample.ResX = Size.X;
            Sample.ResY = Size.Y;
        }
    }
    return Sample;
}

TArray<FTajsGraphPPDiffEntry> UTajsGraphDebugSubsystem::ComputePostProcessDiff(const FTajsGraphRenderStateSample &Baseline, const FTajsGraphRenderStateSample &Current) const
{
    TArray<FTajsGraphPPDiffEntry> Out;
    for (const TPair<FString, float> &Pair : Current.FinalPPWatchedValues)
    {
        const float *BaselineValue = Baseline.FinalPPWatchedValues.Find(Pair.Key);
        if (!BaselineValue)
        {
            continue;
        }

        const float Delta = Pair.Value - *BaselineValue;
        if (FMath::Abs(Delta) <= KINDA_SMALL_NUMBER)
        {
            continue;
        }

        FTajsGraphPPDiffEntry Entry;
        Entry.Field = Pair.Key;
        Entry.Baseline = *BaselineValue;
        Entry.Current = Pair.Value;
        Entry.Delta = Delta;
        Out.Add(MoveTemp(Entry));
    }
    return Out;
}

TArray<FTajsGraphPPProvenanceEntry> UTajsGraphDebugSubsystem::BuildPostProcessProvenance(const FTajsGraphRenderStateSample &Current) const
{
    TArray<FTajsGraphPPProvenanceEntry> Out;
    UWorld *World = ResolveWorld(this);
    if (!World)
    {
        return Out;
    }

    FVector PlayerLocation = FVector::ZeroVector;
    if (const APawn *Pawn = UGameplayStatics::GetPlayerPawn(World, 0))
    {
        PlayerLocation = Pawn->GetActorLocation();
    }

    struct FPPContributor
    {
        FString Name;
        float Priority = 0.0f;
        float BlendWeight = 0.0f;
        TMap<FString, float> Values;
    };

    TArray<FPPContributor> Contributors;
    Contributors.Reserve(32);

    for (TActorIterator<APostProcessVolume> It(World); It; ++It)
    {
        const APostProcessVolume *PPV = *It;
        if (!IsValid(PPV) || !PPV->bEnabled)
        {
            continue;
        }

        // Check if player location is inside the volume
        bool bPlayerInsideVolume = PPV->bUnbound;
        if (!bPlayerInsideVolume)
        {
            const UBrushComponent *BrushComponent = PPV->GetBrushComponent();
            if (!BrushComponent)
            {
                continue;
            }

            FVector ClosestPoint;
            float DistanceSqr = 0.0f;
            if (!BrushComponent->GetSquaredDistanceToCollision(PlayerLocation, DistanceSqr, ClosestPoint))
            {
                continue;
            }

            bPlayerInsideVolume = (DistanceSqr <= KINDA_SMALL_NUMBER);
        }

        if (!bPlayerInsideVolume)
        {
            continue;
        }

        FPPContributor Contributor;
        Contributor.Name = PPV->GetName();
        Contributor.Priority = PPV->Priority;
        Contributor.BlendWeight = PPV->BlendWeight;

        const FPostProcessSettings &Settings = PPV->Settings;
        if (Settings.bOverride_AutoExposureMinBrightness)
            Contributor.Values.Add(TEXT("AutoExposureMinBrightness"), Settings.AutoExposureMinBrightness);
        if (Settings.bOverride_AutoExposureMaxBrightness)
            Contributor.Values.Add(TEXT("AutoExposureMaxBrightness"), Settings.AutoExposureMaxBrightness);
        if (Settings.bOverride_AutoExposureBias)
            Contributor.Values.Add(TEXT("AutoExposureBias"), Settings.AutoExposureBias);
        if (Settings.bOverride_BloomIntensity)
            Contributor.Values.Add(TEXT("BloomIntensity"), Settings.BloomIntensity);
        if (Settings.bOverride_VignetteIntensity)
            Contributor.Values.Add(TEXT("VignetteIntensity"), Settings.VignetteIntensity);
        if (Settings.bOverride_FilmShoulder)
            Contributor.Values.Add(TEXT("FilmShoulder"), Settings.FilmShoulder);
        if (Settings.bOverride_DepthOfFieldFstop)
            Contributor.Values.Add(TEXT("DepthOfFieldFstop"), Settings.DepthOfFieldFstop);
        if (Settings.bOverride_LumenSceneLightingQuality)
            Contributor.Values.Add(TEXT("LumenSceneLightingQuality"), Settings.LumenSceneLightingQuality);
        if (Settings.bOverride_LumenSceneDetail)
            Contributor.Values.Add(TEXT("LumenSceneDetail"), Settings.LumenSceneDetail);
        if (Settings.bOverride_LumenSceneViewDistance)
            Contributor.Values.Add(TEXT("LumenSceneViewDistance"), Settings.LumenSceneViewDistance);
        if (Settings.bOverride_LumenSceneLightingUpdateSpeed)
            Contributor.Values.Add(TEXT("LumenSceneLightingUpdateSpeed"), Settings.LumenSceneLightingUpdateSpeed);
        if (Settings.bOverride_LumenFinalGatherQuality)
            Contributor.Values.Add(TEXT("LumenFinalGatherQuality"), Settings.LumenFinalGatherQuality);
        if (Settings.bOverride_LumenFinalGatherLightingUpdateSpeed)
            Contributor.Values.Add(TEXT("LumenFinalGatherLightingUpdateSpeed"), Settings.LumenFinalGatherLightingUpdateSpeed);
        if (Settings.bOverride_LumenMaxTraceDistance)
            Contributor.Values.Add(TEXT("LumenMaxTraceDistance"), Settings.LumenMaxTraceDistance);
        if (Settings.bOverride_LumenSkylightLeaking)
            Contributor.Values.Add(TEXT("LumenSkylightLeaking"), Settings.LumenSkylightLeaking);
        if (Settings.bOverride_LumenSceneLightingUpdateSpeed)
            Contributor.Values.Add(TEXT("LumenSceneLightingUpdateSpeed"), Settings.LumenSceneLightingUpdateSpeed);

        if (Contributor.Values.Num() > 0)
        {
            Contributors.Add(MoveTemp(Contributor));
        }
    }

    Contributors.Sort([](const FPPContributor &A, const FPPContributor &B)
                      {
        if (A.Priority == B.Priority) {
            return A.BlendWeight > B.BlendWeight;
        }
        return A.Priority > B.Priority; });

    for (const TPair<FString, float> &FinalPair : Current.FinalPPWatchedValues)
    {
        FTajsGraphPPProvenanceEntry Entry;
        Entry.Field = FinalPair.Key;
        Entry.FinalValue = FinalPair.Value;
        Entry.RequestedValue = FinalPair.Value;
        Entry.LastWriterGuess = TEXT("Unknown/Engine");
        Entry.Confidence = 0.2f;
        Entry.bInferred = true;

        for (const FPPContributor &Contributor : Contributors)
        {
            const float *Value = Contributor.Values.Find(FinalPair.Key);
            if (!Value)
            {
                continue;
            }

            const float Delta = FMath::Abs(*Value - FinalPair.Value);
            if (Delta <= 0.01f)
            {
                Entry.LastWriterGuess = Contributor.Name;
                Entry.Confidence = FMath::Clamp(1.0f - (Delta * 10.0f), 0.35f, 0.95f);
                break;
            }
        }

        Out.Add(MoveTemp(Entry));
    }

    return Out;
}

void UTajsGraphDebugSubsystem::WriteLastStateBreadcrumb()
{
    FTajsGraphDebugSnapshot Snapshot = CreateSnapshot(TEXT("LastState"), 5.0f);
    Snapshot.Metadata.Add(TEXT("type"), TEXT("breadcrumb"));

    const FString LastStatePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("TajsGraph"), TEXT("last_state.json"));
    FTajsGraphDebugIO::WriteSnapshotJson(Snapshot, LastStatePath);
}


