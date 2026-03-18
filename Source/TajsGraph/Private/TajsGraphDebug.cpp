#include "TajsGraphDebug.h"
#include "TajsGraphLog.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Forward declaration to Surface Cache subsystem
#include "TajsGraphSurfaceCache.h"

void UTajsGraphDebug::SetVisualizationMode(const UObject* WorldContextObject, ETajsGraphVisMode Mode)
{
    if (!WorldContextObject) return;
    UWorld* World = WorldContextObject->GetWorld();
    if (!World) return;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC) return;

    // Reset viewmode first
    PC->ConsoleCommand(TEXT("viewmode lit"), true);

    FString Command = TEXT("");

    switch (Mode)
    {
    case ETajsGraphVisMode::Default:
        // Already lit
        break;
    case ETajsGraphVisMode::BaseColor:
        Command = TEXT("viewmode buffer basecolor");
        break;
    case ETajsGraphVisMode::WorldNormal:
        Command = TEXT("viewmode buffer worldnormal");
        break;
    case ETajsGraphVisMode::Roughness:
        Command = TEXT("viewmode buffer roughness");
        break;
    case ETajsGraphVisMode::Metallic:
        Command = TEXT("viewmode buffer metallic");
        break;
    case ETajsGraphVisMode::Specular:
        Command = TEXT("viewmode buffer specular");
        break;
    case ETajsGraphVisMode::AmbientOcclusion:
        Command = TEXT("viewmode buffer ambientocclusion");
        break;
    case ETajsGraphVisMode::SceneDepth:
        Command = TEXT("viewmode buffer scenedepth");
        break;
    case ETajsGraphVisMode::LuminanceHeatmap:
        // Typically a custom material, but we issue the legacy command structure
        // This will require the material to be mapped in DefaultEngine.ini
        Command = TEXT("viewmode custom M_TajsGraph_Vis_LuminanceHeatmap");
        break;
    case ETajsGraphVisMode::PseudoWireframe:
        Command = TEXT("viewmode custom M_TajsGraph_Vis_PseudoWireframe");
        break;
    }

    if (!Command.IsEmpty())
    {
        PC->ConsoleCommand(Command, true);
        UE_LOG(LogTajsGraph, Log, TEXT("Set Visualization Mode changed to %d: %s"), (int32)Mode, *Command);
    }
    else
    {
        UE_LOG(LogTajsGraph, Log, TEXT("Set Visualization Mode to Default (Lit)."));
    }
}

void UTajsGraphDebug::DumpCVars(const UObject* WorldContextObject)
{
    if (!WorldContextObject) return;
    UWorld* World = WorldContextObject->GetWorld();
    if (World && World->GetFirstPlayerController())
    {
        World->GetFirstPlayerController()->ConsoleCommand(TEXT("Help"), true); // Dumps to log
    }
    UE_LOG(LogTajsGraph, Display, TEXT("DumpCVars initiated. Check console log output."));
}

void UTajsGraphDebug::DumpSurfaceCacheTrace(const UObject* WorldContextObject, int32 MaxCount)
{
    UTajsGraphSurfaceCache::DumpSurfaceCacheTrace(MaxCount);
}

// -------------------------------------------------------------------------
// Console Commands
// -------------------------------------------------------------------------

static FAutoConsoleCommand GTajsGraphDumpSurfaceCacheCmd(
    TEXT("TajsGraph.DumpSurfaceCacheTrace"),
    TEXT("Dumps the recent surface cache traces to the log."),
    FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
    {
        int32 MaxCount = 50;
        if (Args.Num() > 0)
        {
            MaxCount = FCString::Atoi(*Args[0]);
        }
        UTajsGraphSurfaceCache::DumpSurfaceCacheTrace(MaxCount);
    })
);

void UTajsGraphDebug::RegisterConsoleCommands()
{
    // AutoConsoleCommand macros handle registration globally
    UE_LOG(LogTajsGraph, Log, TEXT("TajsGraph Console Commands Registered."));
}

void UTajsGraphDebug::UnregisterConsoleCommands()
{
    // Normally handled via destruction of FAutoConsoleCommand
    UE_LOG(LogTajsGraph, Log, TEXT("TajsGraph Console Commands Unregistered."));
}
