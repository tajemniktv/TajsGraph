#include "TajsGraphFoliageAccess.h"

class FTajsGraphHISMAccessor : public UHierarchicalInstancedStaticMeshComponent {
public:
    static void SetViewRelevance(UHierarchicalInstancedStaticMeshComponent* Component, EHISMViewRelevanceType NewType) {
        EHISMViewRelevanceType UHierarchicalInstancedStaticMeshComponent::* ViewRelevanceMember = &FTajsGraphHISMAccessor::ViewRelevanceType;
        Component->*ViewRelevanceMember = NewType;
    }
};

void UTajsGraphFoliageAccess::SetViewRelevance(UHierarchicalInstancedStaticMeshComponent* Component, EHISMViewRelevanceType NewType) {
    if (Component) {
        FTajsGraphHISMAccessor::SetViewRelevance(Component, NewType);
    }
}
