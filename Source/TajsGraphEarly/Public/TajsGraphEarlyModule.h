#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Early initialization module for TajsGraph.
 *
 * Loaded during PostConfigInit to run diagnostics and attempt cache-based global
 * shader map population before the engine performs global shader verification.
 *
 * Two-phase approach:
 *   Phase 1 (PostConfigInit): Load the base vanilla GlobalShaderCache plus one or
 *     more loose supplemental shader caches from the mod into GGlobalShaderMap[SM6].
 *     This makes the map non-null before CompileGlobalShaderMap runs, so
 *     VerifyGlobalShaders is skipped entirely.
 *   Phase 2 (OnPostEngineInit): Open one or more loose ShaderArchive-*.ushaderbytecode
 *     packs from the mod's Content/Shaders folder as additional named library
 *     components. This can supplement both global shader lookups and project/material
 *     shader lookups (for example FactoryGame material permutations) when the cooked
 *     shader maps already reference those hashes.
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
	 * Currently forced true for validation/debugging and should be gated by config later.
	 */
	bool ShouldOverrideShaderVerification();

	/**
	 * Stub — actual loading happens inside the CompileGlobalShaderMap detour.
	 * Kept to satisfy call-sites on non-Windows platforms.
	 */
	bool TryLoadCookedShaderData();

	/**
	 * Called via FCoreDelegates::OnPostEngineInit after FShaderCodeLibrary::InitForRuntime
	 * has run. Opens the loose TajsGraph material shader library and any supplemental
	 * shader bytecode packs so both global and project/material shader lookups can
	 * resolve hashes from those archives when first dispatched.
	 */
	void OpenShaderLibrariesPostInit();

	/** Handle for the OnPostEngineInit delegate so we can unbind after first call. */
	FDelegateHandle PostEngineInitHandle;

	// Reserved for potential future detour-based fallback path.
	static void* OriginalOpenLibraryPtr;
};
