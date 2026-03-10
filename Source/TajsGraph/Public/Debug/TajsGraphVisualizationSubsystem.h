#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Debug/TajsGraphDebugTypes.h"
#include "TajsGraphVisualizationSubsystem.generated.h"

class APostProcessVolume;
class UMaterialInterface;
class UMaterialInstanceDynamic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTajsGraphVisModeChangedDelegate, ETajsGraphVisMode, NewMode);

UCLASS()
class TAJSGRAPH_API UTajsGraphVisualizationSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void ReloadVisualizationConfig();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    bool SetVisualizationMode(ETajsGraphVisMode InMode);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void SetVisualizationEnabled(bool bInEnabled);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    bool ToggleVisualizationEnabled();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    ETajsGraphVisMode NextMode();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    ETajsGraphVisMode PreviousMode();

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    ETajsGraphVisMode GetCurrentMode() const { return CurrentMode; }

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    bool IsVisualizationEnabled() const { return bEnabled; }

    UPROPERTY(BlueprintAssignable, Category = "TajsGraph|Debug")
    FTajsGraphVisModeChangedDelegate OnVisualizationModeChanged;

private:
    bool ApplyCurrentMode();
    void RestorePreviousState();
    void RefreshVisualizationConfig();
    bool IsModeSwitchAllowed(ETajsGraphVisMode InMode) const;

    bool TryApplyEngineViewMode(ETajsGraphVisMode InMode);
    bool TryApplyRuntimeVisualizationCVars(ETajsGraphVisMode InMode);
    bool TryApplyPostProcessMaterial(ETajsGraphVisMode InMode);

    void EnsurePostProcessVolume();
    UMaterialInterface* ResolveMaterialForMode(ETajsGraphVisMode InMode) const;

private:
    ETajsGraphVisMode CurrentMode = ETajsGraphVisMode::Default;
    ETajsGraphVisMode LastAppliedMode = ETajsGraphVisMode::Default;
    bool bEnabled = false;

    TWeakObjectPtr<APostProcessVolume> InjectedPPVolume;
    TWeakObjectPtr<UMaterialInstanceDynamic> ActiveMID;
    TWeakObjectPtr<UMaterialInterface> ActiveMaterial;

    bool bEnableEngineViewAttempt = true;
    bool bEnableRuntimeVisCVars = true;
    bool bEnablePostProcessFallback = true;
    int32 ModeSwitchCooldownMs = 100;
    double LastModeSwitchTimeSeconds = -10000.0;

    bool bHasSavedViewportViewMode = false;
    int32 SavedViewportViewModeIndex = INDEX_NONE;

    TMap<FString, FString> SavedCVarValues;
};
