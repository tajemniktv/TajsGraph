#include "TajsGraphModuleInternal.h"

namespace TajsGraphInternal {

namespace {

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4191)
#endif

template <typename VolumeType>
static void InstallPPVHookImpl() {
    SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
        FPostProcessVolumeProperties (VolumeType::*)() const,
        VolumeType,
        GetProperties,
        [](TCallScope<FPostProcessVolumeProperties(*)(const VolumeType*)>& Scope, const VolumeType* Self) {
            RefreshRuntimeConfig(Self ? Self->GetWorld() : nullptr);
            FPostProcessVolumeProperties Properties = Scope(Self);
            if (!Properties.Settings || !Self) {
                Scope.Override(Properties);
                return;
            }

            FPPVConfig ConfigSnapshot;
            {
                FScopeLock Lock(&GPPVConfigMutex);
                ConfigSnapshot = GPPVConfig;
            }

            static thread_local FPostProcessSettings OverrideSettings;
            OverrideSettings = *Properties.Settings;
            ApplyPPVOverrides(OverrideSettings, ConfigSnapshot);
            Properties.Settings = &OverrideSettings;
            Scope.Override(Properties);
        }
    );
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

static void InstallStaticMeshRemapHookImpl() {
    SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
        void (UActorComponent::*)(),
        UActorComponent,
        RegisterComponent,
        [](TCallScope<void(*)(UActorComponent*)>& Scope, UActorComponent* Self) {
            RefreshRuntimeConfig(Self ? Self->GetWorld() : nullptr);
            FPPVConfig ConfigSnapshot;
            {
                FScopeLock Lock(&GPPVConfigMutex);
                ConfigSnapshot = GPPVConfig;
            }
            Scope(Self);

            UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Self);
            if (!StaticMeshComponent || !ConfigSnapshot.bEnableAssetRemap || StaticMeshComponent->IsTemplate()) {
                return;
            }

            // The colored instance proxy has subsystem-dependent instancing behavior.
            // Keep it out of the generic remap path to avoid construction-time crashes.
            if (Cast<UFGColoredInstanceMeshProxy>(StaticMeshComponent)) {
                return;
            }

            FString GuardReason;
            if (ShouldGuardSurfaceCacheMutation(StaticMeshComponent, &GuardReason)) {
                AddSurfaceCacheTrace(
                    ETajsGraphSurfaceCacheTraceKind::StaticMeshRemapSkipped,
                    StaticMeshComponent,
                    StaticMeshComponent,
                    GuardReason,
                    false,
                    true);
                return;
            }

            ApplyRuntimeAssetRemap(StaticMeshComponent, ConfigSnapshot);
        }
    );
}



static void InstallColoredInstanceMeshProxyHookImpl() {
    SUBSCRIBE_UOBJECT_METHOD_EXPLICIT(
        void (UFGColoredInstanceMeshProxy::*)(),
        UFGColoredInstanceMeshProxy,
        OnRegister,
        [](TCallScope<void(*)(UFGColoredInstanceMeshProxy*)>& Scope, UFGColoredInstanceMeshProxy* Self) {
            RefreshRuntimeConfig(Self ? Self->GetWorld() : nullptr);
            FPPVConfig ConfigSnapshot;
            {
                FScopeLock Lock(&GPPVConfigMutex);
                ConfigSnapshot = GPPVConfig;
            }
            Scope(Self);
            if (!Self) {
                return;
            }

            AActor* Owner = Self->GetOwner();
            if (!IsBuildableOwnedActor(Owner)) {
                return;
            }

            // Save-load and construction paths can register the proxy before the
            // buildable subsystem is ready to create a colored instance manager.
            // Forcing instancing here can trip the engine assertion seen in shipping.
            if (Owner && Owner->IsRunningUserConstructionScript()) {
                return;
            }

            if (ConfigSnapshot.bForceInstancing) {
                Self->mBlockInstancing = false;
                Self->SetInstanced(true);
            }

            if (ConfigSnapshot.bForceLumenInstancing) {
                Self->mBlockInstancingWithLumen = false;
            }

            ApplyRuntimeAssetRemap(Self, ConfigSnapshot);

            ApplyNaniteOverrides(Self, ConfigSnapshot, TEXT("ColoredInstanceMeshProxy"));

            Self->bCastStaticShadow = false;
        }
    );
}

}

void InstallPPVHooks() {
    InstallPPVHookImpl<AFGAtmosphereVolume>();
    InstallPPVHookImpl<AFGWaterVolume>();
    InstallPPVHookImpl<AFGGasPillar>();
    InstallPPVHookImpl<AFGDamageOverTimeVolume>();
}


void InstallStaticMeshRemapHook() {
    InstallStaticMeshRemapHookImpl();
}

void InstallColoredInstanceMeshProxyHook() {
    InstallColoredInstanceMeshProxyHookImpl();
}

}

