+D3D12TargetedShaderFormats=PCD3D_SM6

a.ParallelAnimUpdate=1
a.URO.DisableInterpolation=
a.URO.ForceInterpolation=

AllowAsyncRenderThreadUpdates=1
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=1
AllowAsyncRenderThreadUpdatesEditor=1
AllowStreamingLightmaps=False
AnimPhysicsMinDeltaTime=0.000000
AsyncLoadingTimeLimit=5.000000

b.LevelStreamingDistanceRatioForLOD=1.0
b.LevelStreamingMaxLODLevel=3
b.NetMoveEnableByDistance=-1
bAllowMultiThreadedShaderCompile=true
bDefaultHasComplexCollision=True
bDisableActiveActors=False
bDisableCCD=False
bDisableKinematicKinematicPairs=False
bDisableKinematicStaticPairs=False
bEnable2DPhysics=False
bEnableClientVisibilityTracking=True
bEnableEnhancedDeterminism=False
bEnableMouseSmoothing=0
bEnableMultiCoreRendering=True
bEnablePCM=True
bEnableRayTracing=true
bEnableShapeSharing=False
bEnableStabilization=False
BoostPlayerTextures=1.0
BounceThresholdVelocity=200.000000
bPauseOnLossOfFocus=False
bSharedMaterialNativeLibraries=True
bShareMaterialShaderCode=True
bSimulateAnimPhysicsAfterReset=False
bSimulateSkeletalMeshOnDedicatedServer=True
bSmoothFrameRate=False
bSubstepping=False
bSubsteppingAsync=False
bSupportUVFromHitResults=False
bSuppressFaceRemapTable=False
bUseBackgroundLevelStreaming=True
bUseFixedFrameRate=False
bUseFixedTimeStep=False
bUseThreadedPhysics=True
bUseVSync=false
bWarnMissingLocks=True

Compat.UseDXT5NormalMaps=0
ConnectionTimeout=30.0
ContactOffsetMultiplier=0.020000
CSS.Conveyor.EnableLodOptimizations=1

D3D11.AdjustTexturePoolSizeBasedOnBudget=1
D3D11.AFRSyncTemporalResources=1
D3D11.AFRUseFramePacing=1
D3D11.AsyncDeferredDeletion=1
D3D11.ForceThirtyHz=0
D3D11.InsertOuterOcclusionQuery=1
D3D11.ResidencyManagement=1
D3D11.StablePowerState=0
D3D11.SyncWithDWM=1
D3D11.TexturePoolOnlyAccountStreamableTexture=1
D3D11.UseAllowTearing=1
D3D11.UseUpdateTexture3DComputeShader=1
d3d11.ZeroBufferSizeInMB=32

D3D12.AdjustTexturePoolSizeBasedOnBudget=1
D3D12.AFRSyncTemporalResources=1
D3D12.AFRUseFramePacing=1
D3D12.AsyncDeferredDeletion=1
D3D12.EnableAsyncCompileShader=1
D3D12.EnableDynamicBarriers=1
D3D12.EnablePipelineStateCache=1
D3D12.EvictAllResidentResourcesInBackground=true
d3d12.FastAllocator.MinPagesToRetain=2 ;alternatives: 4
D3D12.Force16BitBufferGPUReads=1
D3D12.ForceThirtyHz=0
D3D12.GPUTimeout=
D3D12.InsertOuterOcclusionQuery=1
D3D12.MaxDescriptorHeapSize=1000000
D3D12.MaximumFrameLatency=2 ;alternatives: 0, 3
D3D12.NumBufferedFrames=
D3D12.PreferD32DepthBuffer=0
D3D12.PreferDXCCompiler=1
D3D12.PSO.Cache=
D3D12.PSO.DiskCache=1
D3D12.PSO.DriverOptimizedDiskCache=1
D3D12.PSO.StablePSO=1
D3D12.ResidencyManagement=1
D3D12.StablePowerState=0
D3D12.SyncWithDWM=0 ;alternatives: 1
D3D12.TexturePoolOnlyAccountStreamableTexture=1
D3D12.UseAllowTearing=1
D3D12.UseUpdateTexture3DComputeShader=0 ;alternatives: 1
D3D12.ZeroBufferSizeInMB=32 ;alternatives: 128

DefaultGraphicsRHI=DefaultGraphicsRHI_DX12

foliage.DensityScale=0.1 ;alternatives: 2.5
foliage.DitheredLOD=1
foliage.LODDistanceScale=1
foliage.MinVertsToSplitNode=8000

FrameRateLimit=0.000000
FrictionCombineMode=Average

FX.AllowAsyncTick=1
FX.AllowCulling=0 ;alternatives: 1
FX.AllowGPUParticles=1
FX.AllowGPUSorting=1
FX.BatchAsync=1
FX.BatchAsyncBatchSize=1 ;alternatives: 8
FX.EarlyScheduleAsync=1
FX.FXAllowParticleMeshLODs=1

fx.Niagara.DelayScriptAsyncOptimization=false
fx.Niagara.GpuProfiling.Enabled=false

gc.ActorClusteringEnabled=1
gc.AssetClusteringEnabled=1
gc.BlueprintClusteringEnabled=True
gc.CreateGCClusters=1
gc.EnableTimeoutOnPendingDestroyedObjectInShipping=false
gc.LowMemory.MemoryThresholdMB=2048 ;alternatives: 512, 4096, 8000
gc.LowMemory.TimeBetweenPurgingPendingKillObjects=60 ;alternatives: 300
gc.LowMemory.TimeBetweenPurgingPendingLevels=15 ;alternatives: 60
gc.MaxObjectsInEditor=8388608
gc.MaxObjectsInGame=2097152
gc.MaxObjectsNotConsideredByGC=32
gc.MinDesiredObjectsPerSubTask=4 ;alternatives: 2, 20
gc.MinGCClusterSize=5
gc.MultithreadedDestructionEnabled=1
gc.NumRetriesBeforeForcingGC=3 ;alternatives: 5
gc.SizeOfPermanentObjectPool=0
gc.TimeBetweenPurgingPendingKillObjects=30 ;alternatives: 15, 60, 300
gc.UseDisregardForGCOnDedicatedServers=True

GeometryCache.InterpolateFrames=1
GeometryCache.OffloadUpdate=1

grass.DensityScale=0.25 ;alternatives: 0.8, 2.5
grass.Enable=1
grass.MaxAsyncTasks=4
grass.MaxComputeShaderTiles=64
grass.TickInterval=10

IdealLightMapDensity=0.02
InitialAverageFrameRate=0.016667
InitialConnectTimeout=30.0
InitialDelay=0.5

KeepAliveTime=0.2

LanServerMaxTickRate=120
LevelStreaming.UnloadNotVisibleLevelsWhenLowRAM=1
LevelStreaming.UnloadNotVisibleLevelsWhenLowRAMNormalizedPercentage=80
LevelStreamingActorsUpdateTimeLimit=5.000000

LogAnalytics=all off
LogConfig=all off
LogInput=all off
LogInteractiveProcess=all off
LogLinker=all off
LogMemory=all off
LogOnline=all off
LogOnlineEntitlement=all off
LogOnlineEvents=all off
LogOnlineFriend=all off
LogOnlineGame=all off
LogOnlineIdentity=all off
LogOnlinePresence=all off
LogOnlineSession=all off
LogOnlineTitleFile=all off
LogOnlineUser=all off
LogPakFile=all off
LogPluginManager=all off
LogTemp=all off

Lumen.DiffuseIndirect.AsyncCompute=1

MaxAngularVelocity=3600.000000
MaxClientRate=104857600 ;alternatives: 15000
MaxContactOffset=8.000000
MaxDepenetrationVelocity=0.000000
MaxDynamicBandwidth=16000
MaxES2PixelShaderAdditiveComplexityCount=45
MaximumLoopIterationCount=1000000
MaxInternetClientRate=104857600 ;alternatives: 10000
MaxLightMapDensity=0.05
MaxNetTickRate=120
MaxPhysicsDeltaTime=0.033333
MaxPixelShaderAdditiveComplexityCount=128
MaxSubstepDeltaTime=0.016667
MaxSubsteps=6

Memory.EnablePurgeRequests=1
Memory.IncrementalPurgeMaxSizeThreshold=
Memory.PurgeSleepTimeMsec=

MinContactOffset=2.000000
MinDesiredFrameRate=60.000000
MinDynamicBandwidth=8000

NetServerMaxTickRate=60

niagara.CreateShadersOnLoad=1
Niagara.GPUSorting.CPUToGPUThreshold=200

NumPawnsAllowedToBeSpawnedInAFrame=20

PhysicsUpdateDeltaTime=0.016667
PhysXTreeRebuildRate=10
PriorityAsyncLoadingExtraTime=15.000000

r.AccelPredrawBatchTime=0 ;alternatives: 13
r.AccelTargetPrecompileFrameTime=0
r.AllowDownsampledStandardTranslucency=1
r.AllowGlobalClipPlane=1
r.AllowLandscapeShadows=1
r.AllowOcclusionQueries=0 ;alternatives: 1
r.AllowPointLightCubemapShadows=1
r.AllowStaticLighting=0
r.AllowSubPrimitiveQueries=0 ;alternatives: 1
r.AmbientOcclusion.AsyncComputeBudget=2
r.AmbientOcclusion.Compute=1 ;alternatives: 2
r.AmbientOcclusion.Denoiser=0 ;alternatives: 1, 3
r.AmbientOcclusion.FadeRadiusScale=2
r.AmbientOcclusion.HalfRes=1
r.AmbientOcclusion.Method=1
r.AmbientOcclusionLevels=2 ;alternatives: 0, 25
r.AmbientOcclusionMaxQuality=100 ;alternatives: 0
r.AmbientOcclusionMipLevelFactor=0.5 ;alternatives: 0.0
r.AmbientOcclusionRadiusScale=1.0 ;alternatives: 0.0
r.AmbientOcclusionSampleSetQuality=3
r.AOAsyncBuildQueue=1
r.AOComputeShaderNormalCalculation=1
r.AOFillGapsHighQuality=2
r.AOGlobalDistanceField.FastCameraMode=0
r.AOGlobalDistanceField.MinMeshSDFRadius=1 ;alternatives: 5, 10
r.AOMaxViewDistance=16384
r.AOMinLevel=0
r.AOQuality=1
r.AOScatterInterpolation=1
r.AOSpecularOcclusionMode=2

r.AsyncCompute.bAllowAsyncDataReadback=1
r.AsyncCompute.bAllowBackgroundThreads=1
r.AsyncCompute.MaxNonComputeTaskCost=10
r.AsyncCompute.MaxNumBackgroundThreads=4
r.AsyncCompute=1
r.AsyncCreateLightPrimitiveInteractions=1
r.AsyncPipelineCompile=1
r.Atmosphere=0

r.AutoExposure.IgnoreMaterials.LuminanceScale=0.25
r.AutoExposure.IgnoreMaterials.UsePrecalculatedIlluminance=false
r.AutoExposure.IgnoreMaterials=true

r.BasePassOutputsVelocity=0
r.bForceCPUAccessToGPUSkinVerts=True

r.Bloom.AsyncCompute=1
r.Bloom.Cross=0.6
r.Bloom.HalfResolutionFFT=1
r.BloomQuality=0 ;alternatives: 2, 5

r.BlurGBuffer=0 ;alternatives: -1
r.BufferVisualizationDumpFramesAsHDR=0

r.Cache.DrawDirectionalShadowing=1
r.Cache.DrawInterpolationPoints=1
r.Cache.UpdateEveryFrame=1
r.CacheComputeShaders=1
r.CacheDrawCommands=1
r.CachePSOs=1
r.CacheShaders=1

r.CapsuleDirectShadows=0
r.CapsuleIndirectShadows=0
r.CapsuleShadows=1
r.CGV.CustomInterp=1
r.ClearSceneMethod=2
r.ClearWithExcludeRects=2

r.Color.Max=1.05
r.Color.Mid=0.80
r.Color.Min=0.00

r.CompileShadersForDevelopment=0
r.ComputeShader.AsyncCompileThreshold=10 ;alternatives: 100
r.ComputeShader.bAllowAsyncCompile=1
r.ComputeShader.ForceParallelTranslation=1
r.ComputeShader.WaveOperations=1
r.ComputeShader.WaveSize=32 ;alternatives: 64

r.ContactShadows=1 ;alternatives: 0
r.CreateShadersOnLoad=1
r.CullBeforeFetch=1

r.CustomDepth.Order=0
r.CustomDepth.Stencil=1
r.CustomDepthEnableFastClear=1

r.D3D.ForceDXC=0 ;alternatives: 1
r.D3D.ForceShaderConductorDXCRewrite=0
r.D3D11.UseAllowTearing=1
r.D3D12.AllowAsyncCompute=1
r.D3D12.ExecuteCommandListTask=1
r.D3D12.UseAllowTearing=1

r.DBuffer=0 ;alternatives: 1

r.DefaultFeature.AmbientOcclusion=False
r.DefaultFeature.AmbientOcclusionStaticFraction=False
r.DefaultFeature.AutoExposure.ExtendDefaultLuminanceRange=0
r.DefaultFeature.AutoExposure.Method=0
r.DefaultFeature.AutoExposure=False
r.DefaultFeature.Bloom=False
r.DefaultFeature.LensFlare=True
r.DefaultFeature.MotionBlur=False ;alternatives: True

r.DeferSkeletalDynamicDataUpdateUntilGDME=1
r.DeferUniformBufferUpdatesUntilVisible=1
r.DepthOfField.FarBlur=0
r.DepthOfFieldQuality=0 ;alternatives: 2

r.DetailMode=0 ;alternatives: 2, 3
r.DFFullResolution=1
r.DFShadow.TwoSidedMeshDistanceBiasScale=0
r.DFShadowAsyncCompute=1
r.DFShadowQuality=3 ;alternatives: 0

r.DiffuseIndirect.Denoiser=0 ;alternatives: 1

r.DiscardUnusedQuality=1
r.DistanceField.MaxPerMeshResolution=128
r.DistanceFieldAO=1
r.DistanceFieldGI=1
r.DistanceFieldShadowing=1
r.DoInitViewsLightingAfterPrepass=1
r.DontLimitOnBattery=0 ;alternatives: 1
r.DownsampledOcclusionQueries=1
r.DX11NVAfterMathEnabled=0
r.DynamicGlobalIlluminationMethod=1
r.DynamicRes.MinScreenPercentage=100
r.DynamicRes.OperationMode=0

r.EarlyZPass=2
r.EarlyZPassMovable=1
r.EarlyZPassOnlyMaterialMasking=1
r.Emitter.FastPoolEnable=1
r.EnableAsyncComputeTranslucencyLightingVolumeClear=1
r.EnableAsyncComputeVolumetricFog=1
r.EnableComputeBuildHZB=1
r.experimental.Nanite.EnableMeshes=1
r.experimental.Nanite.EnableStreaming=1
r.EyeAdaptation.MethodOveride=1
r.EyeAdaptationQuality=3 ;alternatives: 2

r.FastBlurThreshold=0 ;alternatives: 3, 7

r.FastVRam.BokehDOF=1
r.FastVRam.CircleDOF=1
r.FastVRam.CombineLUTs=1
r.FastVRam.CustomDepth=1
r.FastVRam.DistanceFieldAODownsampledBentNormal=0
r.FastVRam.DistanceFieldAOHistory=0
r.FastVRam.DistanceFieldAOScreenGridResources=0
r.FastVRam.DistanceFieldNormal=0
r.FastVRam.DistanceFieldShadows=0
r.FastVRam.DistanceFieldTileIntersectionResources=0
r.FastVRam.Distortion=1
r.FastVRam.EyeAdaptation=1
r.FastVRam.ForwardLightingCullingResources=1
r.FastVRam.GBufferA=1
r.FastVRam.GBufferB=1
r.FastVRam.GBufferC=1
r.FastVRam.GBufferD=1
r.FastVRam.GBufferE=1
r.FastVRam.GBufferF=1
r.FastVRam.GBufferVelocity=1
r.FastVRam.Histogram=1
r.FastVRam.HZB=1
r.FastVRam.ShadowPerObject=0
r.FastVRam.ShadowPointLight=1
r.FastVRam.SSR=1
r.FastVRam.Tonemap=1
r.FastVRam.Upscale=1

r.FidelityFX.FI.AllowAsyncWorkloads=1
r.FidelityFX.FI.Enabled=0
r.FidelityFX.FI.OverrideSwapChainDX12=1
r.FidelityFX.FI.RHIPacingMode=1
r.FidelityFX.FSR3.AdjustMipBias=0 ;alternatives: 1
r.FidelityFX.FSR3.AutoExposure=1
r.FidelityFX.FSR3.CreateReactiveMask=1
r.FidelityFX.FSR3.DeDither=1
r.FidelityFX.FSR3.HistoryFormat=0 ;alternatives: 1 (balanced), 2 (quality)
r.FidelityFX.FSR3.QualityMode=1
r.FidelityFX.FSR3.ReparentMode=0 ;alternatives: 1, 2
r.FidelityFX.FSR3.Sharpness=0.5 ;alternatives: 0.3, 0.7
r.FidelityFX.FSR3.UseSSRExperimentalDenoiser=1

r.FilmGrain=0
r.Filter.SizeScale=0.2 ;alternatives: 1.0
r.FinishCurrentFrame=0
r.ForceAllCoresForShaderCompiling=1
r.ForceDebugViewModes=0
r.ForceHighestMipOnUITextures=1
r.ForceLOD=-1 ;alternatives: 0, 1
r.ForceSceneHasDecals=1
r.FrustumCulling=
r.FrustumCullNumWordsPerTask=32

r.GBuffer=1
r.GBufferDiffuseSampleOcclusion=1
r.GBufferFormat=3 ;alternatives: 5
r.GenerateLandscapeGIData=1
r.GenerateMeshDistanceFields=1
r.GlobalIllumination.Denoiser=2
r.GPUCrashDebugging=0
r.GPUHitchDebug=0
r.GPUParticle.Simulate=1
r.GPUScene.ParallelUpdate=0

r.GTAO.Downsample=1
r.GTSyncType=1

r.GPUSkin.Support16BitBoneIndex = True
r.GPUSkin.Limit2BoneInfluences = True
r.GPUSkin.UnlimitedBoneInfluences = False
r.GPUSkin.UnlimitedBoneInfluencesThreshold = 8
MaxSkinBones = (Default=65536,PerPlatform=(("Mobile", 256)))

r.HairStrands.Cull=2
r.HairStrands.Visibility.UseFastPath=1
r.HBAO.Enable=1
r.HeightFieldShadowing=0
r.HeightfieldGlobalIllumination=1
r.HeightfieldInnerBounceDistance=3000
r.HeightfieldOuterBounceDistanceScale=3
r.HeightfieldTargetUnitsPerTexel=200
r.HFShadowQuality=0
r.HighQualityLightMaps=1
r.HZB.BuildUseCompute=1
r.HZBOcclusion=0 ;alternatives: 1

r.LandscapeLODBias=-1 ;alternatives: 0, 0.5
r.LandscapeLODBiasMultiplier=1.0
r.LandscapeLODForcedDistanceOffset=0
r.LandscapeMaxLOD=8
r.LandscapeUseOptionalLODs=1
r.LandscapeUseTessellation=0

r.LensFlareQuality=0 ;alternatives: 2, 3
r.LightFunctionQuality=3
r.LightMaxDrawDistanceScale=1 ;alternatives: 0, 2, 5
r.LightPropagationVolume=1
r.LightShaftDownSampleFactor=4
r.LightShaftQuality=0 ;alternatives: 1
r.LODFadeTime=0.2

r.LPV.EmissiveMultiplier=
r.LPV.GeometryVolumeBias=
r.LPV.IntensityBoost=
r.LPV.RSMResolution=4 ;alternatives: 1024

r.Lumen.AsyncCompute=1
r.Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.HardwareRayTracing.LightingMode=2 ;alternatives: 1
r.Lumen.HardwareRayTracing=True
r.Lumen.RadianceCache.SortTraceTiles=1
r.Lumen.RadianceCache.SpatialFilterMaxRadianceHitAngle=10 ;alternatives: 30
r.Lumen.Reflections.AsyncCompute=1
r.Lumen.Reflections.DownsampleFactor=1
r.Lumen.Reflections.GGXSamplingBias=0.3
r.Lumen.Reflections.HierarchicalScreenTraces.MaxIterations=200 ;alternatives: 400
r.Lumen.Reflections.MaxBounces=3 ;alternatives: 6
r.Lumen.Reflections.MaxRoughness=0.3 ;alternatives: 0.5
r.Lumen.Reflections.RadianceCache=1
r.Lumen.Reflections.ScreenSpaceReconstruction.NumSamples=10
r.Lumen.Reflections.ScreenTraces=1
r.Lumen.Reflections.SmoothBias=0.8
r.Lumen.Reflections.Temporal.MaxFramesAccumulated=128
r.Lumen.Reflections.TraceMeshSDFs=1
r.Lumen.Reflections.TranslucencyReflections.FrontLayer.Enable=1
r.Lumen.Reflections.TranslucencyReflections.FrontLayer.EnableForProject=1
r.Lumen.SampleFog=1
r.Lumen.ScreenProbeGather.DiffuseIntegralMethod=1
r.Lumen.ScreenProbeGather.DownsampleFactor=24 ;alternatives: 26
r.Lumen.ScreenProbeGather.FullResolutionJitterWidth=0.5
r.Lumen.ScreenProbeGather.ImportanceSample.MinPDFToTrace=0.5
r.Lumen.ScreenProbeGather.MaxRayIntensity=200
r.Lumen.ScreenProbeGather.ReferenceMode=1
r.Lumen.ScreenProbeGather.ScreenTraces.HZBTraversal.MaxIterations=400 ;alternatives: 200
r.Lumen.ScreenProbeGather.ScreenTraces.HZBTraversal.SkipFoliageHits=0
r.Lumen.ScreenProbeGather.ScreenTraces.HZBTraversal=1
r.Lumen.ScreenProbeGather.ScreenTraces.MinimumOccupancy=1
r.Lumen.ScreenProbeGather.ShortRangeAO.ScreenSpace.SlopeCompareToleranceScale=1
r.Lumen.ScreenProbeGather.SpatialFilterNumPasses=2
r.Lumen.ScreenProbeGather.StochasticInterpolation=4 ;alternatives: 2
r.Lumen.ScreenProbeGather.Temporal.MaxFramesAccumulated=25 ;alternatives: 30
r.Lumen.ScreenProbeGather.TemporalFilterProbes=1
r.Lumen.ScreenProbeGather.TraceMeshSDFs=1
r.Lumen.ScreenProbeGather.TracingOctahedronResolution=10
r.Lumen.ScreenProbeGather.TwoSidedFoliageBackfaceDiffuse=1
r.Lumen.SkylightLeaking.Roughness=0.5
r.Lumen.TraceDistanceScale=1
r.Lumen.TraceMeshSDFs.Allow=1
r.Lumen.TraceMeshSDFs.TraceDistance=200 ;alternatives: 225, 250
r.Lumen.TraceMeshSDFs=1
r.Lumen.TranslucencyReflections.FrontLayer.Enable=1
r.Lumen.TranslucencyReflections.FrontLayer.EnableForProject=1
r.Lumen.TranslucencyVolume.RadianceCache.FarField=1

r.MinScreenRadiusForLights = 0.030000
r.MinScreenRadiusForDepthPrepass = 0.200000
r.MinScreenRadiusForCSMDepth = 0.010000

r.Material.RoughDiffuse = True
r.Material.EnergyConservation = False

r.Nanite.SoftwareVRS=1
r.Nanite.DicingRate=1
r.Nanite.FastVisBufferClear=2

r.RayTracing.GlobalIllumination.FireflySuppression=1
r.RayTracing.SkyLight.MaxShadowThickness=5
r.RayTracing.Translucency.MinRayDistance=100
r.RayTracing.Reflections.ExperimentalDeferred.GenerateRaysWithRGS=0

r.Streaming.ParallelRenderAssetsNumWorkgroups=4
r.Streaming.UseFixedPoolSize=1

r.TransientResourceAliasing.Buffers=1
r.TranslucencyLightingVolumeDim=16

r.Water.SingleLayer.ShadersSupportVSMFiltering=1
r.Water.SingleLayer.VSMFiltering=1
r.Water.SingleLayerWater.SupportCloudShadow=1

r.VRS.Enable=true
r.VRS.EnableImage=true
r.VRS.BasePass=1
r.VRS.Decals=1
r.VRS.SSAO=1
r.VRS.SSR=1
r.VRS.NaniteEmitGBuffer=1

r.VT.MaxUploadsPerFrame=4
r.VT.MaxContinuousUpdatesPerFrame=1
r.VT.RVT.TileCountBias=-1
r.VT.ParallelFeedbackTasks=1
r.VT.EnableAutoImport = True
r.VT.TileSize = 128
r.VT.TileBorderSize = 4
r.vt.FeedbackFactor = 16
bEnableVirtualTextureOpacityMask = True

s.AllowLevelRequestsWhileAsyncLoadingInMatch=1
s.AsyncIOBandwidthLimit=0 ;alternatives: 50
s.AsyncLoading.MaxConcurrentResLoads=2 ;alternatives: 8, 16
s.AsyncLoadingThreadEnabled=True
s.AsyncLoadingThreadPriority=0 ;alternatives: 1
s.AsyncLoadingThreadStackSizeKB=32 ;alternatives: 128
s.AsyncLoadingTimeLimit=2.0 ;alternatives: 5.0
s.AsyncLoadingUseFullTimeLimit=0 ;alternatives: 1
s.ContinuouslyIncrementalGCWhileLevelsPendingPurge=0 ;alternatives: 1
s.ForceGCAfterLevelStreamedOut=0 ;alternatives: 1
s.IoDispatcherCacheSizeMB=512 ;alternatives: 256, 2048
s.LevelStreamingActorsUpdateTimeLimit=1.00 ;alternatives: 2.00
s.LevelStreamingComponentsRegistrationGranularity=1
s.LevelStreamingComponentsUnregistrationGranularity=1
s.MaxIncomingRequestsToStall=10 ;alternatives: 1, 24, 100
s.MaxReadyRequestsToStallMB=6 ;alternatives: 0
s.MinBulkDataSizeForAsyncLoading=0 ;alternatives: 1000
s.PriorityAsyncLoadingExtraTime=10.00
s.UnregisterComponentsTimeLimit=0.00 ;alternatives: 2.00

sg.FoliageLoadDistance=2 ;alternatives: 4
sg.Globalillumination=4

SimulateScratchMemorySize=262144
SpawnPrioritySeconds=1.0
SyncSceneSmoothingFactor=0.000000

TaskGraph.AllowBulkShortTasks=1
TaskGraph.EnableProfiling=0
TaskGraph.ForkedProcessMaxWorkerThreads=16
TaskGraph.MaxWorkerThreads=8 ;alternatives: 16
TaskGraph.NumWorkerThreads=0 ;alternatives: 4, 8 (0=auto)
TaskGraph.ShowPerTaskTimingInfo=0
TaskGraph.UseSharedProducerConsumerQueue=1

tick.AllowAsyncComponentTicks=1
tick.AllowAsyncTickCleanup=1
tick.AllowAsyncTickDispatch=1
tick.AllowConcurrentTickQueue=1
tick.AnimationDelaysEndGroup=1
tick.HiPriSkinnedMeshes=1

TimeBetweenPurgingPendingKillObjects=15
TimeoutMultiplierForUnoptimizedBuilds=1
TriangleMeshTriangleMinAreaThreshold=5.000000