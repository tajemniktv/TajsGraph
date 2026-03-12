#include "ShaderMapDiagnostics.h"
#include "TajsGraphEarlyShared.h"

#include "MaterialShared.h"
#include "MaterialShaderType.h"
#include "MeshMaterialShaderType.h"
#include "Patching/NativeHookManager.h"
#include "Shader.h"
#include "ShaderCore.h"
#include "VertexFactory.h"
#include "HAL/FileManager.h"
#include "Interfaces/IPluginManager.h"
#include "Logging/LogVerbosity.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"
#include "Misc/OutputDevice.h"
#include "Misc/Paths.h"
#include "Misc/ScopeLock.h"
#include "Misc/SecureHash.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

DEFINE_LOG_CATEGORY_STATIC(LogTajsGraphEarlyDiag, Display, All);

namespace ShaderMapDiagnostics
{
	namespace
	{
		const TCHAR* const DefaultWorldGridPath = TEXT("/Engine/EngineMaterials/WorldGridMaterial");
		const TCHAR* const DefaultPostProcessPath = TEXT("/Engine/EngineMaterials/DefaultPostProcessMaterial");
		const TCHAR* const ReplacementDefaultPostProcessPath = TEXT("/TajsGraph/Engine/EngineMaterials/DefaultPostProcessMaterial");

		using FCacheShadersSignature = bool(FMaterial::*)(EShaderPlatform, EMaterialShaderPrecompileMode, const ITargetPlatform*);
		using FCacheShadersWithIdSignature = bool(FMaterial::*)(const FMaterialShaderMapId&, EShaderPlatform, EMaterialShaderPrecompileMode, const ITargetPlatform*);
		using FTryGetShadersSignature = bool(FMaterial::*)(const FMaterialShaderTypes&, const FVertexFactoryType*, FMaterialShaders&) const;
		using FGetShaderPipelineSignature = FShaderPipelineRef(FMaterial::*)(FShaderPipelineType*, FVertexFactoryType*, bool) const;

		struct FRuntimeMaterialContext
		{
			FString MaterialName;
			FString AssetPath;
			FString Platform;
			FString RuntimeHash;
			FString ExpectedHash;
			FString MatchState;
		};

		class FShaderDiagnosticOutputDevice final : public FOutputDevice
		{
		public:
			bool Startup()
			{
				if (!LoadConfig())
				{
					return false;
				}

				if (!bEnabled)
				{
					return false;
				}

				LoadAssetInfoIndex();
				InstallHooks();

				if (GLog)
				{
					GLog->AddOutputDevice(this);
					bRegistered = true;
				}

				LogStartupSummary();
				return true;
			}

			void Shutdown()
			{
				if (bRegistered && GLog)
				{
					GLog->RemoveOutputDevice(this);
				}
				bRegistered = false;
				UninstallHooks();

				FScopeLock Lock(&StateMutex);
				RecentMissingShaderResourceHashes.Reset();
				RecentMaterialNames.Reset();
				RecentAssetPathByMaterialName.Reset();
				ExpectedHashByAssetPath.Reset();
				AssetPathsByHash.Reset();
				RuntimeContextByHash.Reset();
				EmittedMessageKeys.Reset();
			}

			bool IsEnabled() const
			{
				return bEnabled;
			}

			virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override
			{
				if (!bEnabled || V == nullptr)
				{
					return;
				}

				if (Category == TEXT("LogTajsGraphEarly"))
				{
					return;
				}

				if (bInSerialize)
				{
					return;
				}

				TGuardValue<bool> ReentryGuard(bInSerialize, true);

				const FString Message(V);
				if (Category == TEXT("LogShaders"))
				{
					HandleShaderLog(Message);
				}
				else if (Category == TEXT("LogMaterial"))
				{
					HandleMaterialLog(Message, Verbosity);
				}
			}

		private:
			static inline thread_local bool bInSerialize = false;

			bool bEnabled = false;
			bool bEnableHashRemap = true;
			bool bRegistered = false;
			mutable FCriticalSection StateMutex;
			TMap<FString, FString> ExpectedHashByAssetPath;
			TMap<FString, TArray<FString>> AssetPathsByHash;
			TMap<FString, FString> RecentAssetPathByMaterialName;
			TArray<FString> RecentMissingShaderResourceHashes;
			TArray<FString> RecentMaterialNames;
			TMap<FString, FRuntimeMaterialContext> RuntimeContextByHash;
			TSet<FString> EmittedMessageKeys;
			FDelegateHandle CacheShadersHandle;
			FDelegateHandle CacheShadersWithIdHandle;
			FDelegateHandle TryGetShadersHandle;
			FDelegateHandle GetShaderPipelineHandle;

			bool LoadConfig()
			{
				bEnabled = false;
				bEnableHashRemap = true;

				TArray<FString> ConfigCandidates;
				TajsGraphEarlyShared::GatherConfigCandidatePaths(ConfigCandidates);

				for (const FString& ConfigPath : ConfigCandidates)
				{
					if (ConfigPath.IsEmpty() || !FPaths::FileExists(ConfigPath))
					{
						continue;
					}

					bool bValue = false;
					if (GConfig->GetBool(TajsGraphEarlyShared::GetConfigSection(), TajsGraphEarlyShared::GetEnableMaterialShaderMapDiagKey(), bValue, ConfigPath))
					{
						bEnabled = bValue;
					}

					bool bHashRemapValue = bEnableHashRemap;
					if (GConfig->GetBool(TajsGraphEarlyShared::GetConfigSection(), TajsGraphEarlyShared::GetEnableMaterialShaderMapHashRemapKey(), bHashRemapValue, ConfigPath))
					{
						bEnableHashRemap = bHashRemapValue;
					}
				}

				return true;
			}

			void LoadAssetInfoIndex()
			{
				const FString PlatformName = TEXT("PCD3D_SM6");
				TArray<TajsGraphEarlyShared::FAssetInfoSearchDir> SearchDirs;
				TajsGraphEarlyShared::GatherAssetInfoSearchDirs(PlatformName, SearchDirs);

				bool bLoadedAnyAssetInfo = false;
				for (const FString& LibraryName : { FString(TEXT("FactoryExtra")), FString(TEXT("TajsGraph")) })
				{
					const FString AssetInfoFilename = FString::Printf(TEXT("ShaderAssetInfo-%s-%s-%s.assetinfo.json"), *LibraryName, *PlatformName, *PlatformName);
					for (const TajsGraphEarlyShared::FAssetInfoSearchDir& SearchDir : SearchDirs)
					{
						const FString AssetInfoPath = FPaths::Combine(SearchDir.Directory, AssetInfoFilename);
						if (LoadAssetInfoFile(AssetInfoPath, LibraryName, SearchDir.SourceKind))
						{
							bLoadedAnyAssetInfo = true;
							break;
						}
					}
				}

				if (!bLoadedAnyAssetInfo)
				{
					UE_LOG(LogTajsGraphEarlyDiag, Warning, TEXT("[ShaderDiag][Startup] No assetinfo index files were found for FactoryExtra or TajsGraph; expected hash comparison will be unavailable."));
				}
			}

			void LogStartupSummary() const
			{
				const FString* WorldGridHash = ExpectedHashByAssetPath.Find(DefaultWorldGridPath);
				const FString* DefaultPostProcessHash = ExpectedHashByAssetPath.Find(DefaultPostProcessPath);
				const FString* ReplacementPostProcessHash = ExpectedHashByAssetPath.Find(ReplacementDefaultPostProcessPath);

				UE_LOG(LogTajsGraphEarlyDiag, Display,
					TEXT("[ShaderDiag][Startup] Enabled=true IndexedAssets=%d IndexedHashes=%d HashRemapMode=%s WorldGridHash=%s EngineDefaultPostProcessHash=%s TajsGraphDefaultPostProcessHash=%s"),
					ExpectedHashByAssetPath.Num(),
					AssetPathsByHash.Num(),
					bEnableHashRemap ? TEXT("diagnostic_only") : TEXT("disabled"),
					WorldGridHash ? **WorldGridHash : TEXT("<missing>"),
					DefaultPostProcessHash ? **DefaultPostProcessHash : TEXT("<missing>"),
					ReplacementPostProcessHash ? **ReplacementPostProcessHash : TEXT("<missing>"));
			}

			void InstallHooks()
			{
				if (CacheShadersHandle.IsValid())
				{
					return;
				}

				CacheShadersHandle = SUBSCRIBE_METHOD_EXPLICIT(FCacheShadersSignature, FMaterial::CacheShaders,
					[this](auto& Scope, FMaterial* Material, EShaderPlatform Platform, EMaterialShaderPrecompileMode PrecompileMode, const ITargetPlatform* TargetPlatform)
					{
						Scope(Material, Platform, PrecompileMode, TargetPlatform);
						OnCacheShaders(Material, Platform);
					});

				CacheShadersWithIdHandle = SUBSCRIBE_METHOD_EXPLICIT(FCacheShadersWithIdSignature, FMaterial::CacheShaders,
					[this](auto& Scope, FMaterial* Material, const FMaterialShaderMapId& ShaderMapId, EShaderPlatform Platform, EMaterialShaderPrecompileMode PrecompileMode, const ITargetPlatform* TargetPlatform)
					{
						Scope(Material, ShaderMapId, Platform, PrecompileMode, TargetPlatform);
						OnCacheShaders(Material, Platform);
					});

				TryGetShadersHandle = SUBSCRIBE_METHOD_EXPLICIT(FTryGetShadersSignature, FMaterial::TryGetShaders,
					[this](auto& Scope, const FMaterial* Material, const FMaterialShaderTypes& ShaderTypes, const FVertexFactoryType* VertexFactoryType, FMaterialShaders& OutShaders)
					{
						Scope(Material, ShaderTypes, VertexFactoryType, OutShaders);
						OnTryGetShaders(Scope, Material, ShaderTypes, VertexFactoryType, OutShaders);
					});

				GetShaderPipelineHandle = SUBSCRIBE_METHOD_EXPLICIT(FGetShaderPipelineSignature, FMaterial::GetShaderPipeline,
					[this](auto& Scope, const FMaterial* Material, FShaderPipelineType* ShaderPipelineType, FVertexFactoryType* VertexFactoryType, bool bFatalIfNotFound)
					{
						OnShaderPipelineLookup(Scope, Material, ShaderPipelineType, VertexFactoryType, bFatalIfNotFound);
						if (Scope.ShouldForwardCall())
						{
							Scope(Material, ShaderPipelineType, VertexFactoryType, bFatalIfNotFound);
						}
					});

				UE_LOG(LogTajsGraphEarlyDiag, Display, TEXT("[ShaderDiag][Startup] Installed native material/shader-map hooks."));
			}

			void UninstallHooks()
			{
				if (CacheShadersHandle.IsValid())
				{
					UNSUBSCRIBE_METHOD_EXPLICIT(FCacheShadersSignature, FMaterial::CacheShaders, CacheShadersHandle);
					CacheShadersHandle.Reset();
				}

				if (CacheShadersWithIdHandle.IsValid())
				{
					UNSUBSCRIBE_METHOD_EXPLICIT(FCacheShadersWithIdSignature, FMaterial::CacheShaders, CacheShadersWithIdHandle);
					CacheShadersWithIdHandle.Reset();
				}

				if (TryGetShadersHandle.IsValid())
				{
					UNSUBSCRIBE_METHOD_EXPLICIT(FTryGetShadersSignature, FMaterial::TryGetShaders, TryGetShadersHandle);
					TryGetShadersHandle.Reset();
				}

				if (GetShaderPipelineHandle.IsValid())
				{
					UNSUBSCRIBE_METHOD_EXPLICIT(FGetShaderPipelineSignature, FMaterial::GetShaderPipeline, GetShaderPipelineHandle);
					GetShaderPipelineHandle.Reset();
				}

			}

			void OnCacheShaders(FMaterial* Material, EShaderPlatform Platform)
			{
				if (!Material)
				{
					return;
				}

				const FMaterialShaderMap* ShaderMap = Material->GetGameThreadShaderMap();
				if (!ShaderMap)
				{
					return;
				}

				const FRuntimeMaterialContext Context = BuildMaterialContext(Material, ShaderMap, Platform);
				if (Context.RuntimeHash.IsEmpty())
				{
					return;
				}

				{
					FScopeLock Lock(&StateMutex);
					RuntimeContextByHash.Add(Context.RuntimeHash, Context);
					if (!Context.MaterialName.IsEmpty() && !Context.AssetPath.IsEmpty())
					{
						RecentAssetPathByMaterialName.Add(Context.MaterialName, Context.AssetPath);
					}
				}

				if (!ShouldLogRuntimeHash(Context))
				{
					return;
				}

				const FString Key = FString::Printf(TEXT("RuntimeHash|%s|%s|%s"), *Context.AssetPath, *Context.Platform, *Context.RuntimeHash);
				if (!MarkEmitted(Key))
				{
					return;
				}

				UE_LOG(LogTajsGraphEarlyDiag, Display,
					TEXT("[ShaderDiag][RuntimeHash] Material=%s AssetPath=%s Platform=%s RuntimeHash=%s ExpectedIndexedHash=%s Match=%s"),
					Context.MaterialName.IsEmpty() ? TEXT("<unknown>") : *Context.MaterialName,
					Context.AssetPath.IsEmpty() ? TEXT("<unknown>") : *Context.AssetPath,
					Context.Platform.IsEmpty() ? TEXT("<unknown>") : *Context.Platform,
					*Context.RuntimeHash,
					Context.ExpectedHash.IsEmpty() ? TEXT("<missing>") : *Context.ExpectedHash,
					*Context.MatchState);
			}

			void OnTryGetShaders(TCallScope<bool(*)(const FMaterial*, const FMaterialShaderTypes&, const FVertexFactoryType*, FMaterialShaders&)>& Scope, const FMaterial* Material, const FMaterialShaderTypes& ShaderTypes, const FVertexFactoryType* VertexFactoryType, FMaterialShaders& OutShaders)
			{
				if (!Material || Scope.GetResult())
				{
					return;
				}

				const FMaterialShaderMap* ShaderMap = ResolveShaderMapForCurrentThread(Material);
				if (!ShaderMap)
				{
					return;
				}

				const FRuntimeMaterialContext Context = BuildMaterialContext(Material, ShaderMap, ShaderMap->GetShaderPlatform());
				EmitLookupMiss(
					Context,
					JoinRequestedShaderTypeNames(ShaderTypes),
					ShaderTypes.PipelineType ? ShaderTypes.PipelineType->GetName() : TEXT("<none>"),
					VertexFactoryType ? VertexFactoryType->GetName() : TEXT("<none>"),
					ExtractPrimaryPermutationId(ShaderTypes),
					ShaderTypes.PipelineType ? TEXT("Pipeline") : TEXT("NonPipeline"),
					TEXT("TryGetShaders"));

				if (TryRecoverShadersFromExpectedHash(Context, Material, ShaderMap, ShaderTypes, VertexFactoryType, OutShaders))
				{
					Scope.Override(true);
				}
			}

			void OnShaderPipelineLookup(TCallScope<FShaderPipelineRef(*)(const FMaterial*, FShaderPipelineType*, FVertexFactoryType*, bool)>& Scope, const FMaterial* Material, FShaderPipelineType* ShaderPipelineType, FVertexFactoryType* VertexFactoryType, bool bFatalIfNotFound)
			{
				if (!Material || !ShaderPipelineType || !VertexFactoryType || Scope.GetResult().IsValid())
				{
					return;
				}

				const FMaterialShaderMap* ShaderMap = Material->GetRenderingThreadShaderMap();
				if (!ShaderMap)
				{
					return;
				}

				const FMeshMaterialShaderMap* MeshShaderMap = ShaderMap->GetMeshShaderMap(VertexFactoryType);
				const bool bFoundPipeline = MeshShaderMap && MeshShaderMap->GetShaderPipeline(ShaderPipelineType) != nullptr;
				if (bFoundPipeline)
				{
					return;
				}

				const FRuntimeMaterialContext Context = BuildMaterialContext(Material, ShaderMap, ShaderMap->GetShaderPlatform());
				EmitLookupMiss(Context, TEXT("<none>"), ShaderPipelineType->GetName(), VertexFactoryType->GetName(), 0, TEXT("Pipeline"), bFatalIfNotFound ? TEXT("FatalPath") : TEXT("NonFatalPath"));

				FShaderPipelineRef RemappedPipeline;
				if (TryRecoverPipelineFromExpectedHash(Context, Material, ShaderMap, ShaderPipelineType, VertexFactoryType, RemappedPipeline))
				{
					Scope.Override(RemappedPipeline);
				}
			}

			FRuntimeMaterialContext BuildMaterialContext(const FMaterial* Material, const FMaterialShaderMap* ShaderMap, EShaderPlatform Platform) const
			{
				FRuntimeMaterialContext Context;
				Context.MaterialName = Material ? Material->GetAssetName() : FString();
				if (Material)
				{
					Context.AssetPath = TajsGraphEarlyShared::NormalizeObjectPath(Material->GetFullPath());
				}
				if (Context.AssetPath.IsEmpty())
				{
					Context.AssetPath = ResolveAssetPathForMaterial(Context.MaterialName);
				}
				Context.Platform = LegacyShaderPlatformToShaderFormat(Platform).ToString();
				Context.RuntimeHash = ShaderMap ? ShaderMap->GetShaderContentHash().ToString() : FString();
				Context.ExpectedHash = GetExpectedHashForAssetPath(Context.AssetPath);
				Context.MatchState = CalculateMatchState(Context.RuntimeHash, Context.ExpectedHash);
				return Context;
			}

			FRuntimeMaterialContext ResolveRuntimeContextByHash(const FString& RuntimeHash, EShaderPlatform Platform) const
			{
				FScopeLock Lock(&StateMutex);
				if (const FRuntimeMaterialContext* Existing = RuntimeContextByHash.Find(RuntimeHash))
				{
					return *Existing;
				}

				FRuntimeMaterialContext Context;
				Context.RuntimeHash = RuntimeHash;
				Context.Platform = LegacyShaderPlatformToShaderFormat(Platform).ToString();
				Context.MatchState = TEXT("missing_expected_indexed_hash");
				return Context;
			}

			static const FMaterialShaderMap* ResolveShaderMapForCurrentThread(const FMaterial* Material)
			{
				if (!Material)
				{
					return nullptr;
				}

				if (IsInGameThread() || IsInParallelGameThread() || IsInAsyncLoadingThread())
				{
					return Material->GetGameThreadShaderMap();
				}

				return Material->GetRenderingThreadShaderMap();
			}

			bool ShouldLogRuntimeHash(const FRuntimeMaterialContext& Context) const
			{
				return Context.AssetPath == DefaultWorldGridPath
					|| Context.AssetPath == DefaultPostProcessPath
					|| Context.AssetPath == ReplacementDefaultPostProcessPath
					|| Context.MaterialName == TEXT("WorldGridMaterial")
					|| Context.MaterialName == TEXT("DefaultPostProcessMaterial");
			}

			static FString CalculateMatchState(const FString& RuntimeHash, const FString& ExpectedHash)
			{
				if (RuntimeHash.IsEmpty())
				{
					return TEXT("missing_runtime_hash");
				}
				if (ExpectedHash.IsEmpty())
				{
					return TEXT("missing_expected_indexed_hash");
				}
				return RuntimeHash.Equals(ExpectedHash, ESearchCase::CaseSensitive) ? TEXT("match") : TEXT("mismatch");
			}

			bool ShouldAttemptHashRemap(const FRuntimeMaterialContext& Context) const
			{
				return bEnableHashRemap
					&& Context.MatchState == TEXT("mismatch")
					&& !Context.ExpectedHash.IsEmpty()
					&& Context.AssetPath.StartsWith(TEXT("/Engine/"), ESearchCase::CaseSensitive);
			}

			static bool ParseShaHash(const FString& HashString, FSHAHash& OutHash)
			{
				if (HashString.Len() != FSHAHash::GetStringLen())
				{
					return false;
				}

				OutHash.FromString(HashString);
				return true;
			}

			bool TryRecoverShadersFromExpectedHash(const FRuntimeMaterialContext& Context, const FMaterial* Material, const FMaterialShaderMap* ShaderMap, const FMaterialShaderTypes& ShaderTypes, const FVertexFactoryType* VertexFactoryType, FMaterialShaders& OutShaders)
			{
				if (!ShouldAttemptHashRemap(Context) || !Material || !ShaderMap)
				{
					return false;
				}

				EmitHashRemapUnsupported(Context, JoinRequestedShaderTypeNames(ShaderTypes), ShaderTypes.PipelineType ? ShaderTypes.PipelineType->GetName() : TEXT("<none>"), VertexFactoryType ? VertexFactoryType->GetName() : TEXT("<none>"), ExtractPrimaryPermutationId(ShaderTypes), TEXT("TryGetShaders"));
				return false;
			}

			bool TryRecoverPipelineFromExpectedHash(const FRuntimeMaterialContext& Context, const FMaterial* Material, const FMaterialShaderMap* ShaderMap, FShaderPipelineType* ShaderPipelineType, FVertexFactoryType* VertexFactoryType, FShaderPipelineRef& OutPipeline)
			{
				if (!ShouldAttemptHashRemap(Context) || !Material || !ShaderMap || !ShaderPipelineType || !VertexFactoryType)
				{
					return false;
				}

				EmitHashRemapUnsupported(Context, TEXT("<none>"), ShaderPipelineType->GetName(), VertexFactoryType->GetName(), 0, TEXT("GetShaderPipeline"));
				return false;
			}

			void EmitHashRemapUnsupported(const FRuntimeMaterialContext& Context, const FString& ShaderType, const FString& PipelineType, const FString& VertexFactory, int32 PermutationId, const FString& DetailSource)
			{
				const FString Key = FString::Printf(TEXT("HashRemapUnsupported|%s|%s|%s|%s|%s|%d"),
					*Context.AssetPath,
					*Context.RuntimeHash,
					*Context.ExpectedHash,
					*ShaderType,
					*PipelineType,
					PermutationId);
				if (!MarkEmitted(Key))
				{
					return;
				}

				UE_LOG(LogTajsGraphEarlyDiag, Warning,
					TEXT("[ShaderDiag][HashRemapUnsupported] Material=%s AssetPath=%s RuntimeHash=%s ExpectedIndexedHash=%s ShaderType=%s PipelineType=%s VertexFactory=%s Platform=%s PermutationId=%d DetailSource=%s"),
					Context.MaterialName.IsEmpty() ? TEXT("<unknown>") : *Context.MaterialName,
					Context.AssetPath.IsEmpty() ? TEXT("<unknown>") : *Context.AssetPath,
					Context.RuntimeHash.IsEmpty() ? TEXT("<unavailable>") : *Context.RuntimeHash,
					Context.ExpectedHash.IsEmpty() ? TEXT("<missing>") : *Context.ExpectedHash,
					ShaderType.IsEmpty() ? TEXT("<none>") : *ShaderType,
					PipelineType.IsEmpty() ? TEXT("<none>") : *PipelineType,
					VertexFactory.IsEmpty() ? TEXT("<none>") : *VertexFactory,
					Context.Platform.IsEmpty() ? TEXT("<unknown>") : *Context.Platform,
					PermutationId,
					*DetailSource);
			}

			static FString JoinRequestedShaderTypeNames(const FMaterialShaderTypes& ShaderTypes)
			{
				TArray<FString> Names;
				for (int32 FrequencyIndex = 0; FrequencyIndex < SF_NumFrequencies; ++FrequencyIndex)
				{
					const FShaderType* ShaderType = ShaderTypes.ShaderType[FrequencyIndex];
					if (ShaderType)
					{
						Names.AddUnique(ShaderType->GetName());
					}
				}
				return JoinValues(Names);
			}

			static int32 ExtractPrimaryPermutationId(const FMaterialShaderTypes& ShaderTypes)
			{
				for (int32 FrequencyIndex = 0; FrequencyIndex < SF_NumFrequencies; ++FrequencyIndex)
				{
					if (ShaderTypes.ShaderType[FrequencyIndex])
					{
						return ShaderTypes.PermutationId[FrequencyIndex];
					}
				}
				return 0;
			}

			void EmitLookupMiss(const FRuntimeMaterialContext& Context, const FString& ShaderType, const FString& PipelineType, const FString& VertexFactory, int32 PermutationId, const FString& KeyKind, const FString& DetailSource)
			{
				const FString Key = FString::Printf(TEXT("LookupMiss|%s|%s|%s|%s|%s|%d"),
					*Context.AssetPath,
					*Context.RuntimeHash,
					*ShaderType,
					*PipelineType,
					*VertexFactory,
					PermutationId);
				if (!MarkEmitted(Key))
				{
					return;
				}

				UE_LOG(LogTajsGraphEarlyDiag, Warning,
					TEXT("[ShaderDiag][LookupMiss] Material=%s AssetPath=%s RuntimeHash=%s ExpectedIndexedHash=%s Match=%s ShaderType=%s PipelineType=%s VertexFactory=%s Platform=%s PermutationId=%d KeyKind=%s DetailSource=%s"),
					Context.MaterialName.IsEmpty() ? TEXT("<unknown>") : *Context.MaterialName,
					Context.AssetPath.IsEmpty() ? TEXT("<unknown>") : *Context.AssetPath,
					Context.RuntimeHash.IsEmpty() ? TEXT("<unavailable>") : *Context.RuntimeHash,
					Context.ExpectedHash.IsEmpty() ? TEXT("<missing>") : *Context.ExpectedHash,
					Context.MatchState.IsEmpty() ? TEXT("<unknown>") : *Context.MatchState,
					ShaderType.IsEmpty() ? TEXT("<none>") : *ShaderType,
					PipelineType.IsEmpty() ? TEXT("<none>") : *PipelineType,
					VertexFactory.IsEmpty() ? TEXT("<none>") : *VertexFactory,
					Context.Platform.IsEmpty() ? TEXT("<unknown>") : *Context.Platform,
					PermutationId,
					*KeyKind,
					*DetailSource);
			}

			bool LoadAssetInfoFile(const FString& AssetInfoPath, const FString& SourceLabel, const FString& SourceKind)
			{
				if (!FPaths::FileExists(AssetInfoPath))
				{
					return false;
				}

				FString JsonText;
				if (!FFileHelper::LoadFileToString(JsonText, *AssetInfoPath))
				{
					UE_LOG(LogTajsGraphEarlyDiag, Warning, TEXT("[ShaderDiag][Startup] Failed to read %s assetinfo file %s"), *SourceLabel, *AssetInfoPath);
					return false;
				}

				TSharedPtr<FJsonObject> RootObject;
				const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonText);
				if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
				{
					UE_LOG(LogTajsGraphEarlyDiag, Warning, TEXT("[ShaderDiag][Startup] Failed to parse %s assetinfo JSON %s"), *SourceLabel, *AssetInfoPath);
					return false;
				}

				const TArray<TSharedPtr<FJsonValue>>* Entries = nullptr;
				if (!RootObject->TryGetArrayField(TEXT("ShaderCodeToAssets"), Entries) || Entries == nullptr)
				{
					UE_LOG(LogTajsGraphEarlyDiag, Warning, TEXT("[ShaderDiag][Startup] Missing ShaderCodeToAssets array in %s assetinfo JSON %s"), *SourceLabel, *AssetInfoPath);
					return false;
				}

				int32 AddedEntryCount = 0;
				TSet<FString> AddedHashes;
				for (const TSharedPtr<FJsonValue>& EntryValue : *Entries)
				{
					const TSharedPtr<FJsonObject> EntryObject = EntryValue.IsValid() ? EntryValue->AsObject() : nullptr;
					if (!EntryObject.IsValid())
					{
						continue;
					}

					FString ShaderMapHash;
					if (!EntryObject->TryGetStringField(TEXT("ShaderMapHash"), ShaderMapHash) || ShaderMapHash.IsEmpty())
					{
						continue;
					}

					const TArray<TSharedPtr<FJsonValue>>* AssetValues = nullptr;
					if (!EntryObject->TryGetArrayField(TEXT("Assets"), AssetValues) || AssetValues == nullptr)
					{
						continue;
					}

					for (const TSharedPtr<FJsonValue>& AssetValue : *AssetValues)
					{
						FString AssetPath;
						if (!AssetValue.IsValid() || !AssetValue->TryGetString(AssetPath) || AssetPath.IsEmpty())
						{
							continue;
						}

						const FString NormalizedAssetPath = TajsGraphEarlyShared::NormalizeObjectPath(AssetPath);
						ExpectedHashByAssetPath.Add(NormalizedAssetPath, ShaderMapHash);
						TArray<FString>& Assets = AssetPathsByHash.FindOrAdd(ShaderMapHash);
						Assets.AddUnique(NormalizedAssetPath);
						++AddedEntryCount;
						AddedHashes.Add(ShaderMapHash);
					}
				}

				UE_LOG(LogTajsGraphEarlyDiag, Display,
					TEXT("[ShaderDiag][Startup] Loaded %s assetinfo index from %s [source=%s] (%d entries, %d hashes, %d total indexed assets)"),
					*SourceLabel,
					*AssetInfoPath,
					*SourceKind,
					AddedEntryCount,
					AddedHashes.Num(),
					ExpectedHashByAssetPath.Num());
				return true;
			}

			void HandleShaderLog(const FString& Message)
			{
				static const FString Prefix = TEXT("Missing shader resource for hash '");
				const int32 PrefixIndex = Message.Find(Prefix, ESearchCase::CaseSensitive);
				if (PrefixIndex == INDEX_NONE)
				{
					return;
				}

				const int32 HashStart = PrefixIndex + Prefix.Len();
				const int32 HashEnd = Message.Find(TEXT("'"), ESearchCase::CaseSensitive, ESearchDir::FromStart, HashStart);
				const FString Platform = ExtractQuotedValue(Message, TEXT("shader platform '"));
				if (HashEnd == INDEX_NONE)
				{
					return;
				}

				const FString Hash = Message.Mid(HashStart, HashEnd - HashStart);
				if (Hash.IsEmpty())
				{
					return;
				}

				TArray<FString> MaterialSnapshot;
				{
					FScopeLock Lock(&StateMutex);
					PushBoundedUnique(RecentMissingShaderResourceHashes, Hash, 8);
					MaterialSnapshot = RecentMaterialNames;
				}

				const FString Key = FString::Printf(TEXT("ShaderMiss|%s|%s"), *Platform, *Hash);
				if (!MarkEmitted(Key))
				{
					return;
				}

				UE_LOG(LogTajsGraphEarlyDiag, Warning,
					TEXT("[ShaderDiag][ShaderResourceMiss] Platform=%s ShaderResourceHash=%s KeyKind=Unknown DetailSource=LogFallback RecentMaterials=%s"),
					Platform.IsEmpty() ? TEXT("<unknown>") : *Platform,
					*Hash,
					*JoinValues(MaterialSnapshot));
			}

			void HandleMaterialLog(const FString& Message, ELogVerbosity::Type Verbosity)
			{
				if (Message.Contains(TEXT("[AssetLog]")))
				{
					HandleAssetLog(Message);
				}

				if (Message.Contains(TEXT("Caching uniform expressions for material: ")))
				{
					const FString MaterialName = Message.RightChop(FString(TEXT("Caching uniform expressions for material: ")).Len()).TrimStartAndEnd();
					if (!MaterialName.IsEmpty())
					{
						FScopeLock Lock(&StateMutex);
						PushBoundedUnique(RecentMaterialNames, MaterialName, 8);
					}
					return;
				}

				if (Message.Contains(TEXT("Invalid shader map ID caching shaders for '")))
				{
					const FString MaterialName = ExtractQuotedValue(Message, TEXT("Invalid shader map ID caching shaders for '"));
					EmitMaterialFailure(TEXT("InvalidShaderMapId"), MaterialName, ResolveAssetPathForMaterial(MaterialName), ExtractQuotedValue(Message, TEXT("platform ")), Verbosity);
					return;
				}

				if (Message.Contains(TEXT("Failed to find shader map for default material ")))
				{
					HandleDefaultMaterialFailure(Message, Verbosity);
				}
			}

			void HandleAssetLog(const FString& Message)
			{
				const int32 AssetLogIndex = Message.Find(TEXT("[AssetLog] "), ESearchCase::CaseSensitive);
				if (AssetLogIndex == INDEX_NONE)
				{
					return;
				}

				const int32 PathStart = AssetLogIndex + FString(TEXT("[AssetLog] ")).Len();
				const int32 PathEnd = Message.Find(TEXT(": Failed to compile Material"), ESearchCase::CaseSensitive, ESearchDir::FromStart, PathStart);
				if (PathEnd == INDEX_NONE)
				{
					return;
				}

				const FString FileSystemPath = Message.Mid(PathStart, PathEnd - PathStart).TrimStartAndEnd();
				const FString AssetPath = TajsGraphEarlyShared::NormalizeAssetPathFromFileSystem(FileSystemPath);
				const FString MaterialName = TajsGraphEarlyShared::ExtractShortName(AssetPath);

				if (!MaterialName.IsEmpty() && !AssetPath.IsEmpty())
				{
					FScopeLock Lock(&StateMutex);
					RecentAssetPathByMaterialName.Add(MaterialName, AssetPath);
				}
			}

			void HandleDefaultMaterialFailure(const FString& Message, ELogVerbosity::Type Verbosity)
			{
				static const FString Prefix = TEXT("Failed to find shader map for default material ");
				const int32 PrefixIndex = Message.Find(Prefix, ESearchCase::CaseSensitive);
				if (PrefixIndex == INDEX_NONE)
				{
					return;
				}

				const FString Payload = Message.Mid(PrefixIndex + Prefix.Len());
				const int32 OpenParen = Payload.Find(TEXT("("), ESearchCase::CaseSensitive);
				const int32 CloseParen = Payload.Find(TEXT(")"), ESearchCase::CaseSensitive, ESearchDir::FromStart, OpenParen == INDEX_NONE ? 0 : OpenParen + 1);

				FString MaterialName = Payload;
				FString AssetPath;
				if (OpenParen != INDEX_NONE && CloseParen != INDEX_NONE && CloseParen > OpenParen)
				{
					MaterialName = Payload.Left(OpenParen);
					AssetPath = TajsGraphEarlyShared::NormalizeObjectPath(Payload.Mid(OpenParen + 1, CloseParen - OpenParen - 1));
				}

				MaterialName = MaterialName.TrimStartAndEnd();
				EmitMaterialFailure(TEXT("DefaultMaterialMissing"), MaterialName, AssetPath, TEXT("PCD3D_SM6"), Verbosity);
			}

			void EmitMaterialFailure(const FString& EventName, const FString& MaterialName, FString AssetPath, const FString& PlatformFromMessage, ELogVerbosity::Type Verbosity)
			{
				if (AssetPath.IsEmpty())
				{
					AssetPath = ResolveAssetPathForMaterial(MaterialName);
				}

				const FString Platform = PlatformFromMessage.IsEmpty() ? TEXT("PCD3D_SM6") : PlatformFromMessage;
				const FString ExpectedHash = GetExpectedHashForAssetPath(AssetPath);
				const FString MatchState = ExpectedHash.IsEmpty() ? TEXT("missing_expected_indexed_hash") : TEXT("missing_runtime_hash");

				TArray<FString> MissingHashes;
				{
					FScopeLock Lock(&StateMutex);
					MissingHashes = RecentMissingShaderResourceHashes;
				}

				const FString Key = FString::Printf(TEXT("MaterialFailure|%s|%s|%s|%s"), *EventName, *MaterialName, *AssetPath, *Platform);
				if (!MarkEmitted(Key))
				{
					return;
				}

				if (Verbosity == ELogVerbosity::Error || Verbosity == ELogVerbosity::Fatal)
				{
					UE_LOG(LogTajsGraphEarlyDiag, Error,
						TEXT("[ShaderDiag][MaterialFailure] Event=%s Material=%s AssetPath=%s Platform=%s RuntimeHash=<unavailable> RuntimeHashSource=LogFallback ExpectedIndexedHash=%s Match=%s KeyKind=Unknown ShaderType=<unknown> Pipeline=<unknown> VertexFactory=<unknown> RecentShaderResourceHashes=%s"),
						*EventName,
						MaterialName.IsEmpty() ? TEXT("<unknown>") : *MaterialName,
						AssetPath.IsEmpty() ? TEXT("<unknown>") : *AssetPath,
						*Platform,
						ExpectedHash.IsEmpty() ? TEXT("<missing>") : *ExpectedHash,
						*MatchState,
						*JoinValues(MissingHashes));
				}
				else
				{
					UE_LOG(LogTajsGraphEarlyDiag, Warning,
						TEXT("[ShaderDiag][MaterialFailure] Event=%s Material=%s AssetPath=%s Platform=%s RuntimeHash=<unavailable> RuntimeHashSource=LogFallback ExpectedIndexedHash=%s Match=%s KeyKind=Unknown ShaderType=<unknown> Pipeline=<unknown> VertexFactory=<unknown> RecentShaderResourceHashes=%s"),
						*EventName,
						MaterialName.IsEmpty() ? TEXT("<unknown>") : *MaterialName,
						AssetPath.IsEmpty() ? TEXT("<unknown>") : *AssetPath,
						*Platform,
						ExpectedHash.IsEmpty() ? TEXT("<missing>") : *ExpectedHash,
						*MatchState,
						*JoinValues(MissingHashes));
				}
			}

			bool MarkEmitted(const FString& Key)
			{
				FScopeLock Lock(&StateMutex);
				if (EmittedMessageKeys.Contains(Key))
				{
					return false;
				}
				EmittedMessageKeys.Add(Key);
				return true;
			}

			static void PushBoundedUnique(TArray<FString>& Values, const FString& NewValue, const int32 MaxEntries)
			{
				if (NewValue.IsEmpty())
				{
					return;
				}

				Values.Remove(NewValue);
				Values.Add(NewValue);
				while (Values.Num() > MaxEntries)
				{
					Values.RemoveAt(0);
				}
			}

			FString ResolveAssetPathForMaterial(const FString& MaterialName) const
			{
				if (MaterialName.IsEmpty())
				{
					return FString();
				}

				FScopeLock Lock(&StateMutex);
				if (const FString* Path = RecentAssetPathByMaterialName.Find(MaterialName))
				{
					return *Path;
				}

				FString SingleMatch;
				for (const TPair<FString, FString>& Pair : ExpectedHashByAssetPath)
				{
						if (TajsGraphEarlyShared::ExtractShortName(Pair.Key).Equals(MaterialName, ESearchCase::CaseSensitive))
						{
							if (SingleMatch.IsEmpty())
							{
							SingleMatch = Pair.Key;
						}
						else
						{
							return FString();
						}
					}
				}
				return SingleMatch;
			}

			FString GetExpectedHashForAssetPath(const FString& AssetPath) const
			{
				if (AssetPath.IsEmpty())
				{
					return FString();
				}

				FScopeLock Lock(&StateMutex);
				return ExpectedHashByAssetPath.FindRef(AssetPath);
			}

			static FString ExtractQuotedValue(const FString& Message, const FString& Prefix)
			{
				const int32 StartIndex = Message.Find(Prefix, ESearchCase::CaseSensitive);
				if (StartIndex == INDEX_NONE)
				{
					return FString();
				}

				const int32 ValueStart = StartIndex + Prefix.Len();
				const int32 ValueEnd = Message.Find(TEXT("'"), ESearchCase::CaseSensitive, ESearchDir::FromStart, ValueStart);
				if (ValueEnd == INDEX_NONE)
				{
					return FString();
				}

				return Message.Mid(ValueStart, ValueEnd - ValueStart);
			}

			static FString JoinValues(const TArray<FString>& Values)
			{
				if (Values.IsEmpty())
				{
					return TEXT("<none>");
				}

				return FString::Join(Values, TEXT(";"));
			}
		};

		FShaderDiagnosticOutputDevice GDiagnosticOutputDevice;
	}

	bool Startup()
	{
		return GDiagnosticOutputDevice.Startup();
	}

	void Shutdown()
	{
		GDiagnosticOutputDevice.Shutdown();
	}

	bool IsEnabled()
	{
		return GDiagnosticOutputDevice.IsEnabled();
	}
}
