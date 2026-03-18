#include "TajsGraphSurfaceCache.h"
#include "TajsGraphLog.h"
#include "TajsGraphStateSubsystem.h"
#include "Engine/Engine.h"

// Note: SML hooking macros would usually reside in "Patching/NativeHookManager.h"
// If compiling with WITH_SML or directly linked against SML module:
// #include "Patching/NativeHookManager.h"

static TTajsGraphRingBuffer<FTajsGraphSurfaceCacheEvent> GSurfaceCacheHistory(512);
static FCriticalSection GSurfaceCacheHistoryLock;

void UTajsGraphSurfaceCache::InstallHooks()
{
    // Implementation placeholder for native SML hooking.
    // E.g.:
    // SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
    //     void (UFGBuildGunStatePaint::*)(),
    //     UFGBuildGunStatePaint,
    //     BeginState_Implementation,
    //     [](TCallScope<void(*)(UFGBuildGunStatePaint*)>& Scope, UFGBuildGunStatePaint* Self) {
    //        Scope(Self);
    //        RecordEvent(TEXT("PaintStateBegin"), Self, TEXT(""));
    //     }
    // );
    
    UE_LOG(LogTajsGraph, Log, TEXT("Surface Cache Trace Hooks Installed."));
}

void UTajsGraphSurfaceCache::UninstallHooks()
{
    // Cleanup hooks
    UE_LOG(LogTajsGraph, Log, TEXT("Surface Cache Trace Hooks Uninstalled."));
}

void UTajsGraphSurfaceCache::RecordEvent(const FString& Kind, const UObject* ContextObject, const FString& ExtraDetails)
{
    // Retrieve tracking limits
    if (GEngine)
    {
        if (const UWorld* World = GEngine->GetWorldContexts().Num() > 0 ? GEngine->GetWorldContexts()[0].World() : nullptr)
        {
            if (UTajsGraphStateSubsystem* ConfigState = World->GetGameInstance() ? World->GetGameInstance()->GetSubsystem<UTajsGraphStateSubsystem>() : nullptr)
            {
                int32 TargetCapacity = ConfigState->GetMaxSurfaceCacheEvents();
                if (GSurfaceCacheHistory.GetCapacity() != TargetCapacity)
                {
                    FScopeLock Lock(&GSurfaceCacheHistoryLock);
                    GSurfaceCacheHistory.Reset(TargetCapacity);
                }
            }
        }
    }

    FTajsGraphSurfaceCacheEvent Ev;
    Ev.TimeSeconds = FPlatformTime::Seconds();
    Ev.Kind = Kind;
    Ev.ExtraDetails = ExtraDetails;
    
    if (ContextObject)
    {
        Ev.OwnerClassName = ContextObject->GetClass()->GetName();
        Ev.ElementPath = ContextObject->GetPathName();
    }

    {
        FScopeLock Lock(&GSurfaceCacheHistoryLock);
        GSurfaceCacheHistory.Push(Ev);
    }
}

void UTajsGraphSurfaceCache::DumpSurfaceCacheTrace(int32 MaxCount)
{
    TArray<FTajsGraphSurfaceCacheEvent> Events = GetRecentSurfaceCacheEvents();
    
    int32 StartIdx = FMath::Max(0, Events.Num() - MaxCount);
    UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph][SurfaceCacheTrace] Dumping %d of %d events"), Events.Num() - StartIdx, Events.Num());
    
    for (int32 Index = StartIdx; Index < Events.Num(); ++Index)
    {
        const FTajsGraphSurfaceCacheEvent& Ev = Events[Index];
        UE_LOG(LogTajsGraph, Display,
            TEXT("[%d] Kind=%s OwnerClass=%s Path=%s Extra=%s"),
            Index, *Ev.Kind, *Ev.OwnerClassName, *Ev.ElementPath, *Ev.ExtraDetails);
    }
}

TArray<FTajsGraphSurfaceCacheEvent> UTajsGraphSurfaceCache::GetRecentSurfaceCacheEvents()
{
    FScopeLock Lock(&GSurfaceCacheHistoryLock);
    TArray<FTajsGraphSurfaceCacheEvent> OutArray;
    GSurfaceCacheHistory.ToArray(OutArray);
    return OutArray;
}
