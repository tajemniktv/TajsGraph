#include "TajsGraphDebugHubLibrary.h"

TArray<FTajsGraphPerfSample> UTajsGraphDebugHubLibrary::GetRecentPerformanceSamples()
{
    return FTajRuntimeProfiler::Get().GetRecentPerfSamples();
}

TArray<FTajsGraphSurfaceCacheEvent> UTajsGraphDebugHubLibrary::GetRecentSurfaceCacheEvents()
{
    return UTajsGraphSurfaceCache::GetRecentSurfaceCacheEvents();
}

void UTajsGraphDebugHubLibrary::TickProfiler(UObject* WorldContextObject)
{
    FTajRuntimeProfiler::Get().TickPerFrame(WorldContextObject);
}
