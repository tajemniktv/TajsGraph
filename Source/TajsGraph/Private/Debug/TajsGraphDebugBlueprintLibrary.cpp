#include "Debug/TajsGraphDebugBlueprintLibrary.h"

#include "Debug/TajsGraphDebugSubsystem.h"
#include "Engine/World.h"

namespace {

static UTajsGraphDebugSubsystem* ResolveDebugSubsystem(const UObject* WorldContextObject) {
    if (!WorldContextObject) {
        return nullptr;
    }

    if (const UWorld* World = WorldContextObject->GetWorld()) {
        if (UGameInstance* GameInstance = World->GetGameInstance()) {
            return GameInstance->GetSubsystem<UTajsGraphDebugSubsystem>();
        }
    }

    return nullptr;
}

}

void UTajsGraphDebugBlueprintLibrary::SetDebugEnabled(const UObject* WorldContextObject, const bool bEnabled) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        Debug->SetDebugEnabled(bEnabled);
    }
}

bool UTajsGraphDebugBlueprintLibrary::IsDebugEnabled(const UObject* WorldContextObject) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        return Debug->IsDebugEnabled();
    }
    return false;
}

void UTajsGraphDebugBlueprintLibrary::ToggleOverlay(const UObject* WorldContextObject) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        Debug->ToggleOverlay();
    }
}

void UTajsGraphDebugBlueprintLibrary::SetOverlayPage(const UObject* WorldContextObject, const ETajsGraphOverlayPage Page) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        Debug->SetOverlayPage(Page);
    }
}

FTajsGraphDebugSnapshot UTajsGraphDebugBlueprintLibrary::CreateSnapshot(const UObject* WorldContextObject, const FString& Reason, const float HistorySeconds) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        return Debug->CreateSnapshot(Reason, HistorySeconds);
    }
    return FTajsGraphDebugSnapshot();
}

FTajsGraphSceneProbeSample UTajsGraphDebugBlueprintLibrary::RunSceneProbe(const UObject* WorldContextObject) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        return Debug->RunSceneProbe();
    }
    return FTajsGraphSceneProbeSample();
}

FString UTajsGraphDebugBlueprintLibrary::GenerateReport(const UObject* WorldContextObject) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        return Debug->GenerateReportBundle();
    }
    return FString();
}

void UTajsGraphDebugBlueprintLibrary::ClearHistory(const UObject* WorldContextObject) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        Debug->ClearHistory();
    }
}

void UTajsGraphDebugBlueprintLibrary::SetVisualizationMode(const UObject* WorldContextObject, const ETajsGraphVisMode Mode) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        Debug->SetVisualizationMode(Mode);
    }
}

ETajsGraphVisMode UTajsGraphDebugBlueprintLibrary::NextVisualizationMode(const UObject* WorldContextObject) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        return Debug->NextVisualizationMode();
    }
    return ETajsGraphVisMode::Default;
}

ETajsGraphVisMode UTajsGraphDebugBlueprintLibrary::PreviousVisualizationMode(const UObject* WorldContextObject) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        return Debug->PreviousVisualizationMode();
    }
    return ETajsGraphVisMode::Default;
}

bool UTajsGraphDebugBlueprintLibrary::ToggleVisualization(const UObject* WorldContextObject) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        return Debug->ToggleVisualization();
    }
    return false;
}

TArray<FString> UTajsGraphDebugBlueprintLibrary::GetOverlayLines(const UObject* WorldContextObject) {
    if (UTajsGraphDebugSubsystem* Debug = ResolveDebugSubsystem(WorldContextObject)) {
        return Debug->GetOverlayLines();
    }
    return {};
}
