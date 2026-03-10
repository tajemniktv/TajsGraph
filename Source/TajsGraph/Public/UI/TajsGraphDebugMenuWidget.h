#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Debug/TajsGraphDebugTypes.h"
#include "TajsGraphSettingsSchema.h"
#include "TajsGraphDebugMenuWidget.generated.h"

class UBorder;
class UButton;
class UCanvasPanel;
class UCheckBox;
class UComboBoxString;
class UEditableTextBox;
class UScrollBox;
class UTextBlock;
class UVerticalBox;
class UTajsGraphDebugSubsystem;
class UTajsGraphHubSettingRowWidget;

UENUM()
enum class ETajsGraphHubTab : uint8
{
    Overview,
    Visualization,
    Settings,
    DebugReports,
    Profiles
};

UCLASS()
class TAJSGRAPH_API UTajsGraphDebugMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void RefreshHubData(bool bResetBaseline = true);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleSettingRowDraftChanged(const FString& SettingId, const FString& NewValue);

    UFUNCTION(BlueprintPure, Category = "TajsGraph|DebugHub")
    ETajsGraphHubTab GetActiveTab() const { return ActiveTab; }

    UFUNCTION(BlueprintPure, Category = "TajsGraph|DebugHub")
    const FString& GetStatusMessageText() const { return LastStatusMessage; }

    UFUNCTION(BlueprintImplementableEvent, Category = "TajsGraph|DebugHub")
    void BP_OnHubDataRefreshed();

    UFUNCTION(BlueprintImplementableEvent, Category = "TajsGraph|DebugHub")
    void BP_OnActiveTabChanged(ETajsGraphHubTab NewTab);

    UFUNCTION(BlueprintImplementableEvent, Category = "TajsGraph|DebugHub")
    void BP_OnStatusMessageChanged(const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleTabOverview();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleTabVisualization();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleTabSettings();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleTabDebugReports();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleTabProfiles();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleCloseClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleApplyDraftClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleRevertAllClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleResetSectionClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleAdvancedFilterChanged(bool bChecked);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleSectionSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleVisToggleClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleVisPrevClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleVisNextClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleVisModeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleDebugToggleClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleOverlayToggleClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleOverlayPageSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleGenerateReportClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleExpandedArtifactsChanged(bool bChecked);

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleProfileCreateClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleProfileSaveClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleProfileLoadClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleProfileRenameClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleProfileDuplicateClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleProfileDeleteClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleProfileImportClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleProfileExportClicked();

    UFUNCTION(BlueprintCallable, Category = "TajsGraph|DebugHub")
    void HandleProfileSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

private:
    UTajsGraphDebugSubsystem* ResolveDebugSubsystem() const;

    void BuildBaseLayout();

    void SetActiveTab(ETajsGraphHubTab NewTab);
    void RebuildActiveTabContent();
    void RebuildOverviewTab();
    void RebuildVisualizationTab();
    void RebuildSettingsTab();
    void RebuildDebugReportsTab();
    void RebuildProfilesTab();
    void RebuildSettingsRows();

    void LoadDescriptors();
    void InitializeDraftFromLive(bool bResetBaseline);
    bool TryReadLiveValue(const FTajsGraphSettingDescriptor& Descriptor, FString& OutValue) const;
    bool ApplySettingValue(const FTajsGraphSettingDescriptor& Descriptor, const FString& Value, FString& OutError) const;
    ETajsGraphVisMode ParseVisModeFromName(const FString& Name) const;
    FString GetVisModeDisplayName(ETajsGraphVisMode Mode) const;
    FString GetCompatibilityTextForMode(ETajsGraphVisMode Mode) const;

    FString GetProfilesRootDir() const;
    FString GetProfilePath(const FString& ProfileName) const;
    void RefreshProfileList(const FString& PreferredSelection = FString());
    bool SaveProfileToPath(const FString& ProfileName, const FString& FilePath, bool bOverwrite, FString& OutError) const;
    bool LoadProfileFromPath(const FString& FilePath, FString& OutProfileName, TMap<FString, FString>& OutSettings, TArray<FString>& OutUnknownKeys, FString& OutError) const;
    void SetStatusMessage(const FString& Message);

private:
    UPROPERTY(Transient)
    UCanvasPanel* RootCanvas = nullptr;

    UPROPERTY(Transient)
    UBorder* RootPanel = nullptr;

    UPROPERTY(Transient)
    UVerticalBox* RootBox = nullptr;

    UPROPERTY(Transient)
    UTextBlock* HeaderText = nullptr;

    UPROPERTY(Transient)
    UTextBlock* StatusText = nullptr;

    UPROPERTY(Transient)
    UScrollBox* ContentScrollBox = nullptr;

    UPROPERTY(Transient)
    UCheckBox* AdvancedCheckBox = nullptr;

    UPROPERTY(Transient)
    UComboBoxString* SectionCombo = nullptr;

    UPROPERTY(Transient)
    UComboBoxString* VisualizationModeCombo = nullptr;

    UPROPERTY(Transient)
    UComboBoxString* OverlayPageCombo = nullptr;

    UPROPERTY(Transient)
    UComboBoxString* ProfilesCombo = nullptr;

    UPROPERTY(Transient)
    UEditableTextBox* ProfileNameTextBox = nullptr;

    UPROPERTY(Transient)
    UEditableTextBox* ImportPathTextBox = nullptr;

    UPROPERTY(Transient)
    UEditableTextBox* ExportPathTextBox = nullptr;

    UPROPERTY(Transient)
    TArray<UTajsGraphHubSettingRowWidget*> SettingRows;

    TArray<FTajsGraphSettingDescriptor> Descriptors;
    TMap<FString, FTajsGraphSettingDescriptor> DescriptorById;
    TMap<FString, FString> DraftValues;
    TMap<FString, FString> BaselineValues;
    TSet<FString> ChangedKeys;
    TArray<ETajsGraphVisMode> VisualizationModes;
    ETajsGraphHubTab ActiveTab = ETajsGraphHubTab::Overview;
    FString CurrentSettingsSection;
    bool bShowAdvanced = false;
    FString LastStatusMessage;

    float RefreshAccumulatorSeconds = 0.0f;
    float RefreshIntervalSeconds = 0.25f;
};


