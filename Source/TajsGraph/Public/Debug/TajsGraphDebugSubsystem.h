#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Debug/TajsGraphDebugTypes.h"
#include "TajsGraphDebugSubsystem.generated.h"

class FTajsGraphDebugViewExtension;
class UTajsGraphVisualizationSubsystem;
class UTajsGraphDebugOverlayWidget;
class UTajsGraphDebugMenuWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTajsGraphDebugStateChangedDelegate, bool, bEnabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTajsGraphDebugSnapshotCreatedDelegate, const FString&, SnapshotPath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTajsGraphDebugReportGeneratedDelegate, const FString&, ReportDirectory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTajsGraphDebugWarningDelegate, const FString&, Code, const FString&, Message);

UCLASS()
class TAJSGRAPH_API UTajsGraphDebugSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void ReloadConfigFromSettings();

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    bool IsDebugEnabled() const { return bDebugEnabled; }

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void SetDebugEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void RecordEvent(const FString& Category, const FString& Message, const TMap<FString, FString>& KeyValues);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void RecordSample();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    FTajsGraphDebugSnapshot CreateSnapshot(const FString& Reason, float IncludeHistorySeconds = 10.0f);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    bool WriteReport(const FTajsGraphDebugSnapshot& Snapshot, const FString& OutputDirectory);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    FString GenerateReportBundle();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    FTajsGraphSceneProbeSample RunSceneProbe();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void ClearHistory();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void ToggleOverlay();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void SetOverlayVisible(bool bVisible);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    bool IsOverlayVisible() const { return bOverlayVisible; }

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void ToggleMenu();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void OpenHub();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void CloseHub();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void ToggleHub();

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    bool IsHubVisible() const { return bMenuVisible; }

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void SetMenuVisible(bool bVisible);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    bool IsMenuVisible() const { return bMenuVisible; }

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void SetOverlayPage(ETajsGraphOverlayPage InPage);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    ETajsGraphOverlayPage GetOverlayPage() const { return OverlayPage; }

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void SetVisualizationMode(ETajsGraphVisMode Mode);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    ETajsGraphVisMode NextVisualizationMode();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    ETajsGraphVisMode PreviousVisualizationMode();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    bool ToggleVisualization();

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    bool IsVisualizationEnabled() const;

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    ETajsGraphVisMode GetVisualizationMode() const;

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    TArray<FString> GetOverlayLines() const;

    void UpdateFromViewExtension();

    UPROPERTY(BlueprintAssignable, Category = "TajsGraph|Debug")
    FTajsGraphDebugStateChangedDelegate OnDebugStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "TajsGraph|Debug")
    FTajsGraphDebugSnapshotCreatedDelegate OnSnapshotCreated;

    UPROPERTY(BlueprintAssignable, Category = "TajsGraph|Debug")
    FTajsGraphDebugReportGeneratedDelegate OnReportGenerated;

    UPROPERTY(BlueprintAssignable, Category = "TajsGraph|Debug")
    FTajsGraphDebugWarningDelegate OnWarningRaised;

private:
    bool Tick(float DeltaSeconds);

    void RefreshConfigFromSettings();
    void HandleSettingChanged(const FString& Section, const FString& Key, const FString& Value);
    void ApplyOverlayVisibility(bool bVisible, bool bPersistSetting);
    void ApplyMenuVisibility(bool bVisible);

    void EnsureViewExtensionState();
    void CollectPerfSample(float DeltaSeconds);
    void UpdateQuantiles();
    void EvaluateHitch(const FTajsGraphPerfSample& PerfSample);
    FTajsGraphRenderStateSample BuildBasicRenderStateSample() const;
    FTajsGraphPerfSample BuildBasicPerfSample(float DeltaSeconds = 0.0f) const;

    TArray<FTajsGraphPPDiffEntry> ComputePostProcessDiff(const FTajsGraphRenderStateSample& Baseline, const FTajsGraphRenderStateSample& Current) const;
    TArray<FTajsGraphPPProvenanceEntry> BuildPostProcessProvenance(const FTajsGraphRenderStateSample& Current) const;

    void WriteLastStateBreadcrumb();

private:
    FTSTicker::FDelegateHandle TickHandle;
    FDelegateHandle SettingChangedHandle;

    bool bDebugEnabled = false;
    bool bOverlayVisible = false;
    bool bMenuVisible = false;
    ETajsGraphOverlayPage OverlayPage = ETajsGraphOverlayPage::Perf;
    bool bConfigRefreshRequested = true;
    double LastConfigRefreshTimeSeconds = -10000.0;
    float ConfigRefreshIntervalSeconds = 5.0f;

    float PerfSampleIntervalSeconds = 0.0f;
    float RenderSampleIntervalSeconds = 0.2f;
    float SceneProbeIntervalSeconds = 1.0f;
    float LastStateWriteIntervalSeconds = 3.0f;
    float HitchThresholdMs = 60.0f;
    float HitchSignatureCooldownSeconds = 30.0f;
    int32 HistorySecondsForSnapshot = 10;
    int32 MaxCopiedHitches = 5;
    bool bWriteExpandedReportArtifacts = false;

    int32 MaxPerfSamples = 18000;
    int32 MaxRenderSamples = 3000;
    int32 MaxProbeSamples = 1200;
    int32 MaxEvents = 1024;

    double LastPerfSampleTime = 0.0;
    double LastRenderDrainTime = 0.0;
    double LastProbeSampleTime = 0.0;
    double LastBreadcrumbWriteTime = 0.0;

    float RollingP95Ms = 0.0f;
    float RollingP99Ms = 0.0f;

    TTajsGraphRingBuffer<FTajsGraphPerfSample> PerfRing;
    TTajsGraphRingBuffer<FTajsGraphRenderStateSample> RenderRing;
    TTajsGraphRingBuffer<FTajsGraphSceneProbeSample> ProbeRing;
    TTajsGraphRingBuffer<FTajsGraphDebugEvent> EventRing;

    FTajsGraphRenderStateSample BaselineRenderState;
    bool bHasBaselineRenderState = false;

    TSharedPtr<FTajsGraphDebugViewExtension, ESPMode::ThreadSafe> ViewExtension;

    TMap<uint32, int32> HitchSignatureCounts;
    TMap<uint32, double> HitchSignatureLastWriteTimes;

    TWeakObjectPtr<UTajsGraphVisualizationSubsystem> VisualizationSubsystem;
    TWeakObjectPtr<UTajsGraphDebugOverlayWidget> OverlayWidget;
    TWeakObjectPtr<UTajsGraphDebugMenuWidget> MenuWidget;
};
