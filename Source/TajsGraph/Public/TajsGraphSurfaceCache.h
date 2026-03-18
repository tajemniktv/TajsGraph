#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TajsGraphProfiler.h" // For TTajsGraphRingBuffer
#include "TajsGraphSurfaceCache.generated.h"

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphSurfaceCacheEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    double TimeSeconds = 0.0;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString Kind;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString OwnerClassName;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString ComponentClassName;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString ElementPath;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Debug")
    FString ExtraDetails;
};

/**
 * Handles all hooking for Surface Cache mutations.
 * Tracks Hologram and Paint Gun overrides to diagnose factory rebuild triggers.
 */
UCLASS()
class TAJSGRAPH_API UTajsGraphSurfaceCache : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    static void InstallHooks();
    static void UninstallHooks();

    /** Called internally by native hooks */
    static void RecordEvent(const FString& Kind, const UObject* ContextObject, const FString& ExtraDetails);

    /** Print recent hooks to the Unreal Console */
    static void DumpSurfaceCacheTrace(int32 MaxCount = 50);

    /** Fetch recent history for UMG UI binding */
    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug")
    static TArray<FTajsGraphSurfaceCacheEvent> GetRecentSurfaceCacheEvents();
};
