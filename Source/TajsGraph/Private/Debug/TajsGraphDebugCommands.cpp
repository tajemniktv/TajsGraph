#include "Debug/TajsGraphDebugCommands.h"

#include "Debug/TajsGraphDebugSubsystem.h"
#include "Engine/Engine.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "Input/Events.h"
#include "InputCoreTypes.h"

namespace {

class FTajsGraphDebugInputProcessor : public IInputProcessor {
public:
    virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {
    }

    virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override {
        UTajsGraphDebugSubsystem* DebugSubsystem = ResolveSubsystem();
        if (!DebugSubsystem) {
            return false;
        }

        const FKey Key = InKeyEvent.GetKey();
        if (Key == EKeys::F7) {
            DebugSubsystem->ToggleMenu();
            return true;
        }
        if (Key == EKeys::F8) {
            DebugSubsystem->ToggleOverlay();
            return true;
        }
        if (Key == EKeys::F9) {
            const FTajsGraphDebugSnapshot Snapshot = DebugSubsystem->CreateSnapshot(TEXT("ManualSnapshot"), 10.0f);
            DebugSubsystem->WriteReport(Snapshot, FString());
            return true;
        }
        if (Key == EKeys::F10) {
            DebugSubsystem->RunSceneProbe();
            return true;
        }
        if (Key == EKeys::F11) {
            DebugSubsystem->GenerateReportBundle();
            return true;
        }
        if (Key == EKeys::F12) {
            DebugSubsystem->ToggleVisualization();
            return true;
        }
        if (Key == EKeys::LeftBracket) {
            DebugSubsystem->PreviousVisualizationMode();
            return true;
        }
        if (Key == EKeys::RightBracket) {
            DebugSubsystem->NextVisualizationMode();
            return true;
        }

        return false;
    }

private:
    static UTajsGraphDebugSubsystem* ResolveSubsystem() {
        if (!GEngine) {
            return nullptr;
        }

        for (const FWorldContext& Context : GEngine->GetWorldContexts()) {
            UWorld* World = Context.World();
            if (!World || (Context.WorldType != EWorldType::Game && Context.WorldType != EWorldType::PIE)) {
                continue;
            }

            if (UGameInstance* GameInstance = World->GetGameInstance()) {
                return GameInstance->GetSubsystem<UTajsGraphDebugSubsystem>();
            }
        }

        return nullptr;
    }
};

TSharedPtr<FTajsGraphDebugInputProcessor> GInputProcessor;

}

void FTajsGraphDebugCommands::Startup() {
    if (GInputProcessor.IsValid()) {
        return;
    }

    if (!FSlateApplication::IsInitialized()) {
        return;
    }

    GInputProcessor = MakeShared<FTajsGraphDebugInputProcessor>();
    FSlateApplication::Get().RegisterInputPreProcessor(GInputProcessor);
}

void FTajsGraphDebugCommands::Shutdown() {
    if (!GInputProcessor.IsValid()) {
        return;
    }

    if (FSlateApplication::IsInitialized()) {
        FSlateApplication::Get().UnregisterInputPreProcessor(GInputProcessor);
    }
    GInputProcessor.Reset();
}
