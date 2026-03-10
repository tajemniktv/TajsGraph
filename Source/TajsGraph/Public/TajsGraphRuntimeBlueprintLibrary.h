#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TajsGraphRuntimeBlueprintLibrary.generated.h"

UCLASS()
class TAJSGRAPH_API UTajsGraphRuntimeBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Runtime", meta = (WorldContext = "WorldContextObject"))
    static void RefreshAllFromModConfig(const UObject* WorldContextObject, bool bForceRuntimeApply = true);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Runtime", meta = (WorldContext = "WorldContextObject"))
    static void ApplyManagedPPVFromModConfig(const UObject* WorldContextObject);
};
