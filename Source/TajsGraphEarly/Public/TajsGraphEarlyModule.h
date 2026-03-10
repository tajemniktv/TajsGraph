#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Early initialization module for TajsGraph.
 *
 * Loaded during PostConfigInit to orchestrate three early-startup behaviors:
 *   - supplemental shader-library/global-cache loading
 *   - configured default engine material overrides
 *   - optional material shader-map diagnostics
 *
 * The module keeps startup ordering explicit:
 *   1. apply configured engine material overrides
 *   2. install diagnostics hooks if enabled
 *   3. install the one-shot global shader detour (runtime only)
 *   4. open supplemental shader bytecode libraries after engine init
 */
class FTajsGraphEarlyModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Emits startup diagnostics for platform/feature/library state. */
	void LogShaderDiagnostics();

	/** Emits D3D12 Agility runtime diagnostics (requested exports, loaded modules, and paths). */
	void LogD3D12Diagnostics(const TCHAR* PhaseLabel);

	/**
	 * Returns whether early cache-load behavior should run.
	 * This stays disabled in editor builds, where the runtime startup detour is not needed.
	 */
	bool ShouldOverrideShaderVerification();

	/**
	 * Called via FCoreDelegates::OnPostEngineInit after FShaderCodeLibrary::InitForRuntime
	 * has run. Opens the loose TajsGraph material shader library and any supplemental
	 * shader bytecode packs so both global and project/material shader lookups can
	 * resolve hashes from those archives.
	 */
	void OpenShaderLibrariesPostInit();

	/** Handle for the OnPostEngineInit delegate so we can unbind after first call. */
	FDelegateHandle PostEngineInitHandle;
};
