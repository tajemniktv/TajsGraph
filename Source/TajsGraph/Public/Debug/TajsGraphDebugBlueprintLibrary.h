#pragma once

#include "CoreMinimal.h"
#include "Debug/TajsGraphDebugTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TajsGraphDebugBlueprintLibrary.generated.h"

UCLASS()
class TAJSGRAPH_API UTajsGraphDebugBlueprintLibrary : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static void SetDebugEnabled(const UObject* WorldContextObject, bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static bool IsDebugEnabled(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static void ToggleOverlay(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static void SetOverlayPage(const UObject* WorldContextObject, ETajsGraphOverlayPage Page);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static FTajsGraphDebugSnapshot CreateSnapshot(const UObject* WorldContextObject, const FString& Reason, float HistorySeconds = 10.0f);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static FTajsGraphSceneProbeSample RunSceneProbe(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static FString GenerateReport(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static void ClearHistory(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static void SetVisualizationMode(const UObject* WorldContextObject, ETajsGraphVisMode Mode);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static ETajsGraphVisMode NextVisualizationMode(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static ETajsGraphVisMode PreviousVisualizationMode(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static bool ToggleVisualization(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|Debug", meta = (WorldContext = "WorldContextObject"))
    static TArray<FString> GetOverlayLines(const UObject* WorldContextObject);
};
