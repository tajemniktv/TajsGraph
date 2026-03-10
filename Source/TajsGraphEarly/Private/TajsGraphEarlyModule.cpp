#include "TajsGraphEarlyModule.h"
#include "ShaderMapDiagnostics.h"
#include "TajsGraphEarlyShared.h"

#include "Modules/ModuleManager.h"
#include "RenderCore.h"
#include "ShaderCore.h"
#include "DataDrivenShaderPlatformInfo.h"
#include "GlobalShader.h"
#include "ShaderCodeLibrary.h"
#include "ShaderCompiler.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "Serialization/MemoryReader.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/CoreDelegates.h"
#include "Misc/PackageName.h"
#include "UObject/CoreRedirects.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#define LOCTEXT_NAMESPACE "TajsGraphEarly"

DEFINE_LOG_CATEGORY_STATIC(LogTajsGraphEarly, Display, All);

namespace
{
	struct FEngineMaterialOverrideDefinition
	{
		const TCHAR* ConfigKey;
		const TCHAR* EngineSettingKey;
		const TCHAR* SourceObjectPath;
	};

	struct FResolvedEngineMaterialOverride
	{
		const FEngineMaterialOverrideDefinition* Definition = nullptr;
		FString TargetObjectPath;
		FString TargetPackagePath;
	};

	static const TCHAR* const EngineConfigSection = TEXT("/Script/Engine.Engine");

	static const TArray<FEngineMaterialOverrideDefinition>& GetEngineMaterialOverrideDefinitions()
	{
		static const TArray<FEngineMaterialOverrideDefinition> Definitions = {
			{ TajsGraphEarlyShared::GetWorldGridMaterialOverrideKey(), nullptr, TEXT("/Engine/EngineMaterials/WorldGridMaterial.WorldGridMaterial") },
			{ TajsGraphEarlyShared::GetDefaultMaterialOverrideKey(), TEXT("DefaultMaterialName"), TEXT("/Engine/EngineMaterials/DefaultMaterial.DefaultMaterial") },
			{ TajsGraphEarlyShared::GetDefaultDeferredDecalMaterialOverrideKey(), TEXT("DefaultDeferredDecalMaterialName"), TEXT("/Engine/EngineMaterials/DefaultDeferredDecalMaterial.DefaultDeferredDecalMaterial") },
			{ TajsGraphEarlyShared::GetDefaultLightFunctionMaterialOverrideKey(), TEXT("DefaultLightFunctionMaterialName"), TEXT("/Engine/EngineMaterials/DefaultLightFunctionMaterial.DefaultLightFunctionMaterial") },
			{ TajsGraphEarlyShared::GetDefaultPostProcessMaterialOverrideKey(), TEXT("DefaultPostProcessMaterialName"), TEXT("/Engine/EngineMaterials/DefaultPostProcessMaterial.DefaultPostProcessMaterial") },
		};
		return Definitions;
	}

	static TArray<FResolvedEngineMaterialOverride> ResolveConfiguredEngineMaterialOverrides()
	{
		TArray<FResolvedEngineMaterialOverride> ResolvedOverrides;

		bool bEnableOverrides = false;
		if (!TajsGraphEarlyShared::ReadConfigBool(TajsGraphEarlyShared::GetEnableEngineDefaultMaterialOverridesKey(), bEnableOverrides) || !bEnableOverrides)
		{
			return ResolvedOverrides;
		}

		for (const FEngineMaterialOverrideDefinition& Definition : GetEngineMaterialOverrideDefinitions())
		{
			FString RawOverridePath;
			if (!TajsGraphEarlyShared::ReadConfigString(Definition.ConfigKey, RawOverridePath) || RawOverridePath.TrimStartAndEnd().IsEmpty())
			{
				continue;
			}

			FResolvedEngineMaterialOverride ResolvedOverride;
			ResolvedOverride.Definition = &Definition;

			FString Error;
			if (!TajsGraphEarlyShared::NormalizeConfiguredObjectPath(RawOverridePath, ResolvedOverride.TargetObjectPath, ResolvedOverride.TargetPackagePath, Error))
			{
				UE_LOG(LogTajsGraphEarly, Warning,
					TEXT("[Startup][Overrides] Ignoring %s=%s (%s)"),
					Definition.ConfigKey,
					*RawOverridePath,
					Error.IsEmpty() ? TEXT("invalid asset path") : *Error);
				continue;
			}

			ResolvedOverrides.Add(MoveTemp(ResolvedOverride));
		}

		return ResolvedOverrides;
	}

	static void LogConfiguredEngineMaterialOverrides(const TArray<FResolvedEngineMaterialOverride>& ResolvedOverrides)
	{
		if (ResolvedOverrides.IsEmpty())
		{
			UE_LOG(LogTajsGraphEarly, Display, TEXT("[Startup][Overrides] Enabled, but no valid default material overrides were configured."));
			return;
		}

		for (const FResolvedEngineMaterialOverride& ResolvedOverride : ResolvedOverrides)
		{
			UE_LOG(LogTajsGraphEarly, Display,
				TEXT("[Startup][Overrides] %s -> %s"),
				ResolvedOverride.Definition ? ResolvedOverride.Definition->SourceObjectPath : TEXT("<unknown>"),
				*ResolvedOverride.TargetObjectPath);
		}
	}

	static void ApplyConfiguredDefaultMaterialOverrides(const TArray<FResolvedEngineMaterialOverride>& ResolvedOverrides)
	{
		for (const FResolvedEngineMaterialOverride& ResolvedOverride : ResolvedOverrides)
		{
			if (!ResolvedOverride.Definition || ResolvedOverride.Definition->EngineSettingKey == nullptr)
			{
				continue;
			}

			GConfig->SetString(EngineConfigSection, ResolvedOverride.Definition->EngineSettingKey, *ResolvedOverride.TargetObjectPath, GEngineIni);
			UE_LOG(LogTajsGraphEarly, Display,
				TEXT("[Startup][Overrides] Set %s.%s=%s"),
				EngineConfigSection,
				ResolvedOverride.Definition->EngineSettingKey,
				*ResolvedOverride.TargetObjectPath);
		}
	}

	static void ApplyConfiguredEngineMaterialRedirects(const TArray<FResolvedEngineMaterialOverride>& ResolvedOverrides)
	{
		if (ResolvedOverrides.IsEmpty())
		{
			return;
		}

		TArray<FCoreRedirect> Redirects;
		for (const FResolvedEngineMaterialOverride& ResolvedOverride : ResolvedOverrides)
		{
			if (!ResolvedOverride.Definition)
			{
				continue;
			}

			Redirects.Emplace(ECoreRedirectFlags::Type_Object,
				FCoreRedirectObjectName(FString(ResolvedOverride.Definition->SourceObjectPath)),
				FCoreRedirectObjectName(ResolvedOverride.TargetObjectPath));

			Redirects.Emplace(ECoreRedirectFlags::Type_Package,
				FCoreRedirectObjectName(NAME_None, NAME_None, *FPackageName::ObjectPathToPackageName(FString(ResolvedOverride.Definition->SourceObjectPath))),
				FCoreRedirectObjectName(NAME_None, NAME_None, *ResolvedOverride.TargetPackagePath));
		}

		if (!Redirects.IsEmpty())
		{
			FCoreRedirects::AddRedirectList(Redirects, TEXT("TajsGraphEarlyConfiguredEngineMaterialOverride"));
		}
	}
}

namespace ShaderHook
{
	struct FSupplementalShaderPack
	{
		FString LibraryName;
		FString ArchivePath;
		TArray<FString> CacheCandidates;
	};

	static const TCHAR* BoolText(const bool bValue)
	{
		return bValue ? TEXT("true") : TEXT("false");
	}

	static EShaderPlatform ResolveStartupShaderPlatform()
	{
		// In very early load phases, GMaxRHIShaderPlatform may still be uninitialized.
		// Using it directly can assert in DataDrivenShaderPlatformInfo.
		if (GMaxRHIShaderPlatform == SP_NumPlatforms)
		{
			UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Diag] GMaxRHIShaderPlatform is not initialized yet; falling back to SP_PCD3D_SM6 for early shader loading."));
			return SP_PCD3D_SM6;
		}

		return GMaxRHIShaderPlatform;
	}

	static void GatherTajsGraphShaderDirs(TArray<FString>& OutShaderDirs)
	{
		TajsGraphEarlyShared::GatherShaderDirCandidates(OutShaderDirs);
	}

	static TArray<FSupplementalShaderPack> DiscoverSupplementalShaderPacks(const FString& ShaderDir, const FString& PlatformName)
	{
		TArray<FSupplementalShaderPack> Packs;
		if (ShaderDir.IsEmpty() || !IFileManager::Get().DirectoryExists(*ShaderDir))
		{
			return Packs;
		}

		const FString ArchiveSuffix = FString::Printf(TEXT("-%s-%s.ushaderbytecode"), *PlatformName, *PlatformName);
		const FString ArchivePrefix = TEXT("ShaderArchive-");

		TArray<FString> ArchiveFiles;
		IFileManager::Get().FindFiles(ArchiveFiles, *FPaths::Combine(ShaderDir, TEXT("ShaderArchive-*.ushaderbytecode")), true, false);
		ArchiveFiles.Sort();

		for (const FString& ArchiveFile : ArchiveFiles)
		{
			if (!ArchiveFile.StartsWith(ArchivePrefix) || !ArchiveFile.EndsWith(ArchiveSuffix))
			{
				continue;
			}

			const FString LibraryName = ArchiveFile.Mid(ArchivePrefix.Len(), ArchiveFile.Len() - ArchivePrefix.Len() - ArchiveSuffix.Len());
			if (LibraryName.IsEmpty() || LibraryName.Equals(TEXT("Global"), ESearchCase::CaseSensitive))
			{
				continue;
			}

			FSupplementalShaderPack Pack;
			Pack.LibraryName = LibraryName;
			Pack.ArchivePath = FPaths::Combine(ShaderDir, ArchiveFile);
			Pack.CacheCandidates.Add(FPaths::Combine(ShaderDir, FString::Printf(TEXT("GlobalShaderCache-%s-%s.bin"), *LibraryName, *PlatformName)));

			// Backward compatibility for the original single-pack layout.
			if (LibraryName.Equals(TEXT("TajsGraphShaders"), ESearchCase::CaseSensitive))
			{
				Pack.CacheCandidates.Add(FPaths::Combine(ShaderDir, FString::Printf(TEXT("GlobalShaderCache-%s.bin"), *PlatformName)));
			}

			Packs.Add(MoveTemp(Pack));
		}

		return Packs;
	}


	// -----------------------------------------------------------------------
	// Raw x64 detour on CompileGlobalShaderMap.
	//
	// WHY A DETOUR:
	//   The global shader .bin uses bShareCode=true, so LoadFromGlobalArchive
	//   calls FShaderCodeLibrary::LoadResource() for each section.  The library
	//   (Impl ptr) is null before FShaderCodeLibrary::InitForRuntime, so any
	//   attempt to deserialise the .bin at PostConfigInit yields
	//   "Could not load section 0 (of N)" -> Fatal.
	//
	//   InitForRuntime and CompileGlobalShaderMap are back-to-back lines in
	//   FEngineLoop::PreInitPreStartupScreen with no UE delegate between them.
	//   The only way to inject code in that window is a function-body patch.
	//
	// HOW IT WORKS:
	//   1. At PostConfigInit we write a 12-byte absolute JMP into the prolog of
	//      CompileGlobalShaderMap(EShaderPlatform, const ITargetPlatform*, bool)
	//      (all lighter overloads call this one).
	//   2. When the hook fires (after InitForRuntime), we:
	//      a. Restore the original bytes (one-shot, no trampoline needed).
	//      b. Open our loose Global bytecode file (580 MB, contains tessellation
	//         permutations absent from the vanilla 4688-shader Global library).
	//      c. Load our cooked GlobalShaderCache-PCD3D_SM6.bin into
	//         GGlobalShaderMap[SM6].  LoadResource now succeeds because the
	//         library is open.
	//      d. Call the original CompileGlobalShaderMap.  It sees
	//         GGlobalShaderMap[SM6] != null and skips TryLoadCookedGlobalShaderMap
	//         *and* VerifyGlobalShaders entirely.  No missing-permutation crash.
	// -----------------------------------------------------------------------
#if PLATFORM_WINDOWS
	static constexpr int32 PatchSize = 12;  // MOV RAX, imm64 (10) + JMP RAX (2)

	using FCompileGSMFn = void(*)(EShaderPlatform, const ITargetPlatform*, bool);
	static FCompileGSMFn OriginalCompileGSM = nullptr;
	static void*         HookTargetAddr     = nullptr;
	static uint8         OriginalPrologBytes[PatchSize];

	/** Follow an IAT thunk (FF 25 disp32) to the actual function in the engine DLL. */
	static void* ResolveActualAddress(void* Ptr)
	{
		uint8* Code = static_cast<uint8*>(Ptr);
		if (Code[0] == 0xFF && Code[1] == 0x25)  // JMP [RIP + disp32]
		{
			const int32 Disp = *reinterpret_cast<int32*>(Code + 2);
			void** IAT = reinterpret_cast<void**>(Code + 6 + Disp);
			return *IAT;
		}
		return Ptr;  // Not a thunk — already the real function
	}

	static bool WritePatch(void* Target, void* Destination, uint8* OutSavedBytes)
	{
		// 12-byte absolute JMP via RAX:
		//   48 B8 [8-byte addr]   MOV RAX, imm64
		//   FF E0                 JMP RAX
		uint8 Patch[PatchSize];
		Patch[0] = 0x48; Patch[1] = 0xB8;
		*reinterpret_cast<uintptr_t*>(&Patch[2]) = reinterpret_cast<uintptr_t>(Destination);
		Patch[10] = 0xFF; Patch[11] = 0xE0;

		DWORD OldProtect;
		if (!VirtualProtect(Target, PatchSize, PAGE_EXECUTE_READWRITE, &OldProtect))
			return false;

		if (OutSavedBytes)
			FMemory::Memcpy(OutSavedBytes, Target, PatchSize);
		FMemory::Memcpy(Target, Patch, PatchSize);

		VirtualProtect(Target, PatchSize, OldProtect, &OldProtect);
		FlushInstructionCache(GetCurrentProcess(), Target, PatchSize);
		return true;
	}

	static void RestorePatch()
	{
		if (!HookTargetAddr) return;
		DWORD OldProtect;
		VirtualProtect(HookTargetAddr, PatchSize, PAGE_EXECUTE_READWRITE, &OldProtect);
		FMemory::Memcpy(HookTargetAddr, OriginalPrologBytes, PatchSize);
		VirtualProtect(HookTargetAddr, PatchSize, OldProtect, &OldProtect);
		FlushInstructionCache(GetCurrentProcess(), HookTargetAddr, PatchSize);
		HookTargetAddr = nullptr;
	}

	static void HookedCompileGlobalShaderMap(
		EShaderPlatform Platform, const ITargetPlatform* TargetPlatform, bool bRefreshShaderMap)
	{
		// One-shot: restore immediately so the original we call below is clean.
		RestorePatch();

		UE_LOG(LogTajsGraphEarly, Display,
			TEXT("[Hook] CompileGlobalShaderMap(%s) intercepted — InitForRuntime complete, loading tessellation shaders"),
			*LegacyShaderPlatformToShaderFormat(Platform).ToString());

		// InitForRuntime has just finished — FShaderCodeLibrary::IsEnabled() is now true.
		if (FShaderCodeLibrary::IsEnabled())
		{
			TArray<FString> ShaderDirCandidates;
			GatherTajsGraphShaderDirs(ShaderDirCandidates);
			const FString PlatformName = FDataDrivenShaderPlatformInfo::GetName(Platform).ToString();

			// Step 1: Open our loose supplemental Global bytecode files under custom library names.
			//
			// WHY NOT "Global": The vanilla Global library is opened by InitForRuntime via
			// IoStore (ShaderCodeDirectory is empty). FShaderLibraryInstance::HasContentFrom
			// returns true for ANY directory when ShaderCodeDirectory is empty — so calling
			// OpenLibrary("Global", anyDir) is always rejected with "could not find new
			// components".
			//
			// WHY THIS STILL WORKS: FShaderCodeLibrary::LoadResource iterates ALL entries
			// in NamedLibrariesStack regardless of name, so our hashes will be found here.
			// That means these packs can supplement both:
			//   - global shader lookups (when a matching GlobalShaderCache pack is overlaid)
			//   - project/material shader lookups (when cooked shader maps already reference
			//     hashes that exist in these archives)
			//
			// The files it looks for are:
			//   <Dir>/ShaderArchive-<PackName>-PCD3D_SM6-PCD3D_SM6.ushaderbytecode
			//
			// PHYSICAL FILE SYSTEM NOTE:
			//   The game runs with IoStore exclusively. IFileManager only serves files from
			//   mounted IoStore containers and does NOT fall through to the physical disk for
			//   loose files. The .ushaderbytecode (580 MB) cannot be packed into the mod pak,
			//   so it is invisible to IFileManager via the normal platform file chain.
			//   Fix: temporarily lower the active platform file to PhysicalFile for the
			//   duration of OpenLibrary. FShaderCodeArchive::Create opens an IFileCacheHandle
			//   that holds a direct OS file handle — once opened with the physical path it
			//   stays valid after we restore the original platform file chain.
			bool bAnySupplementalLibOpened = false;
			bool bFoundAnySupplementalPack = false;

			// Find the physical platform file (bottom of the platform file chain).
			IPlatformFile* PhysicalFile = FPlatformFileManager::Get().FindPlatformFile(TEXT("PhysicalFile"));
			IPlatformFile* PreviousPlatformFile = nullptr;
			if (PhysicalFile && (PhysicalFile != &FPlatformFileManager::Get().GetPlatformFile()))
			{
				PreviousPlatformFile = &FPlatformFileManager::Get().GetPlatformFile();
				FPlatformFileManager::Get().SetPlatformFile(*PhysicalFile);
				UE_LOG(LogTajsGraphEarly, Display,
					TEXT("[Hook] Temporarily lowered platform file to PhysicalFile to access loose .ushaderbytecode."));
			}
			else if (!PhysicalFile)
			{
				UE_LOG(LogTajsGraphEarly, Warning,
					TEXT("[Hook] Could not find PhysicalFile platform — loose .ushaderbytecode may not be accessible."));
			}

			for (const FString& Dir : ShaderDirCandidates)
			{
				if (Dir.StartsWith(TEXT("/"))) continue;
				if (!IFileManager::Get().DirectoryExists(*Dir)) continue;

				const TArray<FSupplementalShaderPack> Packs = DiscoverSupplementalShaderPacks(Dir, PlatformName);
				if (Packs.IsEmpty())
				{
					continue;
				}

				bFoundAnySupplementalPack = true;
				for (const FSupplementalShaderPack& Pack : Packs)
				{
					const bool bOk = FShaderCodeLibrary::OpenLibrary(Pack.LibraryName, Dir, false);
					UE_LOG(LogTajsGraphEarly, Display,
						TEXT("[Hook] OpenLibrary(%s, %s) = %s"),
						*Pack.LibraryName,
						*Dir,
						bOk ? TEXT("true") : TEXT("false"));

					if (bOk)
					{
						bAnySupplementalLibOpened = true;
					}
				}

				break;
			}

			// Restore platform file chain before any further engine operations.
			if (PreviousPlatformFile)
			{
				FPlatformFileManager::Get().SetPlatformFile(*PreviousPlatformFile);
				UE_LOG(LogTajsGraphEarly, Display, TEXT("[Hook] Platform file chain restored."));
			}

			if (!bFoundAnySupplementalPack)
			{
				UE_LOG(LogTajsGraphEarly, Warning,
					TEXT("[Hook] No supplemental shader archives were found in Content/Shaders — supplemental global/project shader bytecode will be unavailable."));
				UE_LOG(LogTajsGraphEarly, Warning,
					TEXT("[Hook] Expected files like: ShaderArchive-<PackName>-%s-%s.ushaderbytecode"),
					*PlatformName,
					*PlatformName);
			}
			else if (!bAnySupplementalLibOpened)
			{
				UE_LOG(LogTajsGraphEarly, Warning,
					TEXT("[Hook] Supplemental shader archives were found but none opened successfully — LoadResource may still fail for extra global permutations."));
			}

			// Step 2: Build merged GGlobalShaderMap[Platform].
			//
			// We must load the vanilla .bin plus any supplemental .bin files and merge them
			// via successive LoadFromGlobalArchive calls:
			//
			//   a) Vanilla game bin — lives at Engine/GlobalShaderCache-<PlatformName>.bin inside the pak.
			//      — contains FSR3, DLSS, XeSS, Streamline and all other stock permutations.
			//      — exact path TryLoadCookedGlobalShaderMap uses:
			//          FPaths::GetRelativePathToRoot() / "Engine/GlobalShaderCache-<PlatformName>.bin"
			//      — accessible via normal IFileManager (inside IoStore container, not excluded).
			//
			//   b) Supplemental bins in Content/Shaders
			//      — preferred naming: GlobalShaderCache-<PackName>-<PlatformName>.bin
			//      — legacy fallback for TajsGraphShaders: GlobalShaderCache-<PlatformName>.bin
			//      — loaded after vanilla so matching sections replace the stock entries.
			//
			// AddSection uses TMap::Add keyed on HashedSourceFilename — last writer wins per source file.
			// Load vanilla first (FSR3/DLSS/XeSS sections present), then overlay each supplemental
			// pack so only the intended source files replace those sections.

			const FString BinFilename  = FString::Printf(TEXT("GlobalShaderCache-%s.bin"), *PlatformName);

			if (GGlobalShaderMap[Platform] == nullptr)
				GGlobalShaderMap[Platform] = new FGlobalShaderMap(Platform);

			// --- 2a: Load vanilla bin (Engine/ virtual root, same path as TryLoadCookedGlobalShaderMap) ---
			{
				FString VanillaBinPath = FPaths::GetRelativePathToRoot() / TEXT("Engine") / BinFilename;
				FPaths::MakeStandardFilename(VanillaBinPath);
				TArray<uint8> Data;
				if (FFileHelper::LoadFileToArray(Data, *VanillaBinPath, FILEREAD_Silent) && !Data.IsEmpty())
				{
					FMemoryReader Reader(Data);
					GGlobalShaderMap[Platform]->LoadFromGlobalArchive(Reader);
					if (!Reader.IsError())
					{
						UE_LOG(LogTajsGraphEarly, Display,
							TEXT("[Hook] Loaded vanilla GlobalShaderCache-%s (%.2f MB) from %s"),
							*PlatformName, (float)Data.Num() / (1024.f * 1024.f), *VanillaBinPath);
					}
					else
					{
						UE_LOG(LogTajsGraphEarly, Warning,
							TEXT("[Hook] Deserialisation error reading vanilla bin '%s' — FSR3/DLSS/XeSS permutations may be missing."),
							*VanillaBinPath);
					}
				}
				else
				{
					UE_LOG(LogTajsGraphEarly, Warning,
						TEXT("[Hook] Could not read vanilla bin from '%s' — FSR3/DLSS/XeSS permutations may be missing."),
						*VanillaBinPath);
				}
			}

			// --- 2b: Overlay supplemental global caches from mod Content/Shaders ---
			//
			// Not every supplemental pack needs a GlobalShaderCache companion. Packs with only
			// ShaderArchive-*.ushaderbytecode can still supplement FactoryGame/material shader
			// lookups later; only packs that ship a matching cache bin participate in the
			// GGlobalShaderMap overlay used for the early global-shader crash fix.
			int32 LoadedSupplementalBinCount = 0;
			bool bFoundAnySupplementalBin = false;
			for (const FString& Dir : ShaderDirCandidates)
			{
				if (Dir.StartsWith(TEXT("/"))) continue;
				const TArray<FSupplementalShaderPack> Packs = DiscoverSupplementalShaderPacks(Dir, PlatformName);
				if (Packs.IsEmpty())
				{
					continue;
				}

				for (const FSupplementalShaderPack& Pack : Packs)
				{
					bool bLoadedPack = false;
					for (const FString& BinPath : Pack.CacheCandidates)
					{
						if (!FPaths::FileExists(BinPath))
						{
							continue;
						}

						bFoundAnySupplementalBin = true;

						TArray<uint8> Data;
						if (!FFileHelper::LoadFileToArray(Data, *BinPath) || Data.IsEmpty())
						{
							UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Hook] Failed to read %s"), *BinPath);
							continue;
						}

						FMemoryReader Reader(Data);
						GGlobalShaderMap[Platform]->LoadFromGlobalArchive(Reader);

						if (!Reader.IsError())
						{
							UE_LOG(LogTajsGraphEarly, Display,
								TEXT("[Hook] Overlaid supplemental cache '%s' for %s (%.2f MB) from %s"),
								*Pack.LibraryName,
								*PlatformName,
								(float)Data.Num() / (1024.f * 1024.f),
								*BinPath);
							++LoadedSupplementalBinCount;
							bLoadedPack = true;
							break;
						}

						UE_LOG(LogTajsGraphEarly, Warning,
							TEXT("[Hook] Deserialisation error for %s — trying next candidate"), *BinPath);
					}

					if (!bLoadedPack && Pack.CacheCandidates.ContainsByPredicate([](const FString& Candidate) { return FPaths::FileExists(Candidate); }))
					{
						UE_LOG(LogTajsGraphEarly, Warning,
							TEXT("[Hook] Could not find a usable supplemental cache for '%s' in %s"),
							*Pack.LibraryName,
							*Dir);
					}
				}

				break;
			}

			if (LoadedSupplementalBinCount > 0)
			{
				UE_LOG(LogTajsGraphEarly, Display,
					TEXT("[Hook] GGlobalShaderMap[%s] pre-populated (vanilla + %d supplemental pack(s)) — CompileGlobalShaderMap will skip VerifyGlobalShaders."),
					*PlatformName,
					LoadedSupplementalBinCount);
			}
			else
			{
				if (bFoundAnySupplementalBin)
				{
					UE_LOG(LogTajsGraphEarly, Warning,
						TEXT("[Hook] Could not load any supplemental global shader cache bins — the early global-shader overlay will be unavailable."));
				}
				else
				{
					UE_LOG(LogTajsGraphEarly, Warning,
						TEXT("[Hook] No supplemental global shader cache bins were found — global-shader overlay is inactive, but supplemental project/material shader archives may still be available."));
				}
				// Still have vanilla loaded; delete our empty map so CompileGlobalShaderMap
				// falls through to its normal load path (vanilla only, no supplemental crash fix).
				delete GGlobalShaderMap[Platform];
				GGlobalShaderMap[Platform] = nullptr;
			}
		}
		else
		{
			UE_LOG(LogTajsGraphEarly, Warning,
				TEXT("[Hook] FShaderCodeLibrary not enabled — tessellation shaders cannot be loaded. Startup will likely crash."));
		}

		// Call original (now unpatched).  If GGlobalShaderMap[Platform] is non-null it
		// fast-paths past both TryLoadCookedGlobalShaderMap and VerifyGlobalShaders.
		OriginalCompileGSM(Platform, TargetPlatform, bRefreshShaderMap);
	}

	static bool InstallDetour()
	{
		// Take the address of the specific 4-arg overload (the root implementation).
		using TOverload = void(*)(EShaderPlatform, const ITargetPlatform*, bool);
		void* IATThunk = reinterpret_cast<void*>(static_cast<TOverload>(&CompileGlobalShaderMap));
		void* Actual   = ResolveActualAddress(IATThunk);

		if (!Actual)
		{
			UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Hook] Could not resolve CompileGlobalShaderMap address"));
			return false;
		}

		OriginalCompileGSM = reinterpret_cast<FCompileGSMFn>(Actual);
		HookTargetAddr     = Actual;

		if (!WritePatch(Actual, reinterpret_cast<void*>(&HookedCompileGlobalShaderMap), OriginalPrologBytes))
		{
			UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Hook] VirtualProtect failed — cannot patch CompileGlobalShaderMap"));
			HookTargetAddr     = nullptr;
			OriginalCompileGSM = nullptr;
			return false;
		}

		UE_LOG(LogTajsGraphEarly, Display,
			TEXT("[Hook] CompileGlobalShaderMap detour installed at 0x%p"), Actual);
		return true;
	}
#endif // PLATFORM_WINDOWS

	// Dead-code stub for non-Windows (should never ship there but keeps it compiling).
	static bool TryLoadGlobalCacheForPlatform(const FString& /*ShaderDirPath*/, EShaderPlatform /*Platform*/)
	{
		// Loading must happen inside HookedCompileGlobalShaderMap (after InitForRuntime).
		// Calling LoadFromGlobalArchive before that crashes because LoadResource needs
		// FShaderCodeLibrary::Impl (null before InitForRuntime) to resolve bytecode hashes.
		//
		// WHY THIS MATTERS:
		//   TajsGraph enables Nanite tessellation (and other features) via config.
		//   This causes FPatchSplitCS (and other Nanite shaders) to report
		//   ShouldCompilePermutation() == true at runtime.  The vanilla cooked
		//   GlobalShaderCache-PCD3D_SM6.bin was built WITHOUT these features, so
		//   those permutations are absent from it.
		//
		//   CompileGlobalShaderMap() (called in PreInitPreStartupScreen) will:
		//     - if (GGlobalShaderMap[Platform] == nullptr):
		//         create a new map, load vanilla .bin, call VerifyGlobalShaders
		//         → CRASH: "Missing global shader FPatchSplitCS permutation 0"
		//     - if (GGlobalShaderMap[Platform] != nullptr):
		//         skip the entire block above, including VerifyGlobalShaders.
		//
		//   By loading our full .bin (cooked with tessellation) here at
		//   PostConfigInit — before CompileGlobalShaderMap runs — we make
		//   GGlobalShaderMap[SM6] non-null so the crash path is never reached.
		//
		// SHADER CODE LIBRARY:
		//   LoadFromGlobalArchive() only deserialises shader-map metadata and
		//   SHA hash references into memory.  Actual bytecode is resolved lazily
		//   from the open shader code library when a shader is first used (well
		//   after InitForRuntime has opened all libraries).  So this call is safe
		//   before FShaderCodeLibrary::IsEnabled().
		// -----------------------------------------------------------------------

		return false;
	}
}

namespace
{
#if PLATFORM_WINDOWS
	static FString GetModulePathForLog(HMODULE ModuleHandle)
	{
		if (!ModuleHandle)
		{
			return TEXT("<not loaded>");
		}

		TCHAR Buffer[4096] = { 0 };
		const DWORD PathLen = ::GetModuleFileNameW(ModuleHandle, Buffer, UE_ARRAY_COUNT(Buffer));
		if (PathLen == 0)
		{
			return TEXT("<path unavailable>");
		}

		return FString(Buffer);
	}

	static bool TryReadExportedUint(HMODULE ModuleHandle, const ANSICHAR* SymbolName, uint32& OutValue)
	{
		if (!ModuleHandle)
		{
			return false;
		}

		const FARPROC SymbolPtr = ::GetProcAddress(ModuleHandle, SymbolName);
		if (!SymbolPtr)
		{
			return false;
		}

		OutValue = *reinterpret_cast<const uint32*>(SymbolPtr);
		return true;
	}

	static bool TryReadExportedUtf8Path(HMODULE ModuleHandle, const ANSICHAR* SymbolName, FString& OutPath)
	{
		if (!ModuleHandle)
		{
			return false;
		}

		const FARPROC SymbolPtr = ::GetProcAddress(ModuleHandle, SymbolName);
		if (!SymbolPtr)
		{
			return false;
		}

		const ANSICHAR* const* StringPtr = reinterpret_cast<const ANSICHAR* const*>(SymbolPtr);
		if (!StringPtr || !(*StringPtr))
		{
			return false;
		}

		OutPath = UTF8_TO_TCHAR(*StringPtr);
		return true;
	}
#endif
}

void FTajsGraphEarlyModule::LogD3D12Diagnostics(const TCHAR* PhaseLabel)
{
#if PLATFORM_WINDOWS
	const HMODULE ExeModule = ::GetModuleHandleW(nullptr);
	const HMODULE D3D12LoaderModule = ::GetModuleHandleW(TEXT("d3d12.dll"));
	const HMODULE D3D12CoreModule = ::GetModuleHandleW(TEXT("D3D12Core.dll"));

	const FString ExePath = GetModulePathForLog(ExeModule);
	const FString LoaderPath = GetModulePathForLog(D3D12LoaderModule);
	const FString CorePath = GetModulePathForLog(D3D12CoreModule);

	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag][D3D12][%s] ExePath=%s"), PhaseLabel, *ExePath);
	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag][D3D12][%s] d3d12.dll=%s"), PhaseLabel, *LoaderPath);
	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag][D3D12][%s] D3D12Core.dll=%s"), PhaseLabel, *CorePath);

	if (CorePath.Contains(TEXT("\\System32\\"), ESearchCase::IgnoreCase))
	{
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag][D3D12][%s] D3D12Core appears to be loaded from System32 (OS-selected runtime)."), PhaseLabel);
	}

	uint32 RequestedSdkVersion = 0;
	if (TryReadExportedUint(ExeModule, "D3D12SDKVersion", RequestedSdkVersion))
	{
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag][D3D12][%s] EXE export D3D12SDKVersion=%u"), PhaseLabel, RequestedSdkVersion);
	}
	else
	{
		UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Diag][D3D12][%s] EXE export D3D12SDKVersion not found."), PhaseLabel);
	}

	FString RequestedSdkPath;
	if (TryReadExportedUtf8Path(ExeModule, "D3D12SDKPath", RequestedSdkPath))
	{
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag][D3D12][%s] EXE export D3D12SDKPath=%s"), PhaseLabel, *RequestedSdkPath);
	}
	else
	{
		UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Diag][D3D12][%s] EXE export D3D12SDKPath not found."), PhaseLabel);
	}

	uint32 LoadedCoreSdkVersion = 0;
	if (TryReadExportedUint(D3D12CoreModule, "D3D12SDKVersion", LoadedCoreSdkVersion))
	{
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag][D3D12][%s] Loaded D3D12Core export D3D12SDKVersion=%u"), PhaseLabel, LoadedCoreSdkVersion);

		if (RequestedSdkVersion != 0 && RequestedSdkVersion != LoadedCoreSdkVersion)
		{
			UE_LOG(LogTajsGraphEarly, Warning,
				TEXT("[Diag][D3D12][%s] Requested SDK version (%u) differs from loaded D3D12Core (%u)."),
				PhaseLabel,
				RequestedSdkVersion,
				LoadedCoreSdkVersion);
		}
	}
	else
	{
		UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Diag][D3D12][%s] Loaded D3D12Core export D3D12SDKVersion not found (or D3D12Core not loaded yet)."), PhaseLabel);
	}
#else
	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag][D3D12][%s] Unsupported platform for Windows D3D12 diagnostics."), PhaseLabel);
#endif
}

void FTajsGraphEarlyModule::StartupModule()
{
	// Keep the editor path clean: this module exists to patch runtime shader startup.
	// In Unreal Editor we want zero hooks, zero library registration, and zero side effects.
#if WITH_EDITOR
	if (GIsEditor)
	{
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Startup] Unreal Editor detected; skipping TajsGraphEarly runtime initialization."));
		return;
	}
#endif

	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Startup] === TajsGraphEarly (PostConfigInit) initialized ==="));

	const TArray<FResolvedEngineMaterialOverride> ResolvedOverrides = ResolveConfiguredEngineMaterialOverrides();

	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Startup][Phase 1/5] Applying configured engine material overrides."));
	LogConfiguredEngineMaterialOverrides(ResolvedOverrides);
	ApplyConfiguredEngineMaterialRedirects(ResolvedOverrides);
	ApplyConfiguredDefaultMaterialOverrides(ResolvedOverrides);

	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Startup][Phase 2/5] Emitting early shader diagnostics."));
	LogShaderDiagnostics();
	LogD3D12Diagnostics(TEXT("PostConfigInit"));

	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Startup][Phase 3/5] Installing material shader-map diagnostics."));
	ShaderMapDiagnostics::Startup();

	// Install a one-shot detour on CompileGlobalShaderMap.
	//
	// TIMING EXPLAINED:
	//   - PostConfigInit (now): FShaderCodeLibrary::Impl == null → cannot call
	//     LoadFromGlobalArchive (each section calls LoadResource which needs Impl).
	//   - FShaderCodeLibrary::InitForRuntime: opens Global + plugin libraries.
	//   - CompileGlobalShaderMap: loads vanilla .bin → VerifyGlobalShaders → CRASH
	//     because r.Nanite.AllowTessellation=1 (our config) makes FPatchSplitCS
	//     expected but the vanilla .bin (cooked without tessellation) lacks it.
	//
	//   There is no UE delegate between InitForRuntime and CompileGlobalShaderMap.
	//   The detour fires in that exact window: library open, verify not yet run.
#if PLATFORM_WINDOWS
	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Startup][Phase 4/5] Installing global shader detour."));
	if (ShouldOverrideShaderVerification())
	{
		if (ShaderHook::InstallDetour())
		{
			UE_LOG(LogTajsGraphEarly, Display,
				TEXT("[Startup] CompileGlobalShaderMap detour installed — shaders will load just before VerifyGlobalShaders."));
		}
		else
		{
			UE_LOG(LogTajsGraphEarly, Warning,
				TEXT("[Startup] Detour installation failed — startup will crash at VerifyGlobalShaders."));
		}
	}
#else
	UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Startup] Raw detour not implemented for this platform — shaders startup fix inactive."));
#endif

	// Belt-and-suspenders: also open libraries after full engine init in case
	// the detour succeeded for the .bin but the bytecode library needs reopening.
	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Startup][Phase 5/5] Scheduling supplemental shader library registration."));
	PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FTajsGraphEarlyModule::OpenShaderLibrariesPostInit);

	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Startup] === TajsGraphEarly initialization complete ==="));
}

void FTajsGraphEarlyModule::ShutdownModule()
{
	// Safety: if the hook never fired (e.g. game shut down before CompileGlobalShaderMap),
	// restore original bytes so we don't leave a dangling JMP in released memory.
#if PLATFORM_WINDOWS
	ShaderHook::RestorePatch();
#endif

	if (PostEngineInitHandle.IsValid())
	{
		FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
		PostEngineInitHandle.Reset();
	}
	ShaderMapDiagnostics::Shutdown();
	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Shutdown] Module shutting down."));
}

void FTajsGraphEarlyModule::OpenShaderLibrariesPostInit()
{
	// Unbind immediately — this is a one-shot callback.
	FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
	PostEngineInitHandle.Reset();

	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Library] OnPostEngineInit — opening shader bytecode libraries."));
	LogD3D12Diagnostics(TEXT("OnPostEngineInit"));

	if (!FShaderCodeLibrary::IsEnabled())
	{
		UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Library] FShaderCodeLibrary still not enabled at OnPostEngineInit — bytecode for tessellation permutations cannot be registered. Rendering may fail."));
		return;
	}

	// Open the TajsGraph library (mod's own material shaders).
	if (TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("TajsGraph")))
	{
		const FString ContentDir = Plugin->GetContentDir();
		const bool bTG = FShaderCodeLibrary::OpenLibrary(TEXT("TajsGraph"), ContentDir, false);
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Library] OpenLibrary(TajsGraph, %s) = %s"), *ContentDir, bTG ? TEXT("true") : TEXT("false"));
		if (!bTG)
		{
			UE_LOG(LogTajsGraphEarly, Display,
				TEXT("[Library] TajsGraph material shader library already appears to be registered; treating OpenLibrary=false as non-fatal."));
		}

		// Open every supplemental shader bytecode library in Content/Shaders.
		// We avoid the reserved "Global" name because the vanilla Global library is
		// IoStore-based and OpenLibrary("Global", anyDir) is always rejected by HasContentFrom.
		// LoadResource searches ALL named libraries, so any discovered pack name works for
		// both extra global bytecode and supplemental FactoryGame/material bytecode.
		const FString ShadersDir = FPaths::Combine(ContentDir, TEXT("Shaders"));
		if (IFileManager::Get().DirectoryExists(*ShadersDir))
		{
			const FString PlatformName = FDataDrivenShaderPlatformInfo::GetName(ShaderHook::ResolveStartupShaderPlatform()).ToString();
			const TArray<ShaderHook::FSupplementalShaderPack> Packs = ShaderHook::DiscoverSupplementalShaderPacks(ShadersDir, PlatformName);
			if (Packs.IsEmpty())
			{
				UE_LOG(LogTajsGraphEarly, Warning,
					TEXT("[Library] No supplemental shader archives found in %s for platform %s"),
					*ShadersDir,
					*PlatformName);
			}
			else
			{
				for (const ShaderHook::FSupplementalShaderPack& Pack : Packs)
				{
					const bool bOpened = FShaderCodeLibrary::OpenLibrary(Pack.LibraryName, ShadersDir, false);
					UE_LOG(LogTajsGraphEarly, Display, TEXT("[Library] OpenLibrary(%s, %s) = %s"), *Pack.LibraryName, *ShadersDir, bOpened ? TEXT("true") : TEXT("false"));
					if (!bOpened)
					{
						if (FPaths::FileExists(Pack.ArchivePath))
						{
							UE_LOG(LogTajsGraphEarly, Display,
								TEXT("[Library] %s archive is present; OpenLibrary=false likely means it was already opened earlier or had no new components."),
								*Pack.LibraryName);
						}
						else
						{
							UE_LOG(LogTajsGraphEarly, Warning,
								TEXT("[Library] Could not open supplemental shader bytecode library '%s'."), *Pack.LibraryName);
							UE_LOG(LogTajsGraphEarly, Warning,
								TEXT("[Library] Ensure %s is present in: %s"),
								*FPaths::GetCleanFilename(Pack.ArchivePath),
								*ShadersDir);
						}
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Library] Shaders directory not found: %s"), *ShadersDir);
		}
	}
	else
	{
		UE_LOG(LogTajsGraphEarly, Warning, TEXT("[Library] TajsGraph plugin not found via IPluginManager — cannot open shader libraries."));
	}

	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Library] Shader library registration complete."));
}

void FTajsGraphEarlyModule::LogShaderDiagnostics()
{
	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag] --- Shader diagnostics begin ---"));

	// LegacyShaderPlatformToShaderFormat internally calls FDataDrivenShaderPlatformInfo::GetName
	// which asserts IsValid(Platform).  At PostConfigInit in the editor DDSPII may not be
	// initialised yet, so we guard the call.
	EShaderPlatform Platform = ShaderHook::ResolveStartupShaderPlatform();
	if (FDataDrivenShaderPlatformInfo::IsValid(Platform))
	{
		FString PlatformName = LegacyShaderPlatformToShaderFormat(Platform).ToString();
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag] Current Shader Platform (startup/provisional): %s"), *PlatformName);
	}
	else
	{
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag] Current Shader Platform: uninitialized (DDSPII not ready yet — expected in editor at PostConfigInit)"));
	}

	// Log feature level (may not be initialized this early)
	if (GMaxRHIFeatureLevel != ERHIFeatureLevel::Num)
	{
		FString FeatureLevelStr;
		GetFeatureLevelName(GMaxRHIFeatureLevel, FeatureLevelStr);
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag] Feature Level: %s"), *FeatureLevelStr);
	}
	else
	{
		UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag] Feature Level: Uninitialized (expected during early startup)."));
	}

	// Log shader library status
	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag] Shader library status at this phase: %s"), ShaderHook::BoolText(FShaderCodeLibrary::IsEnabled()));
	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag] If required shaders are missing, this module attempts cache-based global map population."));

	UE_LOG(LogTajsGraphEarly, Display, TEXT("[Diag] --- Shader diagnostics end ---"));
}

bool FTajsGraphEarlyModule::ShouldOverrideShaderVerification()
{
	// The detour is only needed in game runtime builds (Shipping/Development game).
	// In the editor CompileGlobalShaderMap behaves differently (recompiles missing
	// shaders on demand) and patching its prolog is unnecessary and unsafe.
	if (GIsEditor)
	{
		return false;
	}
	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTajsGraphEarlyModule, TajsGraphEarly)
