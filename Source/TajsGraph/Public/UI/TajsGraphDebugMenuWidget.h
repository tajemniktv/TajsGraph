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
class UPanelWidget;
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
    void ResolveWidgetReferences();
    void BindButtonHandler(UButton* Button, void (UTajsGraphDebugMenuWidget::*Handler)());
    void UpdateBoundShellState();
    void SetPageHeaderCopy(const FString& Title, const FString& Lead);
    UPanelWidget* GetContentHost() const;
    UPanelWidget* GetActivePageBodyHost() const;
    UPanelWidget* FindActivePagePanel(const TCHAR* PrimaryName, const TCHAR* SecondaryName = nullptr) const;
    UClass* ResolvePageShellClass(ETajsGraphHubTab Tab) const;
    void CreateActivePageShell();
    void PopulateSettingsRows(UVerticalBox* Box);

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
    UPanelWidget* ContentHost = nullptr;

    UPROPERTY(Transient)
    UUserWidget* ActivePageShellWidget = nullptr;

    UPROPERTY(Transient)
    UPanelWidget* ActivePageBodyHost = nullptr;

    UPROPERTY(Transient)
    UTextBlock* PageTitleText = nullptr;

    UPROPERTY(Transient)
    UTextBlock* PageLeadText = nullptr;

    UPROPERTY(Transient)
    UTextBlock* FooterText = nullptr;

    UPROPERTY(Transient)
    UButton* CloseHubButton = nullptr;

    UPROPERTY(Transient)
    UButton* OverviewNavButton = nullptr;

    UPROPERTY(Transient)
    UButton* VisualizationNavButton = nullptr;

    UPROPERTY(Transient)
    UButton* SettingsNavButton = nullptr;

    UPROPERTY(Transient)
    UButton* ReportsNavButton = nullptr;

    UPROPERTY(Transient)
    UButton* ProfilesNavButton = nullptr;

    UPROPERTY(Transient)
    UTextBlock* OverviewNavLabel = nullptr;

    UPROPERTY(Transient)
    UTextBlock* VisualizationNavLabel = nullptr;

    UPROPERTY(Transient)
    UTextBlock* SettingsNavLabel = nullptr;

    UPROPERTY(Transient)
    UTextBlock* ReportsNavLabel = nullptr;

    UPROPERTY(Transient)
    UTextBlock* ProfilesNavLabel = nullptr;

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
    bool bHasExternalSettingsControls = false;
    FString LastStatusMessage;

    float RefreshAccumulatorSeconds = 0.0f;
    float RefreshIntervalSeconds = 0.25f;
};


