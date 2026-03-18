#pragma once

#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"
#include "Misc/DateTime.h"
#include "RHI.h"
#include "RHIResources.h"
#include "TajsGraphProfiler.generated.h"

// -------------------------------------------------------------------------
// Ring Buffer Template
// -------------------------------------------------------------------------
template <typename T>
class TTajsGraphRingBuffer
{
public:
    TTajsGraphRingBuffer() = default;
    explicit TTajsGraphRingBuffer(const int32 InCapacity) { Reset(InCapacity); }

    void Reset(const int32 InCapacity)
    {
        Capacity = FMath::Max(0, InCapacity);
        Data.Empty();
        if (Capacity > 0) Data.SetNum(Capacity);
        Head = 0; Count = 0;
    }

    void Push(const T& Item)
    {
        if (Capacity <= 0) return;
        Data[Head] = Item;
        Head = (Head + 1) % Capacity;
        Count = FMath::Min(Count + 1, Capacity);
    }

    void ToArray(TArray<T>& OutItems) const
    {
        OutItems.Reset(Count);
        if (Count <= 0 || Capacity <= 0) return;

        const int32 Start = (Count == Capacity) ? Head : 0;
        for (int32 Index = 0; Index < Count; ++Index)
        {
            OutItems.Add(Data[(Start + Index) % Capacity]);
        }
    }
private:
    TArray<T> Data;
    int32 Capacity = 0;
    int32 Head = 0;
    int32 Count = 0;
};

// -------------------------------------------------------------------------
// Profiler Structures Exposed to Blueprint
// -------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphPerfSample
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    double Timestamp = 0.0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float FrameMs = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float FPS = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float GpuMs = -1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float CpuMs = -1.0f;
};

// -------------------------------------------------------------------------
// Runtime Profiler
// -------------------------------------------------------------------------

class TAJSGRAPH_API FTajRuntimeProfiler
{
public:
    static FTajRuntimeProfiler& Get();

    void RecordCpuScope(const FString& ModName, const FString& Category, double Seconds);
    
    // Call this per frame to log the actual perf sample into the ring buffer
    void TickPerFrame(UObject* WorldContextObject);

    // Fetch the recent array of samples for Blueprint UI charting
    TArray<FTajsGraphPerfSample> GetRecentPerfSamples() const;

    // GPU Query mapping
    int32 BeginGpuQuery(const FString& Name);
    void EndGpuQuery(int32 QueryId);

private:
    FTajRuntimeProfiler();
    ~FTajRuntimeProfiler() = default;

    mutable FCriticalSection Lock;

    struct FTajGpuQuery
    {
        // FRenderQueryRHIRef StartQuery;
        // FRenderQueryRHIRef EndQuery;
        FString Name;
        uint64 FrameNumber = 0;
    };

    TTajsGraphRingBuffer<FTajsGraphPerfSample> PerfHistory;

    double LastSampleTime = 0.0;
};

/** RAII scope for profiling a block of work on the CPU */
struct FScopedTajProfileScope
{
    FScopedTajProfileScope(const TCHAR* InMod, const TCHAR* InCategory);
    ~FScopedTajProfileScope();

private:
    double Start;
    FString ModName;
    FString Category;
};
