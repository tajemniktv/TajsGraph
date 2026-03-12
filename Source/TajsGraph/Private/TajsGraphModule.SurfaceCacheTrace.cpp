#include "TajsGraphModuleInternal.h"

#include "Buildables/FGBuildable.h"
#include "Components/ActorComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Equipment/FGBuildGunPaint.h"
#include "FGPlayerController.h"
#include "FGSwatchGroup.h"
#include "Hologram/FGBuildableHologram.h"
#include "Hologram/FGHologram.h"
#include "Materials/MaterialInterface.h"

namespace TajsGraphInternal
{
    namespace
    {
        struct FSurfaceCacheTraceEvent
        {
            double TimeSeconds = 0.0;
            uint64 FrameCounter = 0;
            FString Kind;
            FString WorldName;
            FString OwnerClassName;
            FString OwnerName;
            FString ComponentClassName;
            FString ComponentName;
            FString MeshPath;
            FString MaterialPaths;
            FString Extra;
            int32 Mobility = INDEX_NONE;
            bool bAffectDistanceFieldLighting = false;
            bool bCastStaticShadow = false;
            bool bPreviewActive = false;
            bool bMutationApplied = false;
            bool bMutationSkipped = false;
            bool bColoredProxy = false;
            bool bBlockInstancing = false;
            bool bBlockInstancingWithLumen = false;
        };

        constexpr int32 GMaxSurfaceCacheTraceEvents = 512;

        FCriticalSection GSurfaceCacheTraceMutex;
        TArray<FSurfaceCacheTraceEvent> GSurfaceCacheTraceEvents;
        FTajsGraphSurfaceCachePreviewState GSurfaceCachePreviewState;

        const TCHAR* BoolText(const bool bValue)
        {
            return bValue ? TEXT("true") : TEXT("false");
        }

        FString TraceKindToString(const ETajsGraphSurfaceCacheTraceKind Kind)
        {
            switch (Kind)
            {
            case ETajsGraphSurfaceCacheTraceKind::PaintStateBegin:
                return TEXT("PaintStateBegin");
            case ETajsGraphSurfaceCacheTraceKind::PaintStateEnd:
                return TEXT("PaintStateEnd");
            case ETajsGraphSurfaceCacheTraceKind::ActiveSwatchChanged:
                return TEXT("ActiveSwatchChanged");
            case ETajsGraphSurfaceCacheTraceKind::ActivePatternChanged:
                return TEXT("ActivePatternChanged");
            case ETajsGraphSurfaceCacheTraceKind::ActiveMaterialChanged:
                return TEXT("ActiveMaterialChanged");
            case ETajsGraphSurfaceCacheTraceKind::ActiveSkinChanged:
                return TEXT("ActiveSkinChanged");
            case ETajsGraphSurfaceCacheTraceKind::SwatchGroupDefaultChanged:
                return TEXT("SwatchGroupDefaultChanged");
            case ETajsGraphSurfaceCacheTraceKind::BuildableCustomizationApplied:
                return TEXT("BuildableCustomizationApplied");
            case ETajsGraphSurfaceCacheTraceKind::BuildablePrimitiveDataApplied:
                return TEXT("BuildablePrimitiveDataApplied");
            case ETajsGraphSurfaceCacheTraceKind::HologramCustomizationApplied:
                return TEXT("HologramCustomizationApplied");
            case ETajsGraphSurfaceCacheTraceKind::StaticMeshRemapAttempt:
                return TEXT("StaticMeshRemapAttempt");
            case ETajsGraphSurfaceCacheTraceKind::StaticMeshRemapSkipped:
                return TEXT("StaticMeshRemapSkipped");
            case ETajsGraphSurfaceCacheTraceKind::NaniteOverrideAttempt:
                return TEXT("NaniteOverrideAttempt");
            case ETajsGraphSurfaceCacheTraceKind::PreviewGuardEntered:
                return TEXT("PreviewGuardEntered");
            case ETajsGraphSurfaceCacheTraceKind::PreviewGuardExited:
                return TEXT("PreviewGuardExited");
            default:
                return TEXT("Unknown");
            }
        }

        bool IsPreviewActive_NoLock(const double NowSeconds)
        {
            return GSurfaceCachePreviewState.bPaintStateActive ||
                ((NowSeconds - GSurfaceCachePreviewState.LastMutationTimeSeconds) <= GSurfaceCachePreviewGraceSeconds);
        }

        FString SummarizeMaterials(const UMeshComponent* MeshComponent)
        {
            if (!MeshComponent)
            {
                return FString();
            }

            TArray<FString> Materials;
            const int32 MaterialCount = MeshComponent->GetNumMaterials();
            const int32 MaxMaterialsToLog = FMath::Min(MaterialCount, 4);
            for (int32 MaterialIndex = 0; MaterialIndex < MaxMaterialsToLog; ++MaterialIndex)
            {
                if (const UMaterialInterface* Material = MeshComponent->GetMaterial(MaterialIndex))
                {
                    Materials.Add(Material->GetPathName());
                }
            }

            if (MaterialCount > MaxMaterialsToLog)
            {
                Materials.Add(FString::Printf(TEXT("...+%d"), MaterialCount - MaxMaterialsToLog));
            }

            return FString::Join(Materials, TEXT(";"));
        }

        bool IsPreviewRelevantOwner(const AActor* Owner)
        {
            return Owner &&
                (Owner->IsA<AFGBuildable>() ||
                 Owner->IsA<AFGHologram>() ||
                 Owner->HasAnyFlags(RF_Transient) ||
                 Owner->GetName().Contains(TEXT("Preview"), ESearchCase::IgnoreCase));
        }

        bool IsPreviewRelevantComponent(const UActorComponent* Component)
        {
            if (!Component)
            {
                return false;
            }

            if (Component->HasAnyFlags(RF_Transient) || Component->GetName().Contains(TEXT("Preview"), ESearchCase::IgnoreCase))
            {
                return true;
            }

            return IsPreviewRelevantOwner(Component->GetOwner());
        }

        const UStaticMeshComponent* FindRepresentativeStaticMeshComponent(const AActor* Actor)
        {
            return Actor ? Actor->FindComponentByClass<UStaticMeshComponent>() : nullptr;
        }

        void AppendTraceEvent(const FSurfaceCacheTraceEvent& Event)
        {
            if (GSurfaceCacheTraceEvents.Num() >= GMaxSurfaceCacheTraceEvents)
            {
                GSurfaceCacheTraceEvents.RemoveAt(0, 1, false);
            }

            GSurfaceCacheTraceEvents.Add(Event);
        }
    }

    bool ShouldSurfaceCacheTraceLog()
    {
        FScopeLock Lock(&GPPVConfigMutex);
        return GPPVConfig.bEnableSurfaceCacheTrace;
    }

    bool IsSurfaceCachePreviewActive()
    {
        FScopeLock Lock(&GSurfaceCacheTraceMutex);
        return IsPreviewActive_NoLock(FPlatformTime::Seconds());
    }

    FTajsGraphSurfaceCachePreviewState GetSurfaceCachePreviewStateSnapshot()
    {
        FScopeLock Lock(&GSurfaceCacheTraceMutex);
        return GSurfaceCachePreviewState;
    }

    bool ShouldGuardSurfaceCacheMutation(const UActorComponent* Component, FString* OutReason)
    {
        if (!Component)
        {
            return false;
        }

        FString Reason;
        bool bShouldGuard = false;
        {
            FScopeLock Lock(&GSurfaceCacheTraceMutex);
            if (IsPreviewActive_NoLock(FPlatformTime::Seconds()) && IsPreviewRelevantComponent(Component))
            {
                bShouldGuard = true;

                if (const AActor* Owner = Component->GetOwner())
                {
                    if (Owner->IsA<AFGHologram>())
                    {
                        Reason = TEXT("Active preview window with hologram owner");
                    }
                    else if (Owner->HasAnyFlags(RF_Transient))
                    {
                        Reason = TEXT("Active preview window with transient preview owner");
                    }
                    else
                    {
                        Reason = TEXT("Active preview window affecting buildable registration");
                    }
                }
                else
                {
                    Reason = TEXT("Active preview window affecting component registration");
                }
            }
        }

        if (OutReason)
        {
            *OutReason = Reason;
        }

        return bShouldGuard;
    }

    void AddSurfaceCacheTrace(const ETajsGraphSurfaceCacheTraceKind Kind, const UObject* ContextObject, const UStaticMeshComponent* Component, const FString& Extra, const bool bMutationApplied, const bool bMutationSkipped)
    {
        const bool bCapture = ShouldSurfaceCacheTraceLog() || IsSurfaceCachePreviewActive();
        if (!bCapture)
        {
            return;
        }

        const UActorComponent* ContextComponent = Component ? Cast<UActorComponent>(Component) : Cast<UActorComponent>(ContextObject);
        const AActor* Owner = Component ? Component->GetOwner() : (ContextComponent ? ContextComponent->GetOwner() : Cast<AActor>(ContextObject));
        const UMeshComponent* MeshComponent = Component ? Cast<UMeshComponent>(Component) : Cast<UMeshComponent>(ContextComponent);
        const UFGColoredInstanceMeshProxy* ColoredProxy = Component ? Cast<UFGColoredInstanceMeshProxy>(Component) : Cast<UFGColoredInstanceMeshProxy>(ContextComponent);
        const USceneComponent* SceneComponent = Component ? Cast<USceneComponent>(Component) : Cast<USceneComponent>(ContextComponent);
        const UWorld* TraceWorld = Component ? Component->GetWorld() : (ContextComponent ? ContextComponent->GetWorld() : (Owner ? Owner->GetWorld() : nullptr));
        const UClass* OwnerClass = Owner ? Owner->GetClass() : nullptr;
        const UClass* ComponentClass = ContextComponent ? ContextComponent->GetClass() : nullptr;
        const UStaticMesh* StaticMesh = Component ? Component->GetStaticMesh() : nullptr;
        const UClass* ContextClass = ContextObject ? ContextObject->GetClass() : nullptr;

        FSurfaceCacheTraceEvent Event;
        Event.TimeSeconds = FPlatformTime::Seconds();
        Event.FrameCounter = static_cast<uint64>(GFrameCounter);
        Event.Kind = TraceKindToString(Kind);
        Event.WorldName = TraceWorld ? TraceWorld->GetName() : TEXT("<null>");
        Event.OwnerClassName = OwnerClass ? OwnerClass->GetName() : (ContextClass ? ContextClass->GetName() : TEXT("<null>"));
        Event.OwnerName = Owner ? Owner->GetName() : (ContextObject ? ContextObject->GetName() : TEXT("<null>"));
        Event.ComponentClassName = ComponentClass ? ComponentClass->GetName() : (ContextClass ? ContextClass->GetName() : TEXT("<null>"));
        Event.ComponentName = ContextComponent ? ContextComponent->GetName() : (ContextObject ? ContextObject->GetName() : TEXT("<null>"));
        Event.MeshPath = StaticMesh ? StaticMesh->GetPathName() : TEXT("<null>");
        Event.MaterialPaths = SummarizeMaterials(MeshComponent);
        Event.Extra = Extra;
        Event.Mobility = SceneComponent ? static_cast<int32>(SceneComponent->Mobility) : INDEX_NONE;
        Event.bAffectDistanceFieldLighting = Component ? Component->bAffectDistanceFieldLighting : false;
        Event.bCastStaticShadow = Component ? Component->bCastStaticShadow : false;
        Event.bMutationApplied = bMutationApplied;
        Event.bMutationSkipped = bMutationSkipped;
        Event.bColoredProxy = ColoredProxy != nullptr;
        Event.bBlockInstancing = ColoredProxy ? ColoredProxy->mBlockInstancing : false;
        Event.bBlockInstancingWithLumen = ColoredProxy ? ColoredProxy->mBlockInstancingWithLumen : false;

        {
            FScopeLock Lock(&GSurfaceCacheTraceMutex);
            Event.bPreviewActive = IsPreviewActive_NoLock(Event.TimeSeconds);
            AppendTraceEvent(Event);
        }

        if (ShouldSurfaceCacheTraceLog())
        {
            UE_LOG(LogTajsGraph, Log,
                TEXT("[TajsGraph][SurfaceCacheTrace] Kind=%s Frame=%llu World=%s Owner=%s:%s Component=%s:%s Mesh=%s Materials=%s Mobility=%d AffectDF=%s CastStaticShadow=%s PreviewActive=%s Applied=%s Skipped=%s Proxy=%s BlockInstancing=%s BlockInstancingWithLumen=%s Extra=%s"),
                *Event.Kind,
                Event.FrameCounter,
                *Event.WorldName,
                *Event.OwnerClassName,
                *Event.OwnerName,
                *Event.ComponentClassName,
                *Event.ComponentName,
                *Event.MeshPath,
                *Event.MaterialPaths,
                Event.Mobility,
                BoolText(Event.bAffectDistanceFieldLighting),
                BoolText(Event.bCastStaticShadow),
                BoolText(Event.bPreviewActive),
                BoolText(Event.bMutationApplied),
                BoolText(Event.bMutationSkipped),
                BoolText(Event.bColoredProxy),
                BoolText(Event.bBlockInstancing),
                BoolText(Event.bBlockInstancingWithLumen),
                *Event.Extra);
        }
    }

    void TouchSurfaceCachePreviewActivity(const UObject* ContextObject, const TCHAR* Source)
    {
        {
            FScopeLock Lock(&GSurfaceCacheTraceMutex);
            GSurfaceCachePreviewState.LastMutationTimeSeconds = FPlatformTime::Seconds();
            GSurfaceCachePreviewState.LastSource = Source ? Source : TEXT("Touch");
        }
    }

    void MarkSurfaceCachePaintState(const UObject* ContextObject, const bool bActive, const TCHAR* Source)
    {
        {
            FScopeLock Lock(&GSurfaceCacheTraceMutex);
            GSurfaceCachePreviewState.bPaintStateActive = bActive;
            GSurfaceCachePreviewState.LastMutationTimeSeconds = FPlatformTime::Seconds();
            GSurfaceCachePreviewState.LastSource = Source ? Source : (bActive ? TEXT("PaintStateBegin") : TEXT("PaintStateEnd"));
        }

        AddSurfaceCacheTrace(
            bActive ? ETajsGraphSurfaceCacheTraceKind::PaintStateBegin : ETajsGraphSurfaceCacheTraceKind::PaintStateEnd,
            ContextObject,
            nullptr,
            Source ? Source : TEXT("PaintState"),
            false,
            false);

        AddSurfaceCacheTrace(
            bActive ? ETajsGraphSurfaceCacheTraceKind::PreviewGuardEntered : ETajsGraphSurfaceCacheTraceKind::PreviewGuardExited,
            ContextObject,
            nullptr,
            Source ? Source : TEXT("PaintState"),
            false,
            false);
    }

    void MarkSurfaceCachePreviewDescriptor(const UObject* ContextObject, const ETajsGraphSurfaceCacheTraceKind Kind, const FString& DescriptorPath, const TCHAR* Source)
    {
        {
            FScopeLock Lock(&GSurfaceCacheTraceMutex);
            GSurfaceCachePreviewState.LastMutationTimeSeconds = FPlatformTime::Seconds();
            GSurfaceCachePreviewState.LastSource = Source ? FString(Source) : TraceKindToString(Kind);

            switch (Kind)
            {
            case ETajsGraphSurfaceCacheTraceKind::ActiveSwatchChanged:
                GSurfaceCachePreviewState.ActiveSwatchDesc = DescriptorPath;
                break;
            case ETajsGraphSurfaceCacheTraceKind::ActivePatternChanged:
                GSurfaceCachePreviewState.ActivePatternDesc = DescriptorPath;
                break;
            case ETajsGraphSurfaceCacheTraceKind::ActiveMaterialChanged:
                GSurfaceCachePreviewState.ActiveMaterialDesc = DescriptorPath;
                break;
            case ETajsGraphSurfaceCacheTraceKind::ActiveSkinChanged:
                GSurfaceCachePreviewState.ActiveSkinDesc = DescriptorPath;
                break;
            default:
                break;
            }
        }

        AddSurfaceCacheTrace(Kind, ContextObject, nullptr, FString::Printf(TEXT("%s Descriptor=%s"), Source ? Source : TEXT("PreviewDescriptor"), *DescriptorPath), false, false);
    }

    void DumpSurfaceCacheTrace(const int32 MaxCount)
    {
        TArray<FSurfaceCacheTraceEvent> Events;
        {
            FScopeLock Lock(&GSurfaceCacheTraceMutex);
            Events = GSurfaceCacheTraceEvents;
        }

        const int32 ClampedMaxCount = MaxCount > 0 ? MaxCount : 50;
        const int32 StartIndex = FMath::Max(0, Events.Num() - ClampedMaxCount);

        UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph][SurfaceCacheTrace] Dumping %d of %d events"), Events.Num() - StartIndex, Events.Num());
        for (int32 Index = StartIndex; Index < Events.Num(); ++Index)
        {
            const FSurfaceCacheTraceEvent& Event = Events[Index];
            UE_LOG(LogTajsGraph, Display,
                TEXT("[TajsGraph][SurfaceCacheTrace][%d] Kind=%s Frame=%llu Owner=%s:%s Component=%s:%s Mesh=%s PreviewActive=%s Applied=%s Skipped=%s Extra=%s"),
                Index,
                *Event.Kind,
                Event.FrameCounter,
                *Event.OwnerClassName,
                *Event.OwnerName,
                *Event.ComponentClassName,
                *Event.ComponentName,
                *Event.MeshPath,
                BoolText(Event.bPreviewActive),
                BoolText(Event.bMutationApplied),
                BoolText(Event.bMutationSkipped),
                *Event.Extra);
        }
    }

    void ClearSurfaceCacheTrace()
    {
        FScopeLock Lock(&GSurfaceCacheTraceMutex);
        GSurfaceCacheTraceEvents.Reset();
    }

    void InstallSurfaceCacheHooks()
    {
        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (UFGBuildGunStatePaint::*)(),
            UFGBuildGunStatePaint,
            BeginState_Implementation,
            [](TCallScope<void(*)(UFGBuildGunStatePaint*)>& Scope, UFGBuildGunStatePaint* Self)
            {
                Scope(Self);
                if (Self)
                {
                    MarkSurfaceCachePaintState(Self, true, TEXT("UFGBuildGunStatePaint::BeginState_Implementation"));
                }
            });

        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (UFGBuildGunStatePaint::*)(),
            UFGBuildGunStatePaint,
            EndState_Implementation,
            [](TCallScope<void(*)(UFGBuildGunStatePaint*)>& Scope, UFGBuildGunStatePaint* Self)
            {
                Scope(Self);
                if (Self)
                {
                    MarkSurfaceCachePaintState(Self, false, TEXT("UFGBuildGunStatePaint::EndState_Implementation"));
                }
            });

        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (UFGBuildGunStatePaint::*)(TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>),
            UFGBuildGunStatePaint,
            SetActiveSwatchDesc,
            [](TCallScope<void(*)(UFGBuildGunStatePaint*, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>)>& Scope, UFGBuildGunStatePaint* Self, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch> SwatchDesc)
            {
                Scope(Self, SwatchDesc);
                if (Self)
                {
                    MarkSurfaceCachePreviewDescriptor(Self, ETajsGraphSurfaceCacheTraceKind::ActiveSwatchChanged, SwatchDesc ? SwatchDesc->GetPathName() : TEXT("<null>"), TEXT("UFGBuildGunStatePaint::SetActiveSwatchDesc"));
                }
            });

        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (UFGBuildGunStatePaint::*)(TSubclassOf<UFGFactoryCustomizationDescriptor_Pattern>),
            UFGBuildGunStatePaint,
            SetActivePatternDesc,
            [](TCallScope<void(*)(UFGBuildGunStatePaint*, TSubclassOf<UFGFactoryCustomizationDescriptor_Pattern>)>& Scope, UFGBuildGunStatePaint* Self, TSubclassOf<UFGFactoryCustomizationDescriptor_Pattern> PatternDesc)
            {
                Scope(Self, PatternDesc);
                if (Self)
                {
                    MarkSurfaceCachePreviewDescriptor(Self, ETajsGraphSurfaceCacheTraceKind::ActivePatternChanged, PatternDesc ? PatternDesc->GetPathName() : TEXT("<null>"), TEXT("UFGBuildGunStatePaint::SetActivePatternDesc"));
                }
            });

        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (UFGBuildGunStatePaint::*)(TSubclassOf<UFGFactoryCustomizationDescriptor_Material>),
            UFGBuildGunStatePaint,
            SetActiveMaterialDesc,
            [](TCallScope<void(*)(UFGBuildGunStatePaint*, TSubclassOf<UFGFactoryCustomizationDescriptor_Material>)>& Scope, UFGBuildGunStatePaint* Self, TSubclassOf<UFGFactoryCustomizationDescriptor_Material> MaterialDesc)
            {
                Scope(Self, MaterialDesc);
                if (Self)
                {
                    MarkSurfaceCachePreviewDescriptor(Self, ETajsGraphSurfaceCacheTraceKind::ActiveMaterialChanged, MaterialDesc ? MaterialDesc->GetPathName() : TEXT("<null>"), TEXT("UFGBuildGunStatePaint::SetActiveMaterialDesc"));
                }
            });

        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (UFGBuildGunStatePaint::*)(TSubclassOf<UFGFactoryCustomizationDescriptor_Skin>),
            UFGBuildGunStatePaint,
            SetActiveSkinDesc,
            [](TCallScope<void(*)(UFGBuildGunStatePaint*, TSubclassOf<UFGFactoryCustomizationDescriptor_Skin>)>& Scope, UFGBuildGunStatePaint* Self, TSubclassOf<UFGFactoryCustomizationDescriptor_Skin> SkinDesc)
            {
                Scope(Self, SkinDesc);
                if (Self)
                {
                    MarkSurfaceCachePreviewDescriptor(Self, ETajsGraphSurfaceCacheTraceKind::ActiveSkinChanged, SkinDesc ? SkinDesc->GetPathName() : TEXT("<null>"), TEXT("UFGBuildGunStatePaint::SetActiveSkinDesc"));
                }
            });

        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (AFGPlayerController::*)(TSubclassOf<UFGSwatchGroup>, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>),
            AFGPlayerController,
            SetDefaultSwatchForBuildableGroup,
            [](TCallScope<void(*)(AFGPlayerController*, TSubclassOf<UFGSwatchGroup>, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>)>& Scope, AFGPlayerController* Self, TSubclassOf<UFGSwatchGroup> SwatchGroup, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch> SwatchDesc)
            {
                Scope(Self, SwatchGroup, SwatchDesc);
                if (Self)
                {
                    const FString Extra = FString::Printf(TEXT("Source=AFGPlayerController::SetDefaultSwatchForBuildableGroup Group=%s Swatch=%s"),
                        SwatchGroup ? *SwatchGroup->GetPathName() : TEXT("<null>"),
                        SwatchDesc ? *SwatchDesc->GetPathName() : TEXT("<null>"));
                    TouchSurfaceCachePreviewActivity(Self, TEXT("AFGPlayerController::SetDefaultSwatchForBuildableGroup"));
                    AddSurfaceCacheTrace(ETajsGraphSurfaceCacheTraceKind::SwatchGroupDefaultChanged, Self, nullptr, Extra, false, false);
                }
            });

        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (AFGPlayerController::*)(TSubclassOf<UFGSwatchGroup>, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>),
            AFGPlayerController,
            Server_SetDefaultSwatchForBuildableGroup_Implementation,
            [](TCallScope<void(*)(AFGPlayerController*, TSubclassOf<UFGSwatchGroup>, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>)>& Scope, AFGPlayerController* Self, TSubclassOf<UFGSwatchGroup> SwatchGroup, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch> SwatchDesc)
            {
                Scope(Self, SwatchGroup, SwatchDesc);
                if (Self)
                {
                    const FString Extra = FString::Printf(TEXT("Source=AFGPlayerController::Server_SetDefaultSwatchForBuildableGroup_Implementation Group=%s Swatch=%s"),
                        SwatchGroup ? *SwatchGroup->GetPathName() : TEXT("<null>"),
                        SwatchDesc ? *SwatchDesc->GetPathName() : TEXT("<null>"));
                    TouchSurfaceCachePreviewActivity(Self, TEXT("AFGPlayerController::Server_SetDefaultSwatchForBuildableGroup_Implementation"));
                    AddSurfaceCacheTrace(ETajsGraphSurfaceCacheTraceKind::SwatchGroupDefaultChanged, Self, nullptr, Extra, false, false);
                }
            });

        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (AFGBuildable::*)(const FFactoryCustomizationData&),
            AFGBuildable,
            ApplyCustomizationData_Native,
            [](TCallScope<void(*)(AFGBuildable*, const FFactoryCustomizationData&)>& Scope, AFGBuildable* Self, const FFactoryCustomizationData& CustomizationData)
            {
                Scope(Self, CustomizationData);
                if (Self)
                {
                    const bool bPreviewActive = IsSurfaceCachePreviewActive();
                    const UStaticMeshComponent* RepresentativeComponent = FindRepresentativeStaticMeshComponent(Self);
                    if (bPreviewActive)
                    {
                        TouchSurfaceCachePreviewActivity(Self, TEXT("AFGBuildable::ApplyCustomizationData_Native"));
                        const FString Extra = FString::Printf(TEXT("Source=AFGBuildable::ApplyCustomizationData_Native ColorSlot=%d Swatch=%s Pattern=%s Skin=%s"),
                            CustomizationData.ColorSlot,
                            CustomizationData.SwatchDesc ? *CustomizationData.SwatchDesc->GetPathName() : TEXT("<null>"),
                            CustomizationData.PatternDesc ? *CustomizationData.PatternDesc->GetPathName() : TEXT("<null>"),
                            CustomizationData.SkinDesc ? *CustomizationData.SkinDesc->GetPathName() : TEXT("<null>"));
                        AddSurfaceCacheTrace(ETajsGraphSurfaceCacheTraceKind::BuildableCustomizationApplied, Self, RepresentativeComponent, Extra, false, false);
                    }
                }
            });
        SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
            void (AFGBuildableHologram::*)(const FFactoryCustomizationData&),
            AFGBuildableHologram,
            SetCustomizationData,
            [](TCallScope<void(*)(AFGBuildableHologram*, const FFactoryCustomizationData&)>& Scope, AFGBuildableHologram* Self, const FFactoryCustomizationData& CustomizationData)
            {
                Scope(Self, CustomizationData);
                if (Self)
                {
                    const bool bPreviewActive = IsSurfaceCachePreviewActive();
                    const UStaticMeshComponent* RepresentativeComponent = FindRepresentativeStaticMeshComponent(Self);
                    if (bPreviewActive)
                    {
                        TouchSurfaceCachePreviewActivity(Self, TEXT("AFGBuildableHologram::SetCustomizationData"));
                        AddSurfaceCacheTrace(ETajsGraphSurfaceCacheTraceKind::HologramCustomizationApplied, Self, RepresentativeComponent, TEXT("Source=AFGBuildableHologram::SetCustomizationData"), false, false);
                    }
                }
            });

    }
}
