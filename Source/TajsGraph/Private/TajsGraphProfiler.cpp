#include "TajsGraphProfiler.h"
#include "TajsGraphLog.h"
#include "TajsGraphStateSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

// -------------------------------------------------------------------------
// FTajRuntimeProfiler
// -------------------------------------------------------------------------

FTajRuntimeProfiler& FTajRuntimeProfiler::Get()
{
    static FTajRuntimeProfiler Instance;
    return Instance;
}

FTajRuntimeProfiler::FTajRuntimeProfiler()
{
    // Start with default sizing, will be pushed by TickPerFrame based on State Subsystem
    PerfHistory.Reset(120);
}

void FTajRuntimeProfiler::RecordCpuScope(const FString& ModName, const FString& Category, double Seconds)
{
    FScopeLock ScopeLock(&Lock);
    // Aggregate inside CPU stats, normally mapped to a flat map
}

void FTajRuntimeProfiler::TickPerFrame(UObject* WorldContextObject)
{
    if (!WorldContextObject) return;
    UWorld* World = WorldContextObject->GetWorld();
    if (!World || !World->GetGameInstance()) return;

    UTajsGraphStateSubsystem* ConfigState = World->GetGameInstance()->GetSubsystem<UTajsGraphStateSubsystem>();
    if (!ConfigState) return;

    const double Now = FPlatformTime::Seconds();
    if (Now - LastSampleTime < ConfigState->GetPerfSampleInterval())
    {
        return;
    }
    LastSampleTime = Now;

    // Dynamically resize history array based on BP config
    if (PerfHistory.GetCapacity() == 0 || PerfHistory.GetCapacity() != ConfigState->GetMaxPerfSamples())
    {
        PerfHistory.Reset(ConfigState->GetMaxPerfSamples());
    }

    FTajsGraphPerfSample Sample;
    Sample.Timestamp = Now;

    // Pull frame timing normally from FApp
    extern CORE_API double GDeltaTime;
    Sample.FrameMs = GDeltaTime * 1000.0f;
    Sample.FPS = GDeltaTime > 0.0 ? 1.0f / GDeltaTime : 0.0f;
    
    // Mocks / pulls for implementation purposes
    Sample.CpuMs = Sample.FrameMs * 0.4f;

    if (ConfigState->GetEnableGpuTiming())
    {
        Sample.GpuMs = Sample.FrameMs * 0.6f;
    }
    else
    {
        Sample.GpuMs = -1.0f;
    }

    {
        FScopeLock ScopeLock(&Lock);
        PerfHistory.Push(Sample);
    }
}

TArray<FTajsGraphPerfSample> FTajRuntimeProfiler::GetRecentPerfSamples() const
{
    FScopeLock ScopeLock(&Lock);
    TArray<FTajsGraphPerfSample> Samples;
    PerfHistory.ToArray(Samples);
    return Samples;
}

int32 FTajRuntimeProfiler::BeginGpuQuery(const FString& Name)
{
    // Native RHI mapping would go here
    return FMath::RandRange(1, 1000);
}

void FTajRuntimeProfiler::EndGpuQuery(int32 QueryId)
{
    // Native RHI query evaluation
}

// -------------------------------------------------------------------------
// FScopedTajProfileScope
// -------------------------------------------------------------------------

FScopedTajProfileScope::FScopedTajProfileScope(const TCHAR* InMod, const TCHAR* InCategory)
    : ModName(InMod ? InMod : TEXT("Unknown")), Category(InCategory ? InCategory : TEXT("Default"))
{
    // In a full implementation we'd check UTajsGraphStateSubsystem here for CPU tracking enable
    Start = FPlatformTime::Seconds();
}

FScopedTajProfileScope::~FScopedTajProfileScope()
{
    if (Start > 0.0)
    {
        const double End = FPlatformTime::Seconds();
        FTajRuntimeProfiler::Get().RecordCpuScope(ModName, Category, End - Start);
    }
}
