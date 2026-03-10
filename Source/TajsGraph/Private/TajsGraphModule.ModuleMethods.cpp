#include "TajsGraphModuleInternal.h"
#include "Debug/TajsGraphDebugCommands.h"
#include "Debug/TajsGraphDebugSubsystem.h"
#include "Debug/TajsGraphVisualizationSubsystem.h"

using namespace TajsGraphInternal;

namespace
{
	UTajsGraphVisualizationSubsystem *ResolveVisualizationSubsystem()
	{
		if (!GEngine)
		{
			return nullptr;
		}

		for (const FWorldContext &Context : GEngine->GetWorldContexts())
		{
			UWorld *World = Context.World();
			if (!World || (Context.WorldType != EWorldType::Game && Context.WorldType != EWorldType::PIE))
			{
				continue;
			}

			if (UGameInstance *GameInstance = World->GetGameInstance())
			{
				if (UTajsGraphVisualizationSubsystem *Vis = GameInstance->GetSubsystem<UTajsGraphVisualizationSubsystem>())
				{
					return Vis;
				}
			}
		}

		return nullptr;
	}

	UTajsGraphDebugSubsystem *ResolveDebugSubsystem()
	{
		if (!GEngine)
		{
			return nullptr;
		}

		for (const FWorldContext &Context : GEngine->GetWorldContexts())
		{
			UWorld *World = Context.World();
			if (!World || (Context.WorldType != EWorldType::Game && Context.WorldType != EWorldType::PIE))
			{
				continue;
			}

			if (UGameInstance *GameInstance = World->GetGameInstance())
			{
				if (UTajsGraphDebugSubsystem *DebugSubsystem = GameInstance->GetSubsystem<UTajsGraphDebugSubsystem>())
				{
					return DebugSubsystem;
				}
			}
		}

		return nullptr;
	}

	FString ToLowerTrimmed(const FString &Value)
	{
		FString Out = Value.TrimStartAndEnd();
		Out.ToLowerInline();
		return Out;
	}

	bool TryParseViewModeName(const FString &RawName, ETajsGraphVisMode &OutMode)
	{
		const FString Name = ToLowerTrimmed(RawName);
		if (Name.IsEmpty())
		{
			return false;
		}

		struct FModeAlias
		{
			const TCHAR *Name;
			ETajsGraphVisMode Mode;
		};

		static const FModeAlias Aliases[] = {
			{TEXT("default"), ETajsGraphVisMode::Default},
			{TEXT("basecolor"), ETajsGraphVisMode::BaseColor},
			{TEXT("worldnormal"), ETajsGraphVisMode::WorldNormal},
			{TEXT("roughness"), ETajsGraphVisMode::Roughness},
			{TEXT("metallic"), ETajsGraphVisMode::Metallic},
			{TEXT("specular"), ETajsGraphVisMode::Specular},
			{TEXT("ambientocclusion"), ETajsGraphVisMode::AmbientOcclusion},
			{TEXT("ao"), ETajsGraphVisMode::AmbientOcclusion},
			{TEXT("scenedepth"), ETajsGraphVisMode::SceneDepth},
			{TEXT("luminanceheatmap"), ETajsGraphVisMode::LuminanceHeatmap},
			{TEXT("pseudowireframe"), ETajsGraphVisMode::PseudoWireframe},
			{TEXT("lumencards"), ETajsGraphVisMode::LumenCards},
			{TEXT("meshdistancefields"), ETajsGraphVisMode::MeshDistanceFields},
			{TEXT("globaldistancefield"), ETajsGraphVisMode::GlobalDistanceField},
			{TEXT("lit"), ETajsGraphVisMode::Lit},
			{TEXT("unlit"), ETajsGraphVisMode::Unlit},
			{TEXT("wireframe"), ETajsGraphVisMode::Wireframe},
			{TEXT("detaillighting"), ETajsGraphVisMode::DetailLighting},
			{TEXT("lightingonly"), ETajsGraphVisMode::LightingOnly},
			{TEXT("reflections"), ETajsGraphVisMode::Reflections},
			{TEXT("shadercomplexity"), ETajsGraphVisMode::ShaderComplexity}};

		for (const FModeAlias &Alias : Aliases)
		{
			if (Name == Alias.Name)
			{
				OutMode = Alias.Mode;
				return true;
			}
		}

		return false;
	}

	void LogViewModeList()
	{
		UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Available view modes:"));
		UE_LOG(LogTajsGraph, Display, TEXT("  lit, unlit, wireframe, detaillighting, lightingonly, reflections, shadercomplexity"));
		UE_LOG(LogTajsGraph, Display, TEXT("  basecolor, worldnormal, roughness, metallic, specular, ambientocclusion, scenedepth"));
		UE_LOG(LogTajsGraph, Display, TEXT("  luminanceheatmap, pseudowireframe, lumencards, meshdistancefields, globaldistancefield"));
		UE_LOG(LogTajsGraph, Display, TEXT("  off/default"));
	}

	bool IsAllowedDebugCVarName(const FString &RawName)
	{
		const FString Name = ToLowerTrimmed(RawName);
		if (Name.IsEmpty())
		{
			return false;
		}

		if (Name == TEXT("r.buffervisualizationtarget") ||
			Name == TEXT("r.visualizemeshdistancefields") ||
			Name == TEXT("r.aoglobaldistancefieldvisualize") ||
			Name == TEXT("r.forcedebugviewmodes"))
		{
			return true;
		}

		return Name.StartsWith(TEXT("r.lumen.visualize."));
	}
}

void FTajsGraphModule::InstallBuildableFoliageHook()
{
	SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
		void (UActorComponent::*)(),
		UActorComponent,
		RegisterComponent,
		[](TCallScope<void (*)(UActorComponent *)> &Scope, UActorComponent *Self)
		{
			RefreshRuntimeConfig(Self ? Self->GetWorld() : nullptr);
			FPPVConfig ConfigSnapshot;
			{
				FScopeLock Lock(&GPPVConfigMutex);
				ConfigSnapshot = GPPVConfig;
			}

			Scope(Self);

			if (!Self || !ConfigSnapshot.bFixBuildableFoliage)
			{
				return;
			}

			UHierarchicalInstancedStaticMeshComponent *HISM = Cast<UHierarchicalInstancedStaticMeshComponent>(Self);
			if (!HISM)
			{
				return;
			}

			AActor *Owner = HISM->GetOwner();
			const bool bBuildableOwned = IsBuildableOwnedActor(Owner);
			const bool bRealWorldFoliage = Owner && Owner->IsA<AInstancedFoliageActor>();

			if (bBuildableOwned && !bRealWorldFoliage)
			{
				if (Owner && Owner->IsRunningUserConstructionScript())
				{
					return;
				}
				ApplyRuntimeAssetRemap(HISM, ConfigSnapshot);

				const EHISMViewRelevanceType OldRelevance = HISM->GetViewRelevanceType();
				if (HISM->GetViewRelevanceType() == EHISMViewRelevanceType::Foliage)
				{
					UTajsGraphFoliageAccess::SetViewRelevance(HISM, EHISMViewRelevanceType::HISM);
				}

				HISM->bAffectDistanceFieldLighting = true;
				HISM->SetMobility(EComponentMobility::Movable);
				HISM->bCastStaticShadow = false;

				ApplyNaniteOverrides(HISM, ConfigSnapshot, TEXT("BuildableHISM"));

				const EHISMViewRelevanceType NewRelevance = HISM->GetViewRelevanceType();
				const FString OwnerClassName = Owner ? Owner->GetClass()->GetName() : TEXT("<null>");
				const FString ComponentClassName = HISM->GetClass()->GetName();
				const FString MeshName = HISM->GetStaticMesh() ? HISM->GetStaticMesh()->GetName() : TEXT("<null>");

				static TSet<FString> LoggedKeys;
				const FString LogKey = FString::Printf(TEXT("%s|%s|%d|%d|%d"),
													   *OwnerClassName,
													   *ComponentClassName,
													   static_cast<int32>(OldRelevance),
													   static_cast<int32>(NewRelevance),
													   HISM->bAffectDistanceFieldLighting ? 1 : 0);

				if (!LoggedKeys.Contains(LogKey))
				{
					LoggedKeys.Add(LogKey);
					if (ShouldDebugLog())
					{
						UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph][Debug][BuildableHISM] OwnerClass=%s ComponentClass=%s Mesh=%s Relevance %d->%d AffectDF=%d Mobility=%d CastStaticShadow=%d"),
							   *OwnerClassName,
							   *ComponentClassName,
							   *MeshName,
							   static_cast<int32>(OldRelevance),
							   static_cast<int32>(NewRelevance),
							   HISM->bAffectDistanceFieldLighting ? 1 : 0,
							   static_cast<int32>(HISM->Mobility),
							   HISM->bCastStaticShadow ? 1 : 0);
					}
				}
			}
		});
}

void FTajsGraphModule::StartupModule()
{
#if WITH_EDITOR
	if (GIsEditor)
	{
		UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Unreal Editor detected; skipping runtime module initialization."));
		return;
	}
#endif

	GStartupTimeSeconds = FPlatformTime::Seconds();
	bLoggedCVarDelay = false;
	LoadPersistentPPVConfig(GPPVConfig);
	const FString ConfigPath = GetPPVConfigPath();
	if (ShouldGeneralLog())
	{
		UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph] StartupModule. ConfigPath=%s Exists=%s"),
			   *ConfigPath,
			   FPaths::FileExists(ConfigPath) ? TEXT("true") : TEXT("false"));
		UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph] Runtime config source: SML mod configuration (%s)"), *ConfigPath);
		UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph] PPV config loaded: enabled=%s, forceInstancing=%s, forceLumenInstancing=%s, forceNanite=%s, fixBuildableFoliage=%s"),
			   GPPVConfig.bEnabled ? TEXT("true") : TEXT("false"),
			   GPPVConfig.bForceInstancing ? TEXT("true") : TEXT("false"),
			   GPPVConfig.bForceLumenInstancing ? TEXT("true") : TEXT("false"),
			   GPPVConfig.bForceNanite ? TEXT("true") : TEXT("false"),
			   GPPVConfig.bFixBuildableFoliage ? TEXT("true") : TEXT("false"));
		UE_LOG(LogTajsGraph, Log, TEXT("[TajsGraph] Remap config: enabled=%s, meshRules=%d, materialRules=%d"),
			   GPPVConfig.bEnableAssetRemap ? TEXT("true") : TEXT("false"),
			   GPPVConfig.MeshRemap.Num(),
			   GPPVConfig.MaterialRemap.Num());
	}

	// Register CVAR dump console command
	static FAutoConsoleCommand DumpCVarsCmd(
		TEXT("tajsgraph.cvardump"),
		TEXT("Dumps all CVars to Saved/Logs/TajsGraphCVarDump.txt"),
		FConsoleCommandDelegate::CreateLambda([]()
											  {
			FString OutFile = FPaths::Combine(FPaths::ProjectLogDir(), TEXT("TajsGraphCVarDump.txt"));
			TArray<FString> Lines;
			IConsoleManager::Get().ForEachConsoleObjectThatStartsWith(FConsoleObjectVisitor::CreateLambda([
				&Lines
			](const TCHAR* Name, IConsoleObject* Obj) {
				if (Obj) {
					FString Desc = Obj->GetHelp();
					FString Value;
					if (IConsoleVariable* Var = Obj->AsVariable()) {
						Value = Var->GetString();
					}
					Lines.Add(FString::Printf(TEXT("%s = %s // %s"), Name, *Value, *Desc));
				}
			}), TEXT(""));
			FFileHelper::SaveStringArrayToFile(Lines, *OutFile);
			UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Dumped %d CVars to %s"), Lines.Num(), *OutFile); }));

	static IConsoleObject *ViewModeCmd = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("tajsgraph.viewmode"),
		TEXT("Set debug visualization mode. Usage: tajsgraph.viewmode list | off | <mode>"),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString> &Args)
													  {
			if (Args.Num() <= 0) {
				UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Usage: tajsgraph.viewmode list | off | <mode>"));
				LogViewModeList();
				return;
			}

			UTajsGraphVisualizationSubsystem *Vis = ResolveVisualizationSubsystem();
			if (!Vis) {
				UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] No active game world/viewport. Cannot apply visualization mode."));
				return;
			}

			const FString Command = ToLowerTrimmed(Args[0]);
			if (Command == TEXT("list")) {
				LogViewModeList();
				return;
			}

			if (Command == TEXT("off")) {
				Vis->SetVisualizationMode(ETajsGraphVisMode::Default);
				Vis->SetVisualizationEnabled(false);
				UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Visualization mode disabled."));
				return;
			}

			ETajsGraphVisMode Mode = ETajsGraphVisMode::Default;
			if (!TryParseViewModeName(Command, Mode)) {
				UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] Unknown mode '%s'."), *Command);
				LogViewModeList();
				return;
			}

			const bool bApplied = Vis->SetVisualizationMode(Mode);
			if (Mode == ETajsGraphVisMode::Default) {
				Vis->SetVisualizationEnabled(false);
			} else {
				Vis->SetVisualizationEnabled(true);
			}

			UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Visualization mode request: %s (applied_now=%s, enabled=%s)"),
				*StaticEnum<ETajsGraphVisMode>()->GetNameStringByValue(static_cast<int64>(Mode)),
				bApplied ? TEXT("true") : TEXT("false"),
				Vis->IsVisualizationEnabled() ? TEXT("true") : TEXT("false")); }),
		ECVF_Default);

	static IConsoleObject *DebugCVarCmd = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("tajsgraph.debugcvar"),
		TEXT("Apply an allowed visualization debug CVar. Usage: tajsgraph.debugcvar <name> <value>"),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString> &Args)
													  {
			if (!ResolveVisualizationSubsystem()) {
				UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] No active game world/viewport. Cannot apply debug visualization CVar."));
				return;
			}

			if (Args.Num() < 2) {
				UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Usage: tajsgraph.debugcvar <name> <value>"));
				return;
			}

			const FString CVarName = Args[0].TrimStartAndEnd();
			if (!IsAllowedDebugCVarName(CVarName)) {
				UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] Rejected debugcvar '%s'. Allowed: r.BufferVisualizationTarget, r.Lumen.Visualize.*, r.VisualizeMeshDistanceFields, r.AOGlobalDistanceFieldVisualize, r.ForceDebugViewModes"), *CVarName);
				return;
			}

			FString Value;
			for (int32 Index = 1; Index < Args.Num(); ++Index) {
				if (!Value.IsEmpty()) {
					Value += TEXT(" ");
				}
				Value += Args[Index];
			}

			IConsoleVariable *CVar = IConsoleManager::Get().FindConsoleVariable(*CVarName);
			if (!CVar) {
				UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] CVar not found: %s"), *CVarName);
				return;
			}

			CVar->Set(*Value, ECVF_SetByCode);
			UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Set %s=%s"), *CVarName, *Value); }),
		ECVF_Default);

	static IConsoleObject *HubCmd = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("tajsgraph.hub"),
		TEXT("Open/close/toggle the TajsGraph Hub menu. Usage: tajsgraph.hub open|close|toggle"),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString> &Args)
													  {
			UTajsGraphDebugSubsystem *DebugSubsystem = ResolveDebugSubsystem();
			if (!DebugSubsystem) {
				UE_LOG(LogTajsGraph, Warning, TEXT("[TajsGraph] No active game world. Cannot control hub menu."));
				return;
			}

			const FString Mode = Args.Num() > 0 ? ToLowerTrimmed(Args[0]) : TEXT("toggle");
			if (Mode == TEXT("open")) {
				DebugSubsystem->OpenHub();
				return;
			}
			if (Mode == TEXT("close")) {
				DebugSubsystem->CloseHub();
				return;
			}
			if (Mode == TEXT("toggle")) {
				DebugSubsystem->ToggleHub();
				return;
			}

			UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Usage: tajsgraph.hub open|close|toggle"));
		}),
		ECVF_Default);
	InstallPPVHooks();
	InstallStaticMeshRemapHook();
	UE_LOG(LogTajsGraph, Display, TEXT("[TajsGraph] Remap startup: enabled=%s, meshRules=%d, materialRules=%d"),
		GPPVConfig.bEnableAssetRemap ? TEXT("true") : TEXT("false"),
		GPPVConfig.MeshRemap.Num(),
		GPPVConfig.MaterialRemap.Num());
	FTajsGraphDebugCommands::Startup();
}

void FTajsGraphModule::ShutdownModule()
{
#if WITH_EDITOR
	if (GIsEditor)
	{
		return;
	}
#endif

	FTajsGraphDebugCommands::Shutdown();
}

IMPLEMENT_GAME_MODULE(FTajsGraphModule, TajsGraph);



