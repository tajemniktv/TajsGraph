#include "TajsGraphModuleInternal.h"
#include "Buildables/FGBuildable.h"

namespace TajsGraphInternal {

namespace {

bool IsAbstractInstanceManagerOwner(const AActor* Owner) {
    return Owner && Owner->GetClass() &&
        (Owner->GetClass()->GetFName() == FName(TEXT("AbstractInstanceManager")) ||
         Owner->GetClass()->GetName().Contains(TEXT("AbstractInstanceManager")));
}

}

bool IsBuildableOwnedActor(const AActor* Owner) {
    return Owner && (Owner->IsA<AFGBuildable>() || IsAbstractInstanceManagerOwner(Owner));
}

void ApplyNaniteOverrides(UStaticMeshComponent* Component, const FPPVConfig& Config, const TCHAR* LogContext) {
    if (!Component || !Config.bForceNanite) {
        return;
    }

    AddSurfaceCacheTrace(
        ETajsGraphSurfaceCacheTraceKind::NaniteOverrideAttempt,
        Component,
        Component,
        FString::Printf(TEXT("Context=%s Mesh=%s"), LogContext ? LogContext : TEXT("Component"), Component->GetStaticMesh() ? *Component->GetStaticMesh()->GetPathName() : TEXT("<null>")),
        false,
        false);

    Component->bDisallowNanite = false;
    Component->bForceDisableNanite = false;
    if (Config.bForceNaniteForMasked) {
        Component->bForceNaniteForMasked = true;
    }

    const UStaticMesh* Mesh = Component->GetStaticMesh();
    if (!Mesh || Mesh->HasValidNaniteData() || !ShouldGeneralLog()) {
        return;
    }

    const AActor* Owner = Component->GetOwner();
    UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph][Nanite][%s] Mesh has no Nanite data: %s (OwnerClass=%s OwnerName=%s)"),
        LogContext ? LogContext : TEXT("Component"),
        *Mesh->GetName(),
        Owner ? *Owner->GetClass()->GetName() : TEXT("<null>"),
        Owner ? *Owner->GetName() : TEXT("<null>"));
}

}
