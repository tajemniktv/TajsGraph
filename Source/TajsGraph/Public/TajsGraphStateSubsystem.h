#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "TajsGraphStateSubsystem.generated.h"

/**
 * Global state subsystem for TajsGraph.
 * Provides a Blueprint-accessible API for Unreal Editor / SML to push config states into the C++ layer.
 */
UCLASS(DisplayName = "TajsGraph State Subsystem")
class TAJSGRAPH_API UTajsGraphStateSubsystem : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // -------------------------------------------------------------------
    // Debug & Logging
    // -------------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "TajsGraph|State")
    void SetEnableLogging(bool bEnabled);
    
    UFUNCTION(BlueprintPure, Category = "TajsGraph|State")
    bool GetEnableLogging() const { return bEnableLogging; }

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|State")
    void SetEnableDebugLogging(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|State")
    bool GetEnableDebugLogging() const { return bEnableDebugLogging; }

    // -------------------------------------------------------------------
    // Profiling
    // -------------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "TajsGraph|State")
    void SetEnableGpuTiming(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|State")
    bool GetEnableGpuTiming() const { return bEnableGpuTiming; }

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|State")
    void SetPerfSampleInterval(float Interval);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|State")
    float GetPerfSampleInterval() const { return PerfSampleInterval; }

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|State")
    void SetMaxPerfSamples(int32 MaxSamples);

    // -------------------------------------------------------------------
    // Surface Cache Tracing
    // -------------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "TajsGraph|State")
    void SetMaxSurfaceCacheEvents(int32 MaxEvents);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|State")
    int32 GetMaxSurfaceCacheEvents() const { return MaxSurfaceCacheEvents; }

private:
    bool bEnableLogging = true;
    bool bEnableDebugLogging = false;

    bool bEnableGpuTiming = false;
    float PerfSampleInterval = 1.0f;
    int32 MaxPerfSamples = 120;

    int32 MaxSurfaceCacheEvents = 512;
};
