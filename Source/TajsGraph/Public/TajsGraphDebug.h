#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TajsGraphDebug.generated.h"

/**
 * Static library for applying visualization modes and handling debug commands.
 * Accessible from Blueprint/UMG to power the Debug UI.
 */
UCLASS()
class TAJSGRAPH_API UTajsGraphDebug : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // Core visualization modes from original codebase
    UENUM(BlueprintType)
    enum class ETajsGraphVisMode : uint8
    {
        Default,
        BaseColor,
        WorldNormal,
        Roughness,
        Metallic,
        Specular,
        AmbientOcclusion,
        SceneDepth,
        LuminanceHeatmap,
        PseudoWireframe
    };

    /** Activates a specific visualization mode using engine CVars/commands */
    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    static void SetVisualizationMode(const UObject* WorldContextObject, ETajsGraphVisMode Mode);

    /** Dumps all CVars to log/file */
    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    static void DumpCVars(const UObject* WorldContextObject);

    /** Dumps current Surface Cache Traces to log/file */
    UFUNCTION(BlueprintCallable, Category = "TajsGraph|Debug")
    static void DumpSurfaceCacheTrace(const UObject* WorldContextObject, int32 MaxCount = 50);

    // Registration for developer console commands
    static void RegisterConsoleCommands();
    static void UnregisterConsoleCommands();
};
