#pragma once

#include "CoreMinimal.h"
#include "Debug/TajsGraphDebugTypes.h"

class FJsonObject;

class TAJSGRAPH_API FTajsGraphDebugIO {
public:
    static bool WriteSnapshotJson(const FTajsGraphDebugSnapshot& Snapshot, const FString& FilePath);
    static bool WritePerfCsv(const TArray<FTajsGraphPerfSample>& Samples, const FString& FilePath);
    static bool WriteJsonObject(const TSharedPtr<FJsonObject>& JsonObject, const FString& FilePath);

    static TSharedPtr<FJsonObject> MakeSnapshotJson(const FTajsGraphDebugSnapshot& Snapshot);
    static TSharedPtr<FJsonObject> MakePerfSampleJson(const FTajsGraphPerfSample& Sample);
    static TSharedPtr<FJsonObject> MakeRenderStateJson(const FTajsGraphRenderStateSample& Sample);
    static TSharedPtr<FJsonObject> MakeSceneProbeJson(const FTajsGraphSceneProbeSample& Sample);
    static TSharedPtr<FJsonObject> MakeEventJson(const FTajsGraphDebugEvent& Event);
};
