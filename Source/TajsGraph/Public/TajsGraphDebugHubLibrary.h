#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TajsGraphProfiler.h"
#include "TajsGraphSurfaceCache.h"
#include "TajsGraphDebugHubLibrary.generated.h"

/**
 * Provides static access to deep native profiler and hook data tailored for UMG visualization.
 */
UCLASS()
class TAJSGRAPH_API UTajsGraphDebugHubLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Fetch the latest array of performance history samples (FPS, CPU, GPU) */
    UFUNCTION(BlueprintPure, Category = "TajsGraph|DebugHub")
    static TArray<FTajsGraphPerfSample> GetRecentPerformanceSamples();

    /** Fetch the surface cache tracing events */
    UFUNCTION(BlueprintPure, Category = "TajsGraph|DebugHub")
    static TArray<FTajsGraphSurfaceCacheEvent> GetRecentSurfaceCacheEvents();

    /** Tick the profiler from the Editor/Game UI. Expected to be called on a Blueprint Tick. */
    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    static void TickProfiler(UObject* WorldContextObject);
};
