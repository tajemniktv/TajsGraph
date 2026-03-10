#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Debug/TajsGraphDebugTypes.h"
#include "TajsGraphDebugOverlayWidget.generated.h"

class UTextBlock;
class UBorder;
class UCanvasPanel;

UCLASS()
class TAJSGRAPH_API UTajsGraphDebugOverlayWidget : public UUserWidget {
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    void SetLines(const TArray<FString>& InLines);

private:
    UPROPERTY(Transient)
    UCanvasPanel* RootCanvas = nullptr;

    UPROPERTY(Transient)
    UBorder* RootPanel = nullptr;

    UPROPERTY(Transient)
    UTextBlock* RootTextBlock = nullptr;

    TArray<FString> PendingLines;
    FString CachedDisplayText;
    float RefreshAccumulatorSeconds = 0.0f;
    float RefreshIntervalSeconds = 0.2f;
};
