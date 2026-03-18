#include "TajsGraphStateSubsystem.h"

// Define a log category for TajsGraph
DEFINE_LOG_CATEGORY_STATIC(LogTajsGraphState, Log, All);

void UTajsGraphStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTajsGraphState, Log, TEXT("TajsGraphStateSubsystem initialized. Waiting for Blueprint/SML config push."));
}

void UTajsGraphStateSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UTajsGraphStateSubsystem::SetEnableLogging(bool bEnabled)
{
    bEnableLogging = bEnabled;
    if (bEnableLogging)
    {
        UE_LOG(LogTajsGraphState, Log, TEXT("TajsGraph Logging Enabled via Blueprint."));
    }
}

void UTajsGraphStateSubsystem::SetEnableDebugLogging(bool bEnabled)
{
    bEnableDebugLogging = bEnabled;
    if (bEnableDebugLogging && bEnableLogging)
    {
        UE_LOG(LogTajsGraphState, Log, TEXT("TajsGraph Debug Logging Enabled via Blueprint."));
    }
}

void UTajsGraphStateSubsystem::SetEnableGpuTiming(bool bEnabled)
{
    bEnableGpuTiming = bEnabled;
}

void UTajsGraphStateSubsystem::SetPerfSampleInterval(float Interval)
{
    PerfSampleInterval = FMath::Max(0.01f, Interval);
}

void UTajsGraphStateSubsystem::SetMaxPerfSamples(int32 MaxSamples)
{
    MaxPerfSamples = FMath::Clamp(MaxSamples, 10, 10000);
}

void UTajsGraphStateSubsystem::SetMaxSurfaceCacheEvents(int32 MaxEvents)
{
    MaxSurfaceCacheEvents = FMath::Clamp(MaxEvents, 10, 5000);
}
