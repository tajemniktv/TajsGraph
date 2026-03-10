#pragma once

#include "CoreMinimal.h"
#include "TajsGraphDebugTypes.generated.h"

UENUM(BlueprintType)
enum class ETajsGraphVisMode : uint8
{
    Default UMETA(DisplayName = "Default"),
    BaseColor UMETA(DisplayName = "BaseColor"),
    WorldNormal UMETA(DisplayName = "WorldNormal"),
    Roughness UMETA(DisplayName = "Roughness"),
    Metallic UMETA(DisplayName = "Metallic"),
    Specular UMETA(DisplayName = "Specular"),
    AmbientOcclusion UMETA(DisplayName = "AmbientOcclusion"),
    SceneDepth UMETA(DisplayName = "SceneDepth"),
    LuminanceHeatmap UMETA(DisplayName = "LuminanceHeatmap"),
    PseudoWireframe UMETA(DisplayName = "PseudoWireframe"),
    LumenCards UMETA(DisplayName = "LumenCards"),
    MeshDistanceFields UMETA(DisplayName = "MeshDistanceFields"),
    GlobalDistanceField UMETA(DisplayName = "GlobalDistanceField"),
    Lit UMETA(DisplayName = "Lit"),
    Unlit UMETA(DisplayName = "Unlit"),
    Wireframe UMETA(DisplayName = "Wireframe"),
    DetailLighting UMETA(DisplayName = "DetailLighting"),
    LightingOnly UMETA(DisplayName = "LightingOnly"),
    Reflections UMETA(DisplayName = "Reflections"),
    ShaderComplexity UMETA(DisplayName = "ShaderComplexity")
};

UENUM(BlueprintType)
enum class ETajsGraphOverlayPage : uint8
{
    Perf UMETA(DisplayName = "Perf"),
    Renderer UMETA(DisplayName = "Renderer"),
    PostProcess UMETA(DisplayName = "PostProcess"),
    Lights UMETA(DisplayName = "Lights"),
    Warnings UMETA(DisplayName = "Warnings")
};

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
    bool bHitch = false;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float P95Ms = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float P99Ms = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 ResX = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 ResY = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float ScreenPct = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float GpuMs = -1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float CpuMs = -1.0f;
};

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphRenderStateSample
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    double Timestamp = 0.0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString RHIName;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString FeatureLevel;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString ShaderModel;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString WindowMode;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 ViewRectX = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 ViewRectY = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 ViewRectW = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 ViewRectH = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 ResX = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 ResY = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float ScreenPercentage = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    bool bUpscalerActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    bool bLumenActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    bool bNaniteEnabled = false;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    bool bVSMEnabled = false;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 ShowFlagsHash = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TMap<FString, float> FinalPPWatchedValues;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TArray<FString> ActiveBlendables;
};

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphSceneProbeOffender
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString ActorName;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString ActorClass;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float Distance = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float Radius = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float Intensity = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    bool bCastShadows = false;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float Score = 0.0f;
};

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphSceneProbeSample
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    double Timestamp = 0.0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 NearbyLights10m = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 NearbyLights25m = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 NearbyLights50m = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 NearbyShadowCasters = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 NearbyPointLights = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 NearbySpotLights = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 NearbyRectLights = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float OverlapScore = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 PrimitiveCountApprox = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    int32 NaniteMeshCountApprox = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float TexturePoolMB = -1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float SystemUsedMB = -1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float SystemTotalMB = -1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TArray<FTajsGraphSceneProbeOffender> TopLights;
};

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphDebugEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    double Timestamp = 0.0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString Category;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString Message;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TMap<FString, FString> KeyValues;
};

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphPPDiffEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString Field;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float Baseline = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float Current = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float Delta = 0.0f;
};

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphPPProvenanceEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString Field;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float RequestedValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float FinalValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString LastWriterGuess;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    float Confidence = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    bool bInferred = true;
};

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphDebugSnapshot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString SnapshotId;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString Reason;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    double Timestamp = 0.0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FTajsGraphRenderStateSample CurrentRenderState;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FTajsGraphSceneProbeSample CurrentProbe;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TMap<FString, FString> Metadata;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TArray<FTajsGraphPerfSample> PerfHistory;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TArray<FTajsGraphRenderStateSample> RenderHistory;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TArray<FTajsGraphSceneProbeSample> ProbeHistory;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TArray<FTajsGraphDebugEvent> RecentEvents;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TArray<FTajsGraphPPDiffEntry> PostProcessDiff;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    TArray<FTajsGraphPPProvenanceEntry> PostProcessProvenance;
};

template <typename T>
class TTajsGraphRingBuffer
{
public:
    TTajsGraphRingBuffer() = default;

    explicit TTajsGraphRingBuffer(const int32 InCapacity)
    {
        Reset(InCapacity);
    }

    void Reset(const int32 InCapacity)
    {
        Capacity = FMath::Max(0, InCapacity);
        Data.Empty();
        if (Capacity > 0)
        {
            Data.SetNum(Capacity);
        }
        Head = 0;
        Count = 0;
    }

    int32 Num() const
    {
        return Count;
    }

    int32 GetCapacity() const
    {
        return Capacity;
    }

    bool IsValid() const
    {
        return Capacity > 0;
    }

    void Push(const T &Item)
    {
        if (Capacity <= 0)
        {
            return;
        }
        Data[Head] = Item;
        Head = (Head + 1) % Capacity;
        Count = FMath::Min(Count + 1, Capacity);
    }

    void ToArray(TArray<T> &OutItems) const
    {
        OutItems.Reset(Count);
        if (Count <= 0 || Capacity <= 0)
        {
            return;
        }

        const int32 Start = (Count == Capacity) ? Head : 0;
        for (int32 Index = 0; Index < Count; ++Index)
        {
            const int32 Actual = (Start + Index) % Capacity;
            OutItems.Add(Data[Actual]);
        }
    }

    bool TryGetLatest(T &OutItem) const
    {
        if (Count <= 0 || Capacity <= 0)
        {
            return false;
        }

        const int32 LatestIndex = (Head - 1 + Capacity) % Capacity;
        OutItem = Data[LatestIndex];
        return true;
    }

    void ToArrayTail(const int32 MaxItems, TArray<T> &OutItems) const
    {
        OutItems.Reset();
        if (Count <= 0 || Capacity <= 0 || MaxItems <= 0)
        {
            return;
        }

        const int32 ItemsToCopy = FMath::Min(Count, MaxItems);
        OutItems.Reserve(ItemsToCopy);

        const int32 Start = (Count == Capacity) ? Head : 0;
        const int32 TailStart = Count - ItemsToCopy;
        for (int32 Index = 0; Index < ItemsToCopy; ++Index)
        {
            const int32 Actual = (Start + TailStart + Index) % Capacity;
            OutItems.Add(Data[Actual]);
        }
    }

    void ToArraySince(const double MinTimestamp, TArray<T> &OutItems) const
    {
        TArray<T> Full;
        ToArray(Full);
        OutItems.Reset();
        for (const T &Item : Full)
        {
            if (Item.Timestamp >= MinTimestamp)
            {
                OutItems.Add(Item);
            }
        }
    }

private:
    TArray<T> Data;
    int32 Capacity = 0;
    int32 Head = 0;
    int32 Count = 0;
};
