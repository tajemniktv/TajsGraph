#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

#include "TajsGraphFoliageAccess.generated.h"

UCLASS()
class UTajsGraphFoliageAccess : public UObject {
    GENERATED_BODY()

public:
    static void SetViewRelevance(UHierarchicalInstancedStaticMeshComponent* Component, EHISMViewRelevanceType NewType);
};
