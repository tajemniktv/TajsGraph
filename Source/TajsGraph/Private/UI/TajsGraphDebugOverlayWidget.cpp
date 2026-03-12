#include "UI/TajsGraphDebugOverlayWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Debug/TajsGraphDebugSubsystem.h"

void UTajsGraphDebugOverlayWidget::NativeConstruct() {
    Super::NativeConstruct();

    if (!WidgetTree) {
        return;
    }

    if (!RootCanvas) {
        RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("TajsGraphDebugOverlayRootCanvas"));
        WidgetTree->RootWidget = RootCanvas;
    }

    if (!RootPanel) {
        RootPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("TajsGraphDebugOverlayPanel"));
        RootPanel->SetPadding(FMargin(12.0f, 9.0f, 12.0f, 9.0f));
        RootPanel->SetBrushColor(FLinearColor(0.02f, 0.03f, 0.05f, 0.86f));
        RootPanel->SetDesiredSizeScale(FVector2D(1.0f, 1.0f));

        if (UCanvasPanelSlot* PanelSlot = RootCanvas ? Cast<UCanvasPanelSlot>(RootCanvas->AddChild(RootPanel)) : nullptr) {
            PanelSlot->SetAutoSize(true);
            PanelSlot->SetPosition(FVector2D(16.0f, 16.0f));
        }
    }

    if (!RootTextBlock) {
        RootTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TajsGraphDebugOverlayText"));
    }

    if (RootPanel && RootTextBlock) {
        RootTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0.85f, 0.96f, 1.0f, 1.0f)));
        RootTextBlock->SetShadowColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.75f));
        RootTextBlock->SetShadowOffset(FVector2D(1.0f, 1.0f));
        RootTextBlock->SetAutoWrapText(false);

        FSlateFontInfo Font = RootTextBlock->GetFont();
        Font.Size = 11;
        RootTextBlock->SetFont(Font);
        RootTextBlock->SetText(FText::FromString(TEXT("TajsGraph DebugHub\nWaiting for data...")));
        RootPanel->SetContent(RootTextBlock);
    }
}

void UTajsGraphDebugOverlayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
    Super::NativeTick(MyGeometry, InDeltaTime);

    RefreshAccumulatorSeconds += FMath::Max(0.0f, InDeltaTime);
    if (RefreshAccumulatorSeconds < RefreshIntervalSeconds) {
        return;
    }
    RefreshAccumulatorSeconds = 0.0f;

    UWorld* World = GetWorld();
    if (!World || !RootTextBlock) {
        return;
    }

    if (UGameInstance* GameInstance = World->GetGameInstance()) {
        if (UTajsGraphDebugSubsystem* DebugSubsystem = GameInstance->GetSubsystem<UTajsGraphDebugSubsystem>()) {
            PendingLines = DebugSubsystem->GetOverlayLines();
        }
    }

    FString DisplayText = TEXT("TajsGraph DebugHub");
    DisplayText += TEXT("\n-------------------");
    if (PendingLines.Num() <= 0) {
        DisplayText += TEXT("\nNo data.");
    } else {
        for (int32 Index = 0; Index < PendingLines.Num(); ++Index) {
            DisplayText += TEXT("\n");
            DisplayText += PendingLines[Index];
        }
    }

    if (DisplayText != CachedDisplayText) {
        CachedDisplayText = DisplayText;
        RootTextBlock->SetText(FText::FromString(CachedDisplayText));
    }
}

void UTajsGraphDebugOverlayWidget::SetLines(const TArray<FString>& InLines) {
    PendingLines = InLines;
    RefreshAccumulatorSeconds = RefreshIntervalSeconds;
}
