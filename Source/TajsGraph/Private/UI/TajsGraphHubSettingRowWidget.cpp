#include "UI/TajsGraphHubSettingRowWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CheckBox.h"
#include "Components/Border.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "UI/TajsGraphDebugMenuWidget.h"

void UTajsGraphHubSettingRowWidget::InitializeRow(UTajsGraphDebugMenuWidget* InHub, const FTajsGraphSettingDescriptor& InDescriptor, const FString& InitialDraftValue)
{
    HubOwner = InHub;
    Descriptor = InDescriptor;
    DraftValue = InitialDraftValue;

    if (!WidgetTree)
    {
        return;
    }

    RootRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("Row"));
    WidgetTree->RootWidget = RootRow;

    LabelText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Label"));
    if (LabelText)
    {
        LabelText->SetText(FText::FromString(FString::Printf(TEXT("%s.%s"), *Descriptor.Section, *Descriptor.Key)));
        LabelText->SetToolTipText(FText::FromString(Descriptor.Tooltip));
        if (UHorizontalBoxSlot* RowSlot = RootRow->AddChildToHorizontalBox(LabelText))
        {
            RowSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
            RowSlot->SetPadding(FMargin(2.0f));
        }
    }

    if (Descriptor.bDangerous)
    {
        WarningText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Danger"));
        if (WarningText)
        {
            WarningText->SetText(FText::FromString(TEXT("ADV")));
            WarningText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.76f, 0.2f, 1.0f)));
            if (UHorizontalBoxSlot* RowSlot = RootRow->AddChildToHorizontalBox(WarningText))
            {
                RowSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
                RowSlot->SetPadding(FMargin(4.0f, 2.0f));
            }
        }
    }

    if (Descriptor.Type == ETajsGraphSettingType::Bool)
    {
        BoolCheckBox = WidgetTree->ConstructWidget<UCheckBox>(UCheckBox::StaticClass(), TEXT("BoolCheck"));
        if (BoolCheckBox)
        {
            BoolCheckBox->OnCheckStateChanged.AddDynamic(this, &UTajsGraphHubSettingRowWidget::HandleBoolChanged);
            if (UHorizontalBoxSlot* RowSlot = RootRow->AddChildToHorizontalBox(BoolCheckBox))
            {
                RowSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
                RowSlot->SetPadding(FMargin(8.0f, 2.0f));
            }
        }
    }
    else if (Descriptor.Type == ETajsGraphSettingType::Color)
    {
        ColorBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ColorBox"));
        if (ColorBox)
        {
            auto AddFieldToColorBox = [this](UEditableTextBox* TextBox)
            {
                if (!TextBox || !ColorBox)
                {
                    return;
                }

                TextBox->SetMinDesiredWidth(52.0f);
                if (UHorizontalBoxSlot* ColorSlot = ColorBox->AddChildToHorizontalBox(TextBox))
                {
                    ColorSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
                    ColorSlot->SetPadding(FMargin(2.0f));
                }
            };

            ColorRTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("ColorR"));
            if (ColorRTextBox)
            {
                ColorRTextBox->OnTextCommitted.AddDynamic(this, &UTajsGraphHubSettingRowWidget::HandleColorRCommitted);
                AddFieldToColorBox(ColorRTextBox);
            }

            ColorGTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("ColorG"));
            if (ColorGTextBox)
            {
                ColorGTextBox->OnTextCommitted.AddDynamic(this, &UTajsGraphHubSettingRowWidget::HandleColorGCommitted);
                AddFieldToColorBox(ColorGTextBox);
            }

            ColorBTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("ColorB"));
            if (ColorBTextBox)
            {
                ColorBTextBox->OnTextCommitted.AddDynamic(this, &UTajsGraphHubSettingRowWidget::HandleColorBCommitted);
                AddFieldToColorBox(ColorBTextBox);
            }

            ColorATextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("ColorA"));
            if (ColorATextBox)
            {
                ColorATextBox->OnTextCommitted.AddDynamic(this, &UTajsGraphHubSettingRowWidget::HandleColorACommitted);
                AddFieldToColorBox(ColorATextBox);
            }

            ColorPreviewBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ColorPreview"));
            if (ColorPreviewBorder)
            {
                ColorPreviewBorder->SetPadding(FMargin(8.0f));
                if (UHorizontalBoxSlot* PreviewSlot = ColorBox->AddChildToHorizontalBox(ColorPreviewBorder))
                {
                    PreviewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
                    PreviewSlot->SetPadding(FMargin(4.0f, 2.0f));
                }
            }

            if (UHorizontalBoxSlot* RowSlot = RootRow->AddChildToHorizontalBox(ColorBox))
            {
                RowSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
                RowSlot->SetPadding(FMargin(8.0f, 2.0f));
            }
        }
    }
    else
    {
        if (Descriptor.MinValue < Descriptor.MaxValue && (Descriptor.Type == ETajsGraphSettingType::Float || Descriptor.Type == ETajsGraphSettingType::Int))
        {
            NumericSlider = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("NumericSlider"));
            if (NumericSlider)
            {
                NumericSlider->SetMinValue(Descriptor.MinValue);
                NumericSlider->SetMaxValue(Descriptor.MaxValue);
                NumericSlider->OnValueChanged.AddDynamic(this, &UTajsGraphHubSettingRowWidget::HandleSliderChanged);
                NumericSlider->OnMouseCaptureEnd.AddDynamic(this, &UTajsGraphHubSettingRowWidget::HandleSliderCaptureEnded);
                NumericSlider->OnControllerCaptureEnd.AddDynamic(this, &UTajsGraphHubSettingRowWidget::HandleSliderCaptureEnded);
                if (UHorizontalBoxSlot* RowSlot = RootRow->AddChildToHorizontalBox(NumericSlider))
                {
                    RowSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
                    RowSlot->SetPadding(FMargin(8.0f, 2.0f));
                }
            }
        }

        ValueTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("ValueText"));
        if (ValueTextBox)
        {
            ValueTextBox->OnTextCommitted.AddDynamic(this, &UTajsGraphHubSettingRowWidget::HandleTextCommitted);
            if (UHorizontalBoxSlot* RowSlot = RootRow->AddChildToHorizontalBox(ValueTextBox))
            {
                RowSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
                RowSlot->SetPadding(FMargin(8.0f, 2.0f));
            }
        }
    }

    SyncControlsFromDraft(true, true, true);
}

void UTajsGraphHubSettingRowWidget::RefreshFromDraft(const FString& InDraftValue)
{
    DraftValue = InDraftValue;
    SyncControlsFromDraft(true, true, true);
}

void UTajsGraphHubSettingRowWidget::HandleBoolChanged(const bool bIsChecked)
{
    if (bInternalUpdate)
    {
        return;
    }

    DraftValue = bIsChecked ? TEXT("true") : TEXT("false");
    NotifyDraftChanged();
}

void UTajsGraphHubSettingRowWidget::HandleTextCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
    if (bInternalUpdate)
    {
        return;
    }

    DraftValue = NewText.ToString().TrimStartAndEnd();
    SyncControlsFromDraft(false, true, false);
    NotifyDraftChanged();
}

void UTajsGraphHubSettingRowWidget::HandleSliderChanged(const float InValue)
{
    if (bInternalUpdate)
    {
        return;
    }

    const float Step = Descriptor.Step > 0.0f ? Descriptor.Step : 0.01f;
    const float Snapped = FMath::GridSnap(InValue, Step);
    if (Descriptor.Type == ETajsGraphSettingType::Int)
    {
        DraftValue = FString::FromInt(FMath::RoundToInt(Snapped));
    }
    else
    {
        DraftValue = FString::SanitizeFloat(Snapped);
    }

    SyncControlsFromDraft(true, false, false);
}

void UTajsGraphHubSettingRowWidget::HandleSliderCaptureEnded()
{
    if (!bInternalUpdate)
    {
        NotifyDraftChanged();
    }
}

void UTajsGraphHubSettingRowWidget::HandleColorRCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
    if (bInternalUpdate)
    {
        return;
    }

    FLinearColor Color;
    ParseColorFromDraft(Color);
    ApplyColorComponent(ParseSingleColorField(NewText, Color.R, 0.0f, 16.0f), Color.G, Color.B, Color.A);
    NotifyDraftChanged();
}

void UTajsGraphHubSettingRowWidget::HandleColorGCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
    if (bInternalUpdate)
    {
        return;
    }

    FLinearColor Color;
    ParseColorFromDraft(Color);
    ApplyColorComponent(Color.R, ParseSingleColorField(NewText, Color.G, 0.0f, 16.0f), Color.B, Color.A);
    NotifyDraftChanged();
}

void UTajsGraphHubSettingRowWidget::HandleColorBCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
    if (bInternalUpdate)
    {
        return;
    }

    FLinearColor Color;
    ParseColorFromDraft(Color);
    ApplyColorComponent(Color.R, Color.G, ParseSingleColorField(NewText, Color.B, 0.0f, 16.0f), Color.A);
    NotifyDraftChanged();
}

void UTajsGraphHubSettingRowWidget::HandleColorACommitted(const FText& NewText, ETextCommit::Type CommitType)
{
    if (bInternalUpdate)
    {
        return;
    }

    FLinearColor Color;
    ParseColorFromDraft(Color);
    ApplyColorComponent(Color.R, Color.G, Color.B, ParseSingleColorField(NewText, Color.A, 0.0f, 1.0f));
    NotifyDraftChanged();
}

void UTajsGraphHubSettingRowWidget::NotifyDraftChanged()
{
    if (HubOwner)
    {
        HubOwner->HandleSettingRowDraftChanged(Descriptor.Id, DraftValue);
    }
}

bool UTajsGraphHubSettingRowWidget::ParseBool(const FString& Value) const
{
    const FString Normalized = Value.TrimStartAndEnd().ToLower();
    return Normalized == TEXT("true") || Normalized == TEXT("1") || Normalized == TEXT("on") || Normalized == TEXT("yes");
}

void UTajsGraphHubSettingRowWidget::SyncControlsFromDraft(const bool bUpdateTextBox, const bool bUpdateSlider, const bool bUpdateCheckBox)
{
    TGuardValue<bool> Guard(bInternalUpdate, true);

    if (bUpdateCheckBox && BoolCheckBox)
    {
        BoolCheckBox->SetIsChecked(ParseBool(DraftValue));
    }

    if (bUpdateSlider && NumericSlider)
    {
        const float Numeric = FCString::Atof(*DraftValue);
        NumericSlider->SetValue(FMath::Clamp(Numeric, Descriptor.MinValue, Descriptor.MaxValue));
    }

    if (bUpdateTextBox && ValueTextBox)
    {
        ValueTextBox->SetText(FText::FromString(DraftValue));
    }

    if (Descriptor.Type == ETajsGraphSettingType::Color)
    {
        SyncColorControlsFromDraft();
    }
}

void UTajsGraphHubSettingRowWidget::SyncColorControlsFromDraft()
{
    FLinearColor Color;
    ParseColorFromDraft(Color);

    if (ColorRTextBox)
    {
        ColorRTextBox->SetText(FText::AsNumber(Color.R));
    }
    if (ColorGTextBox)
    {
        ColorGTextBox->SetText(FText::AsNumber(Color.G));
    }
    if (ColorBTextBox)
    {
        ColorBTextBox->SetText(FText::AsNumber(Color.B));
    }
    if (ColorATextBox)
    {
        ColorATextBox->SetText(FText::AsNumber(Color.A));
    }
    if (ColorPreviewBorder)
    {
        ColorPreviewBorder->SetBrushColor(FLinearColor(Color.R, Color.G, Color.B, 1.0f));
    }
}

bool UTajsGraphHubSettingRowWidget::ParseColorFromDraft(FLinearColor& OutColor) const
{
    FString Normalized = DraftValue.TrimStartAndEnd();
    if (Normalized.StartsWith(TEXT("(")) && Normalized.EndsWith(TEXT(")")) && Normalized.Len() >= 2)
    {
        Normalized = Normalized.Mid(1, Normalized.Len() - 2).TrimStartAndEnd();
    }

    if (!OutColor.InitFromString(Normalized))
    {
        OutColor = FLinearColor::White;
        return false;
    }

    OutColor.R = FMath::Clamp(OutColor.R, 0.0f, 16.0f);
    OutColor.G = FMath::Clamp(OutColor.G, 0.0f, 16.0f);
    OutColor.B = FMath::Clamp(OutColor.B, 0.0f, 16.0f);
    OutColor.A = FMath::Clamp(OutColor.A, 0.0f, 1.0f);
    return true;
}

void UTajsGraphHubSettingRowWidget::ApplyColorComponent(const float R, const float G, const float B, const float A)
{
    DraftValue = FString::Printf(TEXT("(R=%s,G=%s,B=%s,A=%s)"),
        *FString::SanitizeFloat(FMath::Clamp(R, 0.0f, 16.0f)),
        *FString::SanitizeFloat(FMath::Clamp(G, 0.0f, 16.0f)),
        *FString::SanitizeFloat(FMath::Clamp(B, 0.0f, 16.0f)),
        *FString::SanitizeFloat(FMath::Clamp(A, 0.0f, 1.0f)));

    TGuardValue<bool> Guard(bInternalUpdate, true);
    SyncColorControlsFromDraft();
}

float UTajsGraphHubSettingRowWidget::ParseSingleColorField(const FText& Text, const float Fallback, const float Min, const float Max) const
{
    const FString Raw = Text.ToString().TrimStartAndEnd();
    if (Raw.IsEmpty())
    {
        return Fallback;
    }
    return FMath::Clamp(FCString::Atof(*Raw), Min, Max);
}
