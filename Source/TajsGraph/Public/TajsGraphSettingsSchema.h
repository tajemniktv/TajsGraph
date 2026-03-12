#pragma once

#include "CoreMinimal.h"
#include "TajsGraphSettingsSchema.generated.h"

UENUM(BlueprintType)
enum class ETajsGraphSettingType : uint8 {
    Bool UMETA(DisplayName = "Bool"),
    Int UMETA(DisplayName = "Int"),
    Float UMETA(DisplayName = "Float"),
    String UMETA(DisplayName = "String"),
    Color UMETA(DisplayName = "Color")
};

UENUM(BlueprintType)
enum class ETajsGraphApplyPolicy : uint8 {
    Live UMETA(DisplayName = "Live"),
    ApplyButton UMETA(DisplayName = "Apply Button"),
    RequiresReload UMETA(DisplayName = "Requires Reload")
};

USTRUCT(BlueprintType)
struct TAJSGRAPH_API FTajsGraphSettingDescriptor {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    FString Section;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    FString Key;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    FString Category;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    FString Subcategory;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    ETajsGraphSettingType Type = ETajsGraphSettingType::String;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    FString DefaultValue;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    float MinValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    float MaxValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    float Step = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    ETajsGraphApplyPolicy ApplyPolicy = ETajsGraphApplyPolicy::Live;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    bool bDangerous = false;

    UPROPERTY(BlueprintReadOnly, Category = "TajsGraph|Settings")
    FString Tooltip;
};
