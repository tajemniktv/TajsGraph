
#include "UI/TajsGraphDebugMenuWidget.h"

#include "TajsGraphModule.h"
#include "TajsGraphSmlSettings.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Debug/TajsGraphDebugSubsystem.h"
#include "Dom/JsonObject.h"
#include "HAL/FileManager.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "UI/TajsGraphHubSettingRowWidget.h"

namespace
{
static FString NormalizeText(const FString& Value)
{
    FString Out = Value.TrimStartAndEnd();
    Out.ToLowerInline();
    return Out;
}

static bool ParseBoolText(const FString& Value)
{
    const FString Normalized = NormalizeText(Value);
    return Normalized == TEXT("true") || Normalized == TEXT("1") || Normalized == TEXT("yes") || Normalized == TEXT("on");
}

static FString MakeSectionKey(const FTajsGraphSettingDescriptor& Descriptor)
{
    return FString::Printf(TEXT("%s|%s"), *Descriptor.Category, *Descriptor.Subcategory);
}

static FString MakeSectionLabel(const FString& SectionKey)
{
    FString Category;
    FString Subcategory;
    if (SectionKey.Split(TEXT("|"), &Category, &Subcategory, ESearchCase::CaseSensitive))
    {
        return FString::Printf(TEXT("%s / %s"), *Category, *Subcategory);
    }
    return SectionKey;
}

static void BroadcastSettingChange(const FString& Section, const FString& Key, const FString& Value)
{
    FTajsGraphModule::OnSettingChanged().Broadcast(Section, Key, Value);
}

static bool PersistBoolSetting(const FString& Section, const FString& Key, const bool bValue)
{
    if (!TajsGraphSmlSettings::SetBool(Section, Key, bValue))
    {
        return false;
    }

    BroadcastSettingChange(Section, Key, bValue ? TEXT("true") : TEXT("false"));
    return true;
}

static bool PersistIntSetting(const FString& Section, const FString& Key, const int32 Value)
{
    if (!TajsGraphSmlSettings::SetInt(Section, Key, Value))
    {
        return false;
    }

    BroadcastSettingChange(Section, Key, FString::FromInt(Value));
    return true;
}

static bool PersistFloatSetting(const FString& Section, const FString& Key, const float Value)
{
    if (!TajsGraphSmlSettings::SetFloat(Section, Key, Value))
    {
        return false;
    }

    BroadcastSettingChange(Section, Key, FString::SanitizeFloat(Value));
    return true;
}

static bool PersistStringSetting(const FString& Section, const FString& Key, const FString& Value)
{
    if (!TajsGraphSmlSettings::SetString(Section, Key, Value))
    {
        return false;
    }

    BroadcastSettingChange(Section, Key, Value);
    return true;
}

static UTextBlock* AddTextLine(UWidgetTree* WidgetTree, UPanelWidget* Parent, const FString& Text, const FLinearColor& Color = FLinearColor(0.90f, 0.95f, 1.0f, 1.0f), int32 FontSize = 10)
{
    if (!WidgetTree || !Parent)
    {
        return nullptr;
    }

    UTextBlock* Label = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    if (!Label)
    {
        return nullptr;
    }

    Label->SetText(FText::FromString(Text));
    Label->SetColorAndOpacity(FSlateColor(Color));
    FSlateFontInfo Font = Label->GetFont();
    Font.Size = FontSize;
    Label->SetFont(Font);

    if (UVerticalBox* VBox = Cast<UVerticalBox>(Parent))
    {
        if (UVerticalBoxSlot* Slot = VBox->AddChildToVerticalBox(Label))
        {
            Slot->SetPadding(FMargin(2.0f));
        }
    }
    else if (UHorizontalBox* HBox = Cast<UHorizontalBox>(Parent))
    {
        if (UHorizontalBoxSlot* Slot = HBox->AddChildToHorizontalBox(Label))
        {
            Slot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            Slot->SetPadding(FMargin(2.0f));
        }
    }

    return Label;
}

static UButton* AddButton(UWidgetTree* WidgetTree, UPanelWidget* Parent, const TCHAR* Name, const FString& Label)
{
    if (!WidgetTree || !Parent)
    {
        return nullptr;
    }

    UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
    UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    if (Text)
    {
        Text->SetText(FText::FromString(Label));
        Text->SetColorAndOpacity(FSlateColor(FLinearColor(0.90f, 0.97f, 1.0f, 1.0f)));
        FSlateFontInfo Font = Text->GetFont();
        Font.Size = 9;
        Text->SetFont(Font);
        Button->SetContent(Text);
    }

    if (UHorizontalBox* HBox = Cast<UHorizontalBox>(Parent))
    {
        if (UHorizontalBoxSlot* Slot = HBox->AddChildToHorizontalBox(Button))
        {
            Slot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            Slot->SetPadding(FMargin(2.0f));
        }
    }
    else if (UVerticalBox* VBox = Cast<UVerticalBox>(Parent))
    {
        if (UVerticalBoxSlot* Slot = VBox->AddChildToVerticalBox(Button))
        {
            Slot->SetPadding(FMargin(2.0f));
        }
    }

    return Button;
}

static FString MakeSafeProfileName(FString Name)
{
    Name = Name.TrimStartAndEnd();
    static const TCHAR* InvalidChars = TEXT("\\/:*?\"<>|");
    for (const TCHAR* Ptr = InvalidChars; *Ptr != 0; ++Ptr)
    {
        Name.ReplaceCharInline(*Ptr, TEXT('_'));
    }
    return Name;
}
}

void UTajsGraphDebugMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    LoadDescriptors();
    RefreshHubData(true);
    SetActiveTab(ETajsGraphHubTab::Overview);
}


void UTajsGraphDebugMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    RefreshAccumulatorSeconds += FMath::Max(0.0f, InDeltaTime);
    if (RefreshAccumulatorSeconds < RefreshIntervalSeconds)
    {
        return;
    }

    RefreshAccumulatorSeconds = 0.0f;
    // Avoid rebuilding widget trees during Slate prepass/tick.
    // Full tab rebuilds happen on explicit user actions only.
}
void UTajsGraphDebugMenuWidget::RefreshHubData(const bool bResetBaseline)
{
    LoadDescriptors();
    InitializeDraftFromLive(bResetBaseline);
    if (ActiveTab == ETajsGraphHubTab::Settings && ContentScrollBox)
    {
        RebuildSettingsRows();
    }
    BP_OnHubDataRefreshed();
}

void UTajsGraphDebugMenuWidget::HandleSettingRowDraftChanged(const FString& SettingId, const FString& NewValue)
{
    DraftValues.Add(SettingId, NewValue);
    const FString* Baseline = BaselineValues.Find(SettingId);
    if (!Baseline || *Baseline != NewValue)
    {
        ChangedKeys.Add(SettingId);
    }
    else
    {
        ChangedKeys.Remove(SettingId);
    }
    SetStatusMessage(FString::Printf(TEXT("Draft changed: %d key(s)."), ChangedKeys.Num()));
}

void UTajsGraphDebugMenuWidget::HandleTabOverview() { SetActiveTab(ETajsGraphHubTab::Overview); }
void UTajsGraphDebugMenuWidget::HandleTabVisualization() { SetActiveTab(ETajsGraphHubTab::Visualization); }
void UTajsGraphDebugMenuWidget::HandleTabSettings() { SetActiveTab(ETajsGraphHubTab::Settings); }
void UTajsGraphDebugMenuWidget::HandleTabDebugReports() { SetActiveTab(ETajsGraphHubTab::DebugReports); }
void UTajsGraphDebugMenuWidget::HandleTabProfiles() { SetActiveTab(ETajsGraphHubTab::Profiles); }

void UTajsGraphDebugMenuWidget::HandleCloseClicked()
{
    if (UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem())
    {
        DebugSubsystem->CloseHub();
    }
}

void UTajsGraphDebugMenuWidget::HandleApplyDraftClicked()
{
    if (ChangedKeys.Num() <= 0)
    {
        SetStatusMessage(TEXT("No draft changes to apply."));
        return;
    }

    int32 SuccessCount = 0;
    int32 FailureCount = 0;
    FString FirstFailure;

    TArray<FString> Keys;
    Keys.Reserve(ChangedKeys.Num());
    for (const FString& Key : ChangedKeys)
    {
        Keys.Add(Key);
    }
    Keys.Sort();

    for (const FString& Key : Keys)
    {
        const FTajsGraphSettingDescriptor* Descriptor = DescriptorById.Find(Key);
        const FString* DraftValue = DraftValues.Find(Key);
        if (!Descriptor || !DraftValue)
        {
            ++FailureCount;
            if (FirstFailure.IsEmpty())
            {
                FirstFailure = FString::Printf(TEXT("%s: missing descriptor/value"), *Key);
            }
            continue;
        }

        FString Error;
        if (ApplySettingValue(*Descriptor, *DraftValue, Error))
        {
            ++SuccessCount;
        }
        else
        {
            ++FailureCount;
            if (FirstFailure.IsEmpty())
            {
                FirstFailure = FString::Printf(TEXT("%s: %s"), *Key, *Error);
            }
        }
    }

    if (SuccessCount > 0)
    {
        FTajsGraphModule::ForceApplySettings();
    }

    RefreshHubData(true);
    SetStatusMessage(FailureCount > 0
        ? FString::Printf(TEXT("Apply done. Success=%d Failure=%d FirstError=%s"), SuccessCount, FailureCount, *FirstFailure)
        : FString::Printf(TEXT("Apply done. Success=%d"), SuccessCount));
}

void UTajsGraphDebugMenuWidget::HandleRevertAllClicked()
{
    DraftValues = BaselineValues;
    ChangedKeys.Reset();
    for (UTajsGraphHubSettingRowWidget* Row : SettingRows)
    {
        if (Row)
        {
            const FString* Value = DraftValues.Find(Row->GetSettingId());
            Row->RefreshFromDraft(Value ? *Value : FString());
        }
    }
    SetStatusMessage(TEXT("Draft reverted to baseline."));
}

void UTajsGraphDebugMenuWidget::HandleResetSectionClicked()
{
    int32 ResetCount = 0;
    for (const FTajsGraphSettingDescriptor& Descriptor : Descriptors)
    {
        const FString SectionKey = MakeSectionKey(Descriptor);
        if (!CurrentSettingsSection.IsEmpty() && CurrentSettingsSection != TEXT("All") && CurrentSettingsSection != SectionKey)
        {
            continue;
        }

        FString LiveValue;
        if (!TryReadLiveValue(Descriptor, LiveValue))
        {
            continue;
        }

        DraftValues.Add(Descriptor.Id, LiveValue);
        const FString* Baseline = BaselineValues.Find(Descriptor.Id);
        if (!Baseline || *Baseline != LiveValue)
        {
            ChangedKeys.Add(Descriptor.Id);
        }
        else
        {
            ChangedKeys.Remove(Descriptor.Id);
        }
        ++ResetCount;
    }

    for (UTajsGraphHubSettingRowWidget* Row : SettingRows)
    {
        if (Row)
        {
            const FString* Value = DraftValues.Find(Row->GetSettingId());
            Row->RefreshFromDraft(Value ? *Value : FString());
        }
    }

    SetStatusMessage(FString::Printf(TEXT("Section reset (%d key(s))."), ResetCount));
}

void UTajsGraphDebugMenuWidget::HandleAdvancedFilterChanged(const bool bChecked)
{
    bShowAdvanced = bChecked;
    RebuildSettingsTab();
}

void UTajsGraphDebugMenuWidget::HandleSectionSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    CurrentSettingsSection = SelectedItem;
    RebuildSettingsRows();
}

void UTajsGraphDebugMenuWidget::HandleVisToggleClicked()
{
    if (UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem())
    {
        const bool bEnabled = DebugSubsystem->ToggleVisualization();
        SetStatusMessage(FString::Printf(TEXT("Visualization %s."), bEnabled ? TEXT("enabled") : TEXT("disabled")));
    }
}

void UTajsGraphDebugMenuWidget::HandleVisPrevClicked()
{
    if (UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem())
    {
        const ETajsGraphVisMode Mode = DebugSubsystem->PreviousVisualizationMode();
        SetStatusMessage(FString::Printf(TEXT("Visualization mode: %s"), *GetVisModeDisplayName(Mode)));
    }
}

void UTajsGraphDebugMenuWidget::HandleVisNextClicked()
{
    if (UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem())
    {
        const ETajsGraphVisMode Mode = DebugSubsystem->NextVisualizationMode();
        SetStatusMessage(FString::Printf(TEXT("Visualization mode: %s"), *GetVisModeDisplayName(Mode)));
    }
}

void UTajsGraphDebugMenuWidget::HandleVisModeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem())
    {
        const ETajsGraphVisMode Mode = ParseVisModeFromName(SelectedItem);
        DebugSubsystem->SetVisualizationMode(Mode);
    }
}

void UTajsGraphDebugMenuWidget::HandleDebugToggleClicked()
{
    if (UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem())
    {
        const bool bEnable = !DebugSubsystem->IsDebugEnabled();
        DebugSubsystem->SetDebugEnabled(bEnable);
        SetStatusMessage(FString::Printf(TEXT("DebugHub %s."), bEnable ? TEXT("enabled") : TEXT("disabled")));
    }
}

void UTajsGraphDebugMenuWidget::HandleOverlayToggleClicked()
{
    if (UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem())
    {
        DebugSubsystem->ToggleOverlay();
        SetStatusMessage(FString::Printf(TEXT("Overlay %s."), DebugSubsystem->IsOverlayVisible() ? TEXT("enabled") : TEXT("disabled")));
    }
}

void UTajsGraphDebugMenuWidget::HandleOverlayPageSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem())
    {
        const FString Normalized = NormalizeText(SelectedItem);
        ETajsGraphOverlayPage Page = ETajsGraphOverlayPage::Perf;
        if (Normalized == TEXT("renderer")) { Page = ETajsGraphOverlayPage::Renderer; }
        else if (Normalized == TEXT("postprocess")) { Page = ETajsGraphOverlayPage::PostProcess; }
        else if (Normalized == TEXT("lights")) { Page = ETajsGraphOverlayPage::Lights; }
        else if (Normalized == TEXT("warnings")) { Page = ETajsGraphOverlayPage::Warnings; }
        DebugSubsystem->SetOverlayPage(Page);
    }
}

void UTajsGraphDebugMenuWidget::HandleGenerateReportClicked()
{
    if (UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem())
    {
        const FString Path = DebugSubsystem->GenerateReportBundle();
        SetStatusMessage(Path.IsEmpty() ? TEXT("Report generation failed.") : FString::Printf(TEXT("Report written to: %s"), *Path));
    }
}
void UTajsGraphDebugMenuWidget::HandleExpandedArtifactsChanged(const bool bChecked)
{
    if (!PersistBoolSetting(TEXT("DebugHub"), TEXT("WriteExpandedReportArtifacts"), bChecked))
    {
        SetStatusMessage(TEXT("Cannot update WriteExpandedReportArtifacts."));
        return;
    }

    SetStatusMessage(FString::Printf(TEXT("DebugHub.WriteExpandedReportArtifacts=%s"), bChecked ? TEXT("true") : TEXT("false")));
}

void UTajsGraphDebugMenuWidget::HandleProfileCreateClicked()
{
    FString ProfileName = ProfileNameTextBox ? MakeSafeProfileName(ProfileNameTextBox->GetText().ToString()) : FString();
    if (ProfileName.IsEmpty())
    {
        ProfileName = FString::Printf(TEXT("Profile_%s"), *FDateTime::UtcNow().ToString(TEXT("%Y%m%d_%H%M%S")));
    }

    FString Error;
    if (SaveProfileToPath(ProfileName, GetProfilePath(ProfileName), false, Error))
    {
        RefreshProfileList(ProfileName);
        SetStatusMessage(FString::Printf(TEXT("Profile created: %s"), *ProfileName));
    }
    else
    {
        SetStatusMessage(FString::Printf(TEXT("Create profile failed: %s"), *Error));
    }
}

void UTajsGraphDebugMenuWidget::HandleProfileSaveClicked()
{
    FString ProfileName = ProfileNameTextBox ? MakeSafeProfileName(ProfileNameTextBox->GetText().ToString()) : FString();
    if (ProfileName.IsEmpty() && ProfilesCombo)
    {
        ProfileName = MakeSafeProfileName(ProfilesCombo->GetSelectedOption());
    }

    if (ProfileName.IsEmpty())
    {
        SetStatusMessage(TEXT("Enter/select profile name before save."));
        return;
    }

    FString Error;
    if (SaveProfileToPath(ProfileName, GetProfilePath(ProfileName), true, Error))
    {
        RefreshProfileList(ProfileName);
        SetStatusMessage(FString::Printf(TEXT("Profile saved: %s"), *ProfileName));
    }
    else
    {
        SetStatusMessage(FString::Printf(TEXT("Save failed: %s"), *Error));
    }
}

void UTajsGraphDebugMenuWidget::HandleProfileLoadClicked()
{
    if (!ProfilesCombo)
    {
        return;
    }

    const FString ProfileName = MakeSafeProfileName(ProfilesCombo->GetSelectedOption());
    if (ProfileName.IsEmpty())
    {
        SetStatusMessage(TEXT("Select a profile to load."));
        return;
    }

    FString OutName;
    TMap<FString, FString> OutSettings;
    TArray<FString> UnknownKeys;
    FString Error;
    if (!LoadProfileFromPath(GetProfilePath(ProfileName), OutName, OutSettings, UnknownKeys, Error))
    {
        SetStatusMessage(FString::Printf(TEXT("Load failed: %s"), *Error));
        return;
    }

    for (const TPair<FString, FString>& Pair : OutSettings)
    {
        DraftValues.Add(Pair.Key, Pair.Value);
        const FString* Baseline = BaselineValues.Find(Pair.Key);
        if (!Baseline || *Baseline != Pair.Value)
        {
            ChangedKeys.Add(Pair.Key);
        }
        else
        {
            ChangedKeys.Remove(Pair.Key);
        }
    }

    for (UTajsGraphHubSettingRowWidget* Row : SettingRows)
    {
        if (Row)
        {
            const FString* NewValue = DraftValues.Find(Row->GetSettingId());
            if (NewValue)
            {
                Row->RefreshFromDraft(*NewValue);
            }
        }
    }

    FFileHelper::SaveStringToFile(ProfileName, *FPaths::Combine(GetProfilesRootDir(), TEXT("active_profile.txt")));
    SetStatusMessage(UnknownKeys.Num() > 0
        ? FString::Printf(TEXT("Profile loaded with %d unknown key(s) skipped."), UnknownKeys.Num())
        : FString::Printf(TEXT("Profile loaded to draft: %s"), *ProfileName));
}

void UTajsGraphDebugMenuWidget::HandleProfileRenameClicked()
{
    if (!ProfilesCombo || !ProfileNameTextBox)
    {
        return;
    }

    const FString OldName = MakeSafeProfileName(ProfilesCombo->GetSelectedOption());
    const FString NewName = MakeSafeProfileName(ProfileNameTextBox->GetText().ToString());
    if (OldName.IsEmpty() || NewName.IsEmpty())
    {
        SetStatusMessage(TEXT("Select profile and enter new name."));
        return;
    }

    if (IFileManager::Get().Move(*GetProfilePath(NewName), *GetProfilePath(OldName), true, true, false, false))
    {
        RefreshProfileList(NewName);
        SetStatusMessage(FString::Printf(TEXT("Profile renamed: %s -> %s"), *OldName, *NewName));
    }
    else
    {
        SetStatusMessage(TEXT("Rename failed."));
    }
}

void UTajsGraphDebugMenuWidget::HandleProfileDuplicateClicked()
{
    if (!ProfilesCombo || !ProfileNameTextBox)
    {
        return;
    }

    const FString SourceName = MakeSafeProfileName(ProfilesCombo->GetSelectedOption());
    const FString TargetName = MakeSafeProfileName(ProfileNameTextBox->GetText().ToString());
    if (SourceName.IsEmpty() || TargetName.IsEmpty())
    {
        SetStatusMessage(TEXT("Select source profile and enter target name."));
        return;
    }

    if (IFileManager::Get().Copy(*GetProfilePath(TargetName), *GetProfilePath(SourceName), true, true) == COPY_OK)
    {
        RefreshProfileList(TargetName);
        SetStatusMessage(FString::Printf(TEXT("Profile duplicated: %s -> %s"), *SourceName, *TargetName));
    }
    else
    {
        SetStatusMessage(TEXT("Duplicate failed."));
    }
}

void UTajsGraphDebugMenuWidget::HandleProfileDeleteClicked()
{
    if (!ProfilesCombo)
    {
        return;
    }

    const FString ProfileName = MakeSafeProfileName(ProfilesCombo->GetSelectedOption());
    if (ProfileName.IsEmpty())
    {
        SetStatusMessage(TEXT("Select profile to delete."));
        return;
    }

    if (IFileManager::Get().Delete(*GetProfilePath(ProfileName)))
    {
        RefreshProfileList();
        SetStatusMessage(FString::Printf(TEXT("Profile deleted: %s"), *ProfileName));
    }
    else
    {
        SetStatusMessage(TEXT("Delete failed."));
    }
}

void UTajsGraphDebugMenuWidget::HandleProfileImportClicked()
{
    if (!ImportPathTextBox)
    {
        return;
    }

    const FString ImportPath = ImportPathTextBox->GetText().ToString().TrimStartAndEnd();
    if (ImportPath.IsEmpty())
    {
        SetStatusMessage(TEXT("Enter import file path first."));
        return;
    }

    FString OutName;
    TMap<FString, FString> OutSettings;
    TArray<FString> UnknownKeys;
    FString Error;
    if (!LoadProfileFromPath(ImportPath, OutName, OutSettings, UnknownKeys, Error))
    {
        SetStatusMessage(FString::Printf(TEXT("Import failed: %s"), *Error));
        return;
    }

    FString ProfileName = OutName;
    if (ProfileName.IsEmpty())
    {
        ProfileName = ProfileNameTextBox ? MakeSafeProfileName(ProfileNameTextBox->GetText().ToString()) : FString();
    }
    if (ProfileName.IsEmpty())
    {
        ProfileName = FString::Printf(TEXT("Imported_%s"), *FDateTime::UtcNow().ToString(TEXT("%Y%m%d_%H%M%S")));
    }

    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *ImportPath) || !FFileHelper::SaveStringToFile(JsonString, *GetProfilePath(ProfileName)))
    {
        SetStatusMessage(TEXT("Import failed: cannot copy file."));
        return;
    }

    RefreshProfileList(ProfileName);
    SetStatusMessage(FString::Printf(TEXT("Profile imported: %s"), *ProfileName));
}

void UTajsGraphDebugMenuWidget::HandleProfileExportClicked()
{
    if (!ProfilesCombo || !ExportPathTextBox)
    {
        return;
    }

    const FString ProfileName = MakeSafeProfileName(ProfilesCombo->GetSelectedOption());
    const FString ExportPath = ExportPathTextBox->GetText().ToString().TrimStartAndEnd();
    if (ProfileName.IsEmpty() || ExportPath.IsEmpty())
    {
        SetStatusMessage(TEXT("Select profile and enter export path."));
        return;
    }

    FString Error;
    if (SaveProfileToPath(ProfileName, ExportPath, true, Error))
    {
        SetStatusMessage(FString::Printf(TEXT("Profile exported to %s"), *ExportPath));
    }
    else
    {
        SetStatusMessage(FString::Printf(TEXT("Export failed: %s"), *Error));
    }
}

void UTajsGraphDebugMenuWidget::HandleProfileSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    const FString ProfileName = MakeSafeProfileName(SelectedItem);
    if (ProfileName.IsEmpty())
    {
        return;
    }

    if (ProfileNameTextBox)
    {
        ProfileNameTextBox->SetText(FText::FromString(ProfileName));
    }

    FFileHelper::SaveStringToFile(ProfileName, *FPaths::Combine(GetProfilesRootDir(), TEXT("active_profile.txt")));
}

UTajsGraphDebugSubsystem* UTajsGraphDebugMenuWidget::ResolveDebugSubsystem() const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    if (UGameInstance* GI = World->GetGameInstance())
    {
        return GI->GetSubsystem<UTajsGraphDebugSubsystem>();
    }

    return nullptr;
}
void UTajsGraphDebugMenuWidget::BuildBaseLayout()
{
    if (!WidgetTree)
    {
        return;
    }

    RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("HubRootCanvas"));
    WidgetTree->RootWidget = RootCanvas;

    RootPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("HubRootPanel"));
    RootPanel->SetPadding(FMargin(12.0f));
    RootPanel->SetBrushColor(FLinearColor(0.02f, 0.04f, 0.08f, 0.93f));

    if (UCanvasPanelSlot* PanelSlot = RootCanvas ? Cast<UCanvasPanelSlot>(RootCanvas->AddChild(RootPanel)) : nullptr)
    {
        PanelSlot->SetAutoSize(false);
        PanelSlot->SetPosition(FVector2D(24.0f, 24.0f));
        PanelSlot->SetSize(FVector2D(980.0f, 680.0f));
    }

    RootBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("HubRootVBox"));
    RootPanel->SetContent(RootBox);

    UHorizontalBox* HeaderRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HeaderRow"));
    RootBox->AddChildToVerticalBox(HeaderRow);

    HeaderText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("HeaderText"));
    HeaderText->SetText(FText::FromString(TEXT("TajsGraph Hub v1")));
    HeaderText->SetColorAndOpacity(FSlateColor(FLinearColor(0.97f, 0.99f, 1.0f, 1.0f)));
    {
        FSlateFontInfo Font = HeaderText->GetFont();
        Font.Size = 14;
        HeaderText->SetFont(Font);
    }
    if (UHorizontalBoxSlot* HeaderTextSlot = HeaderRow->AddChildToHorizontalBox(HeaderText))
    {
        HeaderTextSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
        HeaderTextSlot->SetPadding(FMargin(2.0f));
    }

    if (UButton* CloseButton = AddButton(WidgetTree, HeaderRow, TEXT("CloseHubBtn"), TEXT("Close (F7)")))
    {
        CloseButton->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleCloseClicked);
    }

    UHorizontalBox* TabRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("TabRow"));
    if (UVerticalBoxSlot* TabRowSlot = RootBox->AddChildToVerticalBox(TabRow))
    {
        TabRowSlot->SetPadding(FMargin(0.0f, 6.0f, 0.0f, 4.0f));
    }

    if (UButton* Btn = AddButton(WidgetTree, TabRow, TEXT("TabOverview"), TEXT("Overview"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleTabOverview); }
    if (UButton* Btn = AddButton(WidgetTree, TabRow, TEXT("TabVisualization"), TEXT("Visualization"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleTabVisualization); }
    if (UButton* Btn = AddButton(WidgetTree, TabRow, TEXT("TabSettings"), TEXT("Settings"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleTabSettings); }
    if (UButton* Btn = AddButton(WidgetTree, TabRow, TEXT("TabDebug"), TEXT("Debug/Reports"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleTabDebugReports); }
    if (UButton* Btn = AddButton(WidgetTree, TabRow, TEXT("TabProfiles"), TEXT("Profiles"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleTabProfiles); }

    UHorizontalBox* SettingsActionRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("SettingsActionRow"));
    if (UVerticalBoxSlot* SettingsActionRowSlot = RootBox->AddChildToVerticalBox(SettingsActionRow))
    {
        SettingsActionRowSlot->SetPadding(FMargin(0.0f, 2.0f, 0.0f, 4.0f));
    }

    AdvancedCheckBox = WidgetTree->ConstructWidget<UCheckBox>(UCheckBox::StaticClass(), TEXT("AdvancedToggle"));
    if (AdvancedCheckBox)
    {
        AdvancedCheckBox->OnCheckStateChanged.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleAdvancedFilterChanged);
        SettingsActionRow->AddChildToHorizontalBox(AdvancedCheckBox);
    }
    AddTextLine(WidgetTree, SettingsActionRow, TEXT("Show Advanced/Dangerous"), FLinearColor(1.0f, 0.80f, 0.35f, 1.0f), 9);

    SectionCombo = WidgetTree->ConstructWidget<UComboBoxString>(UComboBoxString::StaticClass(), TEXT("SectionCombo"));
    if (SectionCombo)
    {
        SectionCombo->OnSelectionChanged.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleSectionSelectionChanged);
        if (UHorizontalBoxSlot* SectionComboSlot = SettingsActionRow->AddChildToHorizontalBox(SectionCombo))
        {
            SectionComboSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            SectionComboSlot->SetPadding(FMargin(8.0f, 2.0f));
        }
    }

    if (UButton* Btn = AddButton(WidgetTree, SettingsActionRow, TEXT("ApplyDraftBtn"), TEXT("Apply"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleApplyDraftClicked); }
    if (UButton* Btn = AddButton(WidgetTree, SettingsActionRow, TEXT("ResetSectionBtn"), TEXT("Reset Section"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleResetSectionClicked); }
    if (UButton* Btn = AddButton(WidgetTree, SettingsActionRow, TEXT("RevertAllBtn"), TEXT("Revert All"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleRevertAllClicked); }

    ContentScrollBox = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("HubContentScroll"));
    if (UVerticalBoxSlot* ContentScrollSlot = RootBox->AddChildToVerticalBox(ContentScrollBox))
    {
        ContentScrollSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
        ContentScrollSlot->SetPadding(FMargin(0.0f, 2.0f));
    }

    StatusText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StatusText"));
    if (StatusText)
    {
        StatusText->SetColorAndOpacity(FSlateColor(FLinearColor(0.82f, 0.92f, 1.0f, 1.0f)));
        StatusText->SetText(FText::FromString(TEXT("Hub ready.")));
        if (UVerticalBoxSlot* StatusTextSlot = RootBox->AddChildToVerticalBox(StatusText))
        {
            StatusTextSlot->SetPadding(FMargin(0.0f, 6.0f, 0.0f, 0.0f));
        }
    }
}

void UTajsGraphDebugMenuWidget::SetActiveTab(const ETajsGraphHubTab NewTab)
{
    ActiveTab = NewTab;
    const bool bSettingsTab = ActiveTab == ETajsGraphHubTab::Settings;
    if (AdvancedCheckBox)
    {
        AdvancedCheckBox->SetVisibility(bSettingsTab ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
    if (SectionCombo)
    {
        SectionCombo->SetVisibility(bSettingsTab ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }

    if (ContentScrollBox)
    {
        RebuildActiveTabContent();
    }
    BP_OnActiveTabChanged(NewTab);
}

void UTajsGraphDebugMenuWidget::RebuildActiveTabContent()
{
    if (!ContentScrollBox)
    {
        return;
    }

    ContentScrollBox->ClearChildren();
    SettingRows.Reset();

    switch (ActiveTab)
    {
    case ETajsGraphHubTab::Overview: RebuildOverviewTab(); break;
    case ETajsGraphHubTab::Visualization: RebuildVisualizationTab(); break;
    case ETajsGraphHubTab::Settings: RebuildSettingsTab(); break;
    case ETajsGraphHubTab::DebugReports: RebuildDebugReportsTab(); break;
    case ETajsGraphHubTab::Profiles: RebuildProfilesTab(); break;
    default: break;
    }
}

void UTajsGraphDebugMenuWidget::RebuildOverviewTab()
{
    UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("OverviewBox"));
    ContentScrollBox->AddChild(Box);

    UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem();
    AddTextLine(WidgetTree, Box, TEXT("Overview"), FLinearColor(0.95f, 0.98f, 1.0f, 1.0f), 12);
    if (!DebugSubsystem)
    {
        AddTextLine(WidgetTree, Box, TEXT("No debug subsystem available."), FLinearColor::Yellow, 10);
        return;
    }

    AddTextLine(WidgetTree, Box, FString::Printf(TEXT("DebugHub: %s"), DebugSubsystem->IsDebugEnabled() ? TEXT("On") : TEXT("Off")));
    AddTextLine(WidgetTree, Box, FString::Printf(TEXT("Overlay: %s"), DebugSubsystem->IsOverlayVisible() ? TEXT("On") : TEXT("Off")));
    AddTextLine(WidgetTree, Box, FString::Printf(TEXT("Visualization: %s (%s)"), DebugSubsystem->IsVisualizationEnabled() ? TEXT("On") : TEXT("Off"), *GetVisModeDisplayName(DebugSubsystem->GetVisualizationMode())));
    AddTextLine(WidgetTree, Box, FString::Printf(TEXT("Draft pending changes: %d"), ChangedKeys.Num()));
    AddTextLine(WidgetTree, Box, TEXT("Hotkeys: F7 Hub, F8 Overlay, F12 Visualization, [ ] Mode"));
}

void UTajsGraphDebugMenuWidget::RebuildVisualizationTab()
{
    UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VisBox"));
    ContentScrollBox->AddChild(Box);

    AddTextLine(WidgetTree, Box, TEXT("Visualization"), FLinearColor(0.95f, 0.98f, 1.0f, 1.0f), 12);
    UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem();
    if (!DebugSubsystem)
    {
        AddTextLine(WidgetTree, Box, TEXT("No debug subsystem available."), FLinearColor::Yellow, 10);
        return;
    }

    UHorizontalBox* Controls = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("VisControls"));
    Box->AddChildToVerticalBox(Controls);

    VisualizationModeCombo = WidgetTree->ConstructWidget<UComboBoxString>(UComboBoxString::StaticClass(), TEXT("VisualizationModeCombo"));
    if (VisualizationModeCombo)
    {
        VisualizationModes.Reset();
        for (int32 Index = 0; Index <= static_cast<int32>(ETajsGraphVisMode::ShaderComplexity); ++Index)
        {
            const ETajsGraphVisMode Mode = static_cast<ETajsGraphVisMode>(Index);
            VisualizationModes.Add(Mode);
            VisualizationModeCombo->AddOption(GetVisModeDisplayName(Mode));
        }

        VisualizationModeCombo->SetSelectedOption(GetVisModeDisplayName(DebugSubsystem->GetVisualizationMode()));
        VisualizationModeCombo->OnSelectionChanged.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleVisModeSelectionChanged);
        if (UHorizontalBoxSlot* VisualizationModeComboSlot = Controls->AddChildToHorizontalBox(VisualizationModeCombo))
        {
            VisualizationModeComboSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            VisualizationModeComboSlot->SetPadding(FMargin(2.0f));
        }
    }

    if (UButton* Btn = AddButton(WidgetTree, Controls, TEXT("VisToggleBtn"), TEXT("Enable/Disable"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleVisToggleClicked); }
    if (UButton* Btn = AddButton(WidgetTree, Controls, TEXT("VisPrevBtn"), TEXT("Prev"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleVisPrevClicked); }
    if (UButton* Btn = AddButton(WidgetTree, Controls, TEXT("VisNextBtn"), TEXT("Next"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleVisNextClicked); }

    const ETajsGraphVisMode CurrentMode = DebugSubsystem->GetVisualizationMode();
    AddTextLine(WidgetTree, Box, FString::Printf(TEXT("Current: %s (%s)"), *GetVisModeDisplayName(CurrentMode), DebugSubsystem->IsVisualizationEnabled() ? TEXT("enabled") : TEXT("disabled")));
    AddTextLine(WidgetTree, Box, FString::Printf(TEXT("Compatibility: %s"), *GetCompatibilityTextForMode(CurrentMode)), FLinearColor(0.88f, 0.95f, 1.0f, 1.0f));
}
void UTajsGraphDebugMenuWidget::RebuildSettingsTab()
{
    if (SectionCombo)
    {
        SectionCombo->ClearOptions();
        SectionCombo->AddOption(TEXT("All"));

        TSet<FString> UniqueSections;
        for (const FTajsGraphSettingDescriptor& Descriptor : Descriptors)
        {
            if (!bShowAdvanced && Descriptor.bDangerous)
            {
                continue;
            }
            UniqueSections.Add(MakeSectionKey(Descriptor));
        }

        TArray<FString> SortedSections = UniqueSections.Array();
        SortedSections.Sort();
        for (const FString& Section : SortedSections)
        {
            SectionCombo->AddOption(Section);
        }

        if (CurrentSettingsSection.IsEmpty() || SectionCombo->FindOptionIndex(CurrentSettingsSection) < 0)
        {
            CurrentSettingsSection = TEXT("All");
        }

        SectionCombo->SetSelectedOption(CurrentSettingsSection);
    }

    RebuildSettingsRows();
}

void UTajsGraphDebugMenuWidget::RebuildDebugReportsTab()
{
    UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("DebugBox"));
    ContentScrollBox->AddChild(Box);

    AddTextLine(WidgetTree, Box, TEXT("Debug / Reports"), FLinearColor(0.95f, 0.98f, 1.0f, 1.0f), 12);
    UTajsGraphDebugSubsystem* DebugSubsystem = ResolveDebugSubsystem();
    if (!DebugSubsystem)
    {
        AddTextLine(WidgetTree, Box, TEXT("No debug subsystem available."), FLinearColor::Yellow, 10);
        return;
    }

    UHorizontalBox* Row1 = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("DebugRow1"));
    Box->AddChildToVerticalBox(Row1);

    if (UButton* Btn = AddButton(WidgetTree, Row1, TEXT("DbgToggleBtn"), TEXT("Toggle DebugHub"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleDebugToggleClicked); }
    if (UButton* Btn = AddButton(WidgetTree, Row1, TEXT("OverlayToggleBtn"), TEXT("Toggle Overlay"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleOverlayToggleClicked); }

    OverlayPageCombo = WidgetTree->ConstructWidget<UComboBoxString>(UComboBoxString::StaticClass(), TEXT("OverlayPageCombo"));
    if (OverlayPageCombo)
    {
        OverlayPageCombo->AddOption(TEXT("Perf"));
        OverlayPageCombo->AddOption(TEXT("Renderer"));
        OverlayPageCombo->AddOption(TEXT("PostProcess"));
        OverlayPageCombo->AddOption(TEXT("Lights"));
        OverlayPageCombo->AddOption(TEXT("Warnings"));

        switch (DebugSubsystem->GetOverlayPage())
        {
        case ETajsGraphOverlayPage::Perf: OverlayPageCombo->SetSelectedOption(TEXT("Perf")); break;
        case ETajsGraphOverlayPage::Renderer: OverlayPageCombo->SetSelectedOption(TEXT("Renderer")); break;
        case ETajsGraphOverlayPage::PostProcess: OverlayPageCombo->SetSelectedOption(TEXT("PostProcess")); break;
        case ETajsGraphOverlayPage::Lights: OverlayPageCombo->SetSelectedOption(TEXT("Lights")); break;
        case ETajsGraphOverlayPage::Warnings: OverlayPageCombo->SetSelectedOption(TEXT("Warnings")); break;
        default: OverlayPageCombo->SetSelectedOption(TEXT("Perf")); break;
        }

        OverlayPageCombo->OnSelectionChanged.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleOverlayPageSelectionChanged);
        if (UHorizontalBoxSlot* OverlayPageComboSlot = Row1->AddChildToHorizontalBox(OverlayPageCombo))
        {
            OverlayPageComboSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            OverlayPageComboSlot->SetPadding(FMargin(2.0f));
        }
    }

    UHorizontalBox* Row2 = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("DebugRow2"));
    Box->AddChildToVerticalBox(Row2);

    if (UButton* Btn = AddButton(WidgetTree, Row2, TEXT("GenerateReportBtn"), TEXT("Generate Report")))
    {
        Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleGenerateReportClicked);
    }

    UCheckBox* ExpandedCheck = WidgetTree->ConstructWidget<UCheckBox>(UCheckBox::StaticClass(), TEXT("ExpandedArtifactsCheck"));
    if (ExpandedCheck)
    {
        bool bExpanded = false;
        TajsGraphSmlSettings::GetBool(TEXT("DebugHub"), TEXT("WriteExpandedReportArtifacts"), bExpanded);
        ExpandedCheck->SetIsChecked(bExpanded);
        ExpandedCheck->OnCheckStateChanged.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleExpandedArtifactsChanged);
        if (UHorizontalBoxSlot* ExpandedCheckSlot = Row2->AddChildToHorizontalBox(ExpandedCheck))
        {
            ExpandedCheckSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            ExpandedCheckSlot->SetPadding(FMargin(8.0f, 2.0f));
        }
        AddTextLine(WidgetTree, Row2, TEXT("Expanded report artifacts"), FLinearColor(0.88f, 0.95f, 1.0f, 1.0f), 9);
    }

    AddTextLine(WidgetTree, Box, TEXT("Reports: Saved/TajsGraph/Reports"));
}

void UTajsGraphDebugMenuWidget::RebuildProfilesTab()
{
    UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("ProfilesBox"));
    ContentScrollBox->AddChild(Box);
    AddTextLine(WidgetTree, Box, TEXT("Profiles"), FLinearColor(0.95f, 0.98f, 1.0f, 1.0f), 12);

    UHorizontalBox* SelectRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ProfilesSelectRow"));
    Box->AddChildToVerticalBox(SelectRow);

    ProfilesCombo = WidgetTree->ConstructWidget<UComboBoxString>(UComboBoxString::StaticClass(), TEXT("ProfilesCombo"));
    if (ProfilesCombo)
    {
        ProfilesCombo->OnSelectionChanged.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleProfileSelectionChanged);
        if (UHorizontalBoxSlot* ProfilesComboSlot = SelectRow->AddChildToHorizontalBox(ProfilesCombo))
        {
            ProfilesComboSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            ProfilesComboSlot->SetPadding(FMargin(2.0f));
        }
    }

    ProfileNameTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("ProfileNameTextBox"));
    if (ProfileNameTextBox)
    {
        ProfileNameTextBox->SetHintText(FText::FromString(TEXT("Profile name")));
        ProfileNameTextBox->SetMinDesiredWidth(220.0f);
        if (UHorizontalBoxSlot* ProfileNameTextBoxSlot = SelectRow->AddChildToHorizontalBox(ProfileNameTextBox))
        {
            ProfileNameTextBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            ProfileNameTextBoxSlot->SetPadding(FMargin(4.0f, 2.0f));
        }
    }

    UHorizontalBox* Actions = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ProfilesActions"));
    Box->AddChildToVerticalBox(Actions);
    if (UButton* Btn = AddButton(WidgetTree, Actions, TEXT("ProfileCreateBtn"), TEXT("Create"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleProfileCreateClicked); }
    if (UButton* Btn = AddButton(WidgetTree, Actions, TEXT("ProfileSaveBtn"), TEXT("Save/Overwrite"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleProfileSaveClicked); }
    if (UButton* Btn = AddButton(WidgetTree, Actions, TEXT("ProfileLoadBtn"), TEXT("Load -> Draft"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleProfileLoadClicked); }
    if (UButton* Btn = AddButton(WidgetTree, Actions, TEXT("ProfileRenameBtn"), TEXT("Rename"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleProfileRenameClicked); }
    if (UButton* Btn = AddButton(WidgetTree, Actions, TEXT("ProfileDuplicateBtn"), TEXT("Duplicate"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleProfileDuplicateClicked); }
    if (UButton* Btn = AddButton(WidgetTree, Actions, TEXT("ProfileDeleteBtn"), TEXT("Delete"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleProfileDeleteClicked); }

    UHorizontalBox* ImportRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ProfilesImportRow"));
    Box->AddChildToVerticalBox(ImportRow);

    ImportPathTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("ImportPathTextBox"));
    if (ImportPathTextBox)
    {
        ImportPathTextBox->SetHintText(FText::FromString(TEXT("Import path (.json)")));
        ImportPathTextBox->SetMinDesiredWidth(360.0f);
        if (UHorizontalBoxSlot* ImportPathTextBoxSlot = ImportRow->AddChildToHorizontalBox(ImportPathTextBox))
        {
            ImportPathTextBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            ImportPathTextBoxSlot->SetPadding(FMargin(2.0f));
        }
    }
    if (UButton* Btn = AddButton(WidgetTree, ImportRow, TEXT("ImportBtn"), TEXT("Import"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleProfileImportClicked); }

    UHorizontalBox* ExportRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ProfilesExportRow"));
    Box->AddChildToVerticalBox(ExportRow);

    ExportPathTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("ExportPathTextBox"));
    if (ExportPathTextBox)
    {
        ExportPathTextBox->SetHintText(FText::FromString(TEXT("Export path (.json)")));
        ExportPathTextBox->SetMinDesiredWidth(360.0f);
        if (UHorizontalBoxSlot* ExportPathTextBoxSlot = ExportRow->AddChildToHorizontalBox(ExportPathTextBox))
        {
            ExportPathTextBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            ExportPathTextBoxSlot->SetPadding(FMargin(2.0f));
        }
    }
    if (UButton* Btn = AddButton(WidgetTree, ExportRow, TEXT("ExportBtn"), TEXT("Export"))) { Btn->OnClicked.AddDynamic(this, &UTajsGraphDebugMenuWidget::HandleProfileExportClicked); }

    AddTextLine(WidgetTree, Box, FString::Printf(TEXT("Profiles directory: %s"), *GetProfilesRootDir()));
    RefreshProfileList();
}

void UTajsGraphDebugMenuWidget::RebuildSettingsRows()
{
    if (!ContentScrollBox)
    {
        return;
    }

    ContentScrollBox->ClearChildren();
    SettingRows.Reset();

    UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("SettingsRowsBox"));
    ContentScrollBox->AddChild(Box);

    AddTextLine(WidgetTree, Box, TEXT("Settings"), FLinearColor(0.95f, 0.98f, 1.0f, 1.0f), 12);

    int32 AddedCount = 0;
    for (const FTajsGraphSettingDescriptor& Descriptor : Descriptors)
    {
        if (!bShowAdvanced && Descriptor.bDangerous)
        {
            continue;
        }

        const FString SectionKey = MakeSectionKey(Descriptor);
        if (!CurrentSettingsSection.IsEmpty() && CurrentSettingsSection != TEXT("All") && CurrentSettingsSection != SectionKey)
        {
            continue;
        }

        UTajsGraphHubSettingRowWidget* RowWidget = CreateWidget<UTajsGraphHubSettingRowWidget>(GetWorld(), UTajsGraphHubSettingRowWidget::StaticClass());
        if (!RowWidget)
        {
            continue;
        }

        const FString* DraftValue = DraftValues.Find(Descriptor.Id);
        RowWidget->InitializeRow(this, Descriptor, DraftValue ? *DraftValue : Descriptor.DefaultValue);

        if (UVerticalBoxSlot* RowSlot = Box->AddChildToVerticalBox(RowWidget))
        {
            RowSlot->SetPadding(FMargin(0.0f, 1.0f));
        }

        SettingRows.Add(RowWidget);
        ++AddedCount;
    }

    AddTextLine(WidgetTree, Box, FString::Printf(TEXT("Rendered %d row(s). Section=%s"), AddedCount, *MakeSectionLabel(CurrentSettingsSection)), FLinearColor(0.80f, 0.90f, 1.0f, 1.0f), 9);
}
void UTajsGraphDebugMenuWidget::LoadDescriptors()
{
    Descriptors = FTajsGraphModule::GetSettingDescriptors();
    DescriptorById.Reset();
    for (const FTajsGraphSettingDescriptor& Descriptor : Descriptors)
    {
        DescriptorById.Add(Descriptor.Id, Descriptor);
    }

    if (CurrentSettingsSection.IsEmpty())
    {
        CurrentSettingsSection = TEXT("All");
    }
}

void UTajsGraphDebugMenuWidget::InitializeDraftFromLive(const bool bResetBaseline)
{
    for (const FTajsGraphSettingDescriptor& Descriptor : Descriptors)
    {
        FString LiveValue;
        if (!TryReadLiveValue(Descriptor, LiveValue))
        {
            LiveValue = Descriptor.DefaultValue;
        }

        DraftValues.Add(Descriptor.Id, LiveValue);
        if (bResetBaseline)
        {
            BaselineValues.Add(Descriptor.Id, LiveValue);
        }
    }

    if (bResetBaseline)
    {
        ChangedKeys.Reset();
    }
}

bool UTajsGraphDebugMenuWidget::TryReadLiveValue(const FTajsGraphSettingDescriptor& Descriptor, FString& OutValue) const
{
    const FString& SectionName = Descriptor.Section;
    const FString& KeyName = Descriptor.Key;

    switch (Descriptor.Type)
    {
    case ETajsGraphSettingType::Bool:
    {
        bool bValue = false;
        if (!TajsGraphSmlSettings::GetBool(SectionName, KeyName, bValue))
        {
            return false;
        }
        OutValue = bValue ? TEXT("true") : TEXT("false");
        return true;
    }
    case ETajsGraphSettingType::Int:
    {
        int32 Value = 0;
        if (!TajsGraphSmlSettings::GetInt(SectionName, KeyName, Value))
        {
            return false;
        }
        OutValue = FString::FromInt(Value);
        return true;
    }
    case ETajsGraphSettingType::Float:
    {
        float Value = 0.0f;
        if (!TajsGraphSmlSettings::GetFloat(SectionName, KeyName, Value))
        {
            return false;
        }
        OutValue = FString::SanitizeFloat(Value);
        return true;
    }
    case ETajsGraphSettingType::Color:
    case ETajsGraphSettingType::String:
    default:
    {
        FString Value;
        if (!TajsGraphSmlSettings::GetString(SectionName, KeyName, Value))
        {
            return false;
        }
        OutValue = Value;
        return true;
    }
    }
}

bool UTajsGraphDebugMenuWidget::ApplySettingValue(const FTajsGraphSettingDescriptor& Descriptor, const FString& Value, FString& OutError) const
{
    const FString& SectionName = Descriptor.Section;
    const FString& KeyName = Descriptor.Key;

    switch (Descriptor.Type)
    {
    case ETajsGraphSettingType::Bool:
    {
        if (!PersistBoolSetting(SectionName, KeyName, ParseBoolText(Value)))
        {
            OutError = TEXT("SetBool failed");
            return false;
        }
        return true;
    }
    case ETajsGraphSettingType::Int:
    {
        if (!PersistIntSetting(SectionName, KeyName, FCString::Atoi(*Value.TrimStartAndEnd())))
        {
            OutError = TEXT("SetInt failed");
            return false;
        }
        return true;
    }
    case ETajsGraphSettingType::Float:
    {
        if (!PersistFloatSetting(SectionName, KeyName, FCString::Atof(*Value.TrimStartAndEnd())))
        {
            OutError = TEXT("SetFloat failed");
            return false;
        }
        return true;
    }
    case ETajsGraphSettingType::Color:
    case ETajsGraphSettingType::String:
    default:
    {
        if (!PersistStringSetting(SectionName, KeyName, Value))
        {
            OutError = TEXT("SetString failed");
            return false;
        }
        return true;
    }
    }
}

ETajsGraphVisMode UTajsGraphDebugMenuWidget::ParseVisModeFromName(const FString& Name) const
{
    const FString Normalized = NormalizeText(Name);
    for (int32 Index = 0; Index <= static_cast<int32>(ETajsGraphVisMode::ShaderComplexity); ++Index)
    {
        const ETajsGraphVisMode Mode = static_cast<ETajsGraphVisMode>(Index);
        if (NormalizeText(GetVisModeDisplayName(Mode)) == Normalized)
        {
            return Mode;
        }
    }
    return ETajsGraphVisMode::Default;
}

FString UTajsGraphDebugMenuWidget::GetVisModeDisplayName(const ETajsGraphVisMode Mode) const
{
    if (const UEnum* Enum = StaticEnum<ETajsGraphVisMode>())
    {
        return Enum->GetNameStringByValue(static_cast<int64>(Mode));
    }
    return TEXT("Default");
}

FString UTajsGraphDebugMenuWidget::GetCompatibilityTextForMode(const ETajsGraphVisMode Mode) const
{
    bool bEngineAttempt = true;
    bool bRuntimeCVars = true;
    bool bPPFallback = true;
    TajsGraphSmlSettings::GetBool(TEXT("Visualization"), TEXT("EnableEngineViewAttempt"), bEngineAttempt);
    TajsGraphSmlSettings::GetBool(TEXT("Visualization"), TEXT("EnableRuntimeVisCVars"), bRuntimeCVars);
    TajsGraphSmlSettings::GetBool(TEXT("Visualization"), TEXT("EnablePostProcessFallback"), bPPFallback);

    const bool bEngineMode = Mode == ETajsGraphVisMode::Lit || Mode == ETajsGraphVisMode::Unlit || Mode == ETajsGraphVisMode::Wireframe ||
        Mode == ETajsGraphVisMode::DetailLighting || Mode == ETajsGraphVisMode::LightingOnly || Mode == ETajsGraphVisMode::Reflections ||
        Mode == ETajsGraphVisMode::ShaderComplexity || Mode == ETajsGraphVisMode::BaseColor || Mode == ETajsGraphVisMode::WorldNormal ||
        Mode == ETajsGraphVisMode::Roughness || Mode == ETajsGraphVisMode::Metallic || Mode == ETajsGraphVisMode::Specular ||
        Mode == ETajsGraphVisMode::AmbientOcclusion || Mode == ETajsGraphVisMode::SceneDepth;

    const bool bRuntimeMode = Mode == ETajsGraphVisMode::LumenCards || Mode == ETajsGraphVisMode::MeshDistanceFields || Mode == ETajsGraphVisMode::GlobalDistanceField;
    const bool bFallbackMode = Mode == ETajsGraphVisMode::LuminanceHeatmap || Mode == ETajsGraphVisMode::PseudoWireframe;

    if (Mode == ETajsGraphVisMode::Default)
    {
        return TEXT("Default renderer mode");
    }
    if (bEngineMode)
    {
        if (bEngineAttempt)
        {
            return TEXT("Supported via engine view mode");
        }
        return bPPFallback ? TEXT("Engine disabled; fallback may apply") : TEXT("Engine disabled; likely unsupported");
    }
    if (bRuntimeMode)
    {
        return bRuntimeCVars ? TEXT("Supported via visualization CVars") : TEXT("Runtime CVar path disabled");
    }
    if (bFallbackMode)
    {
        return bPPFallback ? TEXT("Supported via post-process fallback") : TEXT("Post-process fallback disabled");
    }

    return TEXT("Unknown compatibility");
}

FString UTajsGraphDebugMenuWidget::GetProfilesRootDir() const
{
    return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("TajsGraph"), TEXT("Profiles"));
}

FString UTajsGraphDebugMenuWidget::GetProfilePath(const FString& ProfileName) const
{
    return FPaths::Combine(GetProfilesRootDir(), MakeSafeProfileName(ProfileName) + TEXT(".json"));
}

void UTajsGraphDebugMenuWidget::RefreshProfileList(const FString& PreferredSelection)
{
    if (!ProfilesCombo)
    {
        return;
    }

    IFileManager::Get().MakeDirectory(*GetProfilesRootDir(), true);
    ProfilesCombo->ClearOptions();

    TArray<FString> Files;
    IFileManager::Get().FindFiles(Files, *FPaths::Combine(GetProfilesRootDir(), TEXT("*.json")), true, false);
    Files.Sort();

    FString ActiveProfile;
    FFileHelper::LoadFileToString(ActiveProfile, *FPaths::Combine(GetProfilesRootDir(), TEXT("active_profile.txt")));
    ActiveProfile = MakeSafeProfileName(ActiveProfile);

    FString FirstOption;
    for (const FString& File : Files)
    {
        FString Name = File;
        Name.RemoveFromEnd(TEXT(".json"));
        Name = MakeSafeProfileName(Name);
        if (Name.IsEmpty())
        {
            continue;
        }

        if (FirstOption.IsEmpty())
        {
            FirstOption = Name;
        }
        ProfilesCombo->AddOption(Name);
    }

    FString ToSelect = PreferredSelection;
    if (ToSelect.IsEmpty()) { ToSelect = ActiveProfile; }
    if (ToSelect.IsEmpty()) { ToSelect = FirstOption; }
    if (!ToSelect.IsEmpty())
    {
        ProfilesCombo->SetSelectedOption(ToSelect);
        if (ProfileNameTextBox)
        {
            ProfileNameTextBox->SetText(FText::FromString(ToSelect));
        }
    }
}
bool UTajsGraphDebugMenuWidget::SaveProfileToPath(const FString& ProfileName, const FString& FilePath, const bool bOverwrite, FString& OutError) const
{
    const FString SafeName = MakeSafeProfileName(ProfileName);
    if (SafeName.IsEmpty())
    {
        OutError = TEXT("empty profile name");
        return false;
    }

    if (!bOverwrite && IFileManager::Get().FileExists(*FilePath))
    {
        OutError = TEXT("target file already exists");
        return false;
    }

    TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();
    Root->SetStringField(TEXT("name"), SafeName);
    Root->SetStringField(TEXT("created_utc"), FDateTime::UtcNow().ToString(TEXT("%Y-%m-%dT%H:%M:%SZ")));
    Root->SetStringField(TEXT("updated_utc"), FDateTime::UtcNow().ToString(TEXT("%Y-%m-%dT%H:%M:%SZ")));
    Root->SetNumberField(TEXT("schema_version"), FTajsGraphModule::GetSettingsSchemaVersion());

    TSharedPtr<FJsonObject> SettingsObject = MakeShared<FJsonObject>();
    TArray<FString> Keys;
    DraftValues.GenerateKeyArray(Keys);
    Keys.Sort();
    for (const FString& Key : Keys)
    {
        if (const FString* Value = DraftValues.Find(Key))
        {
            SettingsObject->SetStringField(Key, *Value);
        }
    }
    Root->SetObjectField(TEXT("settings"), SettingsObject);

    FString Serialized;
    const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Serialized);
    if (!FJsonSerializer::Serialize(Root.ToSharedRef(), Writer))
    {
        OutError = TEXT("failed to serialize profile json");
        return false;
    }

    IFileManager::Get().MakeDirectory(*FPaths::GetPath(FilePath), true);
    if (!FFileHelper::SaveStringToFile(Serialized, *FilePath))
    {
        OutError = TEXT("failed writing profile file");
        return false;
    }

    return true;
}

bool UTajsGraphDebugMenuWidget::LoadProfileFromPath(const FString& FilePath, FString& OutProfileName, TMap<FString, FString>& OutSettings, TArray<FString>& OutUnknownKeys, FString& OutError) const
{
    FString Json;
    if (!FFileHelper::LoadFileToString(Json, *FilePath))
    {
        OutError = TEXT("cannot read profile file");
        return false;
    }

    TSharedPtr<FJsonObject> Root;
    const TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(Json);
    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
    {
        OutError = TEXT("invalid profile json");
        return false;
    }

    Root->TryGetStringField(TEXT("name"), OutProfileName);

    const TSharedPtr<FJsonObject>* SettingsObjectPtr = nullptr;
    if (!Root->TryGetObjectField(TEXT("settings"), SettingsObjectPtr) || !SettingsObjectPtr || !SettingsObjectPtr->IsValid())
    {
        OutError = TEXT("profile missing settings object");
        return false;
    }
    const TSharedPtr<FJsonObject> SettingsObject = *SettingsObjectPtr;

    OutSettings.Reset();
    OutUnknownKeys.Reset();
    for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : SettingsObject->Values)
    {
        FString Value;
        if (!Pair.Value.IsValid() || !Pair.Value->TryGetString(Value))
        {
            continue;
        }

        if (DescriptorById.Contains(Pair.Key))
        {
            OutSettings.Add(Pair.Key, Value);
        }
        else
        {
            OutUnknownKeys.Add(Pair.Key);
        }
    }

    return true;
}

void UTajsGraphDebugMenuWidget::SetStatusMessage(const FString& Message)
{
    LastStatusMessage = Message;
    if (StatusText)
    {
        StatusText->SetText(FText::FromString(Message));
    }
    BP_OnStatusMessageChanged(Message);
}







