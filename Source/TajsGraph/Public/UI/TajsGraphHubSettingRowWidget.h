#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TajsGraphSettingsSchema.h"
#include "TajsGraphHubSettingRowWidget.generated.h"

class UCheckBox;
class UEditableTextBox;
class UHorizontalBox;
class USlider;
class UTextBlock;
class UBorder;
class UTajsGraphDebugMenuWidget;

UCLASS()
class TAJSGRAPH_API UTajsGraphHubSettingRowWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void InitializeRow(UTajsGraphDebugMenuWidget* InHub, const FTajsGraphSettingDescriptor& InDescriptor, const FString& InitialDraftValue);
    FString GetSettingId() const { return Descriptor.Id; }
    FString GetSection() const { return Descriptor.Section; }
    FString GetDraftValue() const { return DraftValue; }
    void RefreshFromDraft(const FString& InDraftValue);

private:
    UFUNCTION()
    void HandleBoolChanged(bool bIsChecked);

    UFUNCTION()
    void HandleTextCommitted(const FText& NewText, ETextCommit::Type CommitType);

    UFUNCTION()
    void HandleSliderChanged(float InValue);

    UFUNCTION()
    void HandleSliderCaptureEnded();

    UFUNCTION()
    void HandleColorRCommitted(const FText& NewText, ETextCommit::Type CommitType);

    UFUNCTION()
    void HandleColorGCommitted(const FText& NewText, ETextCommit::Type CommitType);

    UFUNCTION()
    void HandleColorBCommitted(const FText& NewText, ETextCommit::Type CommitType);

    UFUNCTION()
    void HandleColorACommitted(const FText& NewText, ETextCommit::Type CommitType);

    void NotifyDraftChanged();
    bool ParseBool(const FString& Value) const;
    void SyncControlsFromDraft(bool bUpdateTextBox, bool bUpdateSlider, bool bUpdateCheckBox);
    void SyncColorControlsFromDraft();
    bool ParseColorFromDraft(FLinearColor& OutColor) const;
    void ApplyColorComponent(float R, float G, float B, float A);
    float ParseSingleColorField(const FText& Text, float Fallback, float Min, float Max) const;

private:
    UPROPERTY(Transient)
    UHorizontalBox* RootRow = nullptr;

    UPROPERTY(Transient)
    UTextBlock* LabelText = nullptr;

    UPROPERTY(Transient)
    UTextBlock* WarningText = nullptr;

    UPROPERTY(Transient)
    UCheckBox* BoolCheckBox = nullptr;

    UPROPERTY(Transient)
    USlider* NumericSlider = nullptr;

    UPROPERTY(Transient)
    UEditableTextBox* ValueTextBox = nullptr;

    UPROPERTY(Transient)
    UHorizontalBox* ColorBox = nullptr;

    UPROPERTY(Transient)
    UEditableTextBox* ColorRTextBox = nullptr;

    UPROPERTY(Transient)
    UEditableTextBox* ColorGTextBox = nullptr;

    UPROPERTY(Transient)
    UEditableTextBox* ColorBTextBox = nullptr;

    UPROPERTY(Transient)
    UEditableTextBox* ColorATextBox = nullptr;

    UPROPERTY(Transient)
    UBorder* ColorPreviewBorder = nullptr;

    UPROPERTY()
    UTajsGraphDebugMenuWidget* HubOwner = nullptr;

    FTajsGraphSettingDescriptor Descriptor;
    FString DraftValue;
    bool bInternalUpdate = false;
};
