#pragma once

#include "CoreMinimal.h"
#include "Debug/TajsGraphDebugTypes.h"
#include "SceneViewExtension.h"

class UTajsGraphDebugSubsystem;

class TAJSGRAPH_API FTajsGraphDebugViewExtension : public FSceneViewExtensionBase {
public:
    FTajsGraphDebugViewExtension(const FAutoRegister& AutoRegister, TWeakObjectPtr<UTajsGraphDebugSubsystem> InOwnerSubsystem);

    virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
    virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
    virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
    virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const override;

    void DrainRenderStateSamples(TArray<FTajsGraphRenderStateSample>& OutSamples);

private:
    FTajsGraphRenderStateSample BuildRenderStateSample(const FSceneViewFamily& InViewFamily) const;

private:
    mutable FCriticalSection QueueMutex;
    TArray<FTajsGraphRenderStateSample> PendingSamples;
    TWeakObjectPtr<UTajsGraphDebugSubsystem> OwnerSubsystem;
};
