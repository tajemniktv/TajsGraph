[SystemSettings]
; --- Async & Threading Settings ---
tick.AllowAsyncTickDispatch=1
tick.AllowAsyncTickCleanup=1
tick.AllowAsyncComponentTicks=1
tick.AllowConcurrentTickQueue=1
tick.AnimationDelaysEndGroup=1
tick.HiPriSkinnedMeshes=1
a.ParallelAnimUpdate=1
r.AsyncCompute=1
r.AsyncCreateLightPrimitiveInteractions=1
r.AsyncPipelineCompile=1
r.RDG.AsyncCompute=1
r.PreTileTextures=1

; --- FX & Niagara Settings ---
FX.BatchAsync=1
FX.BatchAsyncBatchSize=1 ; Alternatives: 8
FX.AllowAsyncTick=1
FX.EarlyScheduleAsync=1
FX.AllowGPUParticles=1
FX.AllowGPUSorting=1
FX.AllowCulling=0
FX.FXAllowParticleMeshLODs=1
fx.Niagara.DelayScriptAsyncOptimization=false
fx.Niagara.GpuProfiling.Enabled=false
Niagara.GPUSorting.CPUToGPUThreshold=200

; --- Memory & GC Settings ---
gc.ActorClusteringEnabled=1
gc.AssetClusteringEnabled=1
gc.BlueprintClusteringEnabled=True
gc.CreateGCClusters=1
gc.EnableTimeoutOnPendingDestroyedObjectInShipping=false
gc.LowMemory.MemoryThresholdMB=512 ; Alternatives: 2048, 4096, 8000
gc.LowMemory.TimeBetweenPurgingPendingLevels=15 ; Alternatives: 60
gc.LowMemory.TimeBetweenPurgingPendingKillObjects=60 ; Alternatives: 300
gc.MaxObjectsNotConsideredByGC=32
gc.MinDesiredObjectsPerSubTask=2 ; Alternatives: 4, 20
gc.MultithreadedDestructionEnabled=1
gc.NumRetriesBeforeForcingGC=3 ; Alternatives: 5
gc.TimeBetweenPurgingPendingKillObjects=30 ; Alternatives: 60, 300

; --- Streaming & Texture Settings ---
r.Streaming.PoolSize=-1 ; Alternatives: 0, 2000, 2048, 4096
r.Streaming.MaxTempMemoryAllowed=682 ; Alternatives: 1024, 2048, 4096
r.Streaming.LimitPoolSizeToVRAM=1 ; Alternatives: 0
r.Streaming.PoolSize.VRAMPercentageClamp=512 ; Alternatives: 1024
r.Streaming.UseFixedPoolSize=1
r.Streaming.MaxNumTexturesToStreamPerFrame=2 ; Alternatives: 3, 10, 15, 20, 30, 32
r.Streaming.NumStaticComponentsProcessedPerFrame=2 ; Alternatives: 3, 4096, 6144
r.Streaming.AmortizeCPUToGPUCopy=1
r.Streaming.UsePerTextureBias=1 ; Alternatives: 0
r.Streaming.HLODStrategy=0 ; Alternatives: 2
r.Streaming.FullyLoadUsedTextures=0 ; Alternatives: 1
r.Streaming.DefragDynamicBounds=1
r.Streaming.FramesForFullUpdate=1 ; Alternatives: 2
r.Streaming.UseBackgroundThreadPool=1
r.Streaming.MinMipForSplitRequest=2 ; Alternatives: 15
r.Streaming.HiddenPrimitiveScale=0.0 ; Alternatives: 0.1, 0.0333
r.Streaming.UseAsyncRequestsForDDC=1
r.TextureStreaming=1
r.Streaming.Boost=1 ; Alternatives: 6, 1024
r.Streaming.ParallelRenderAssetsNumWorkgroups=4

; --- Ray Tracing Settings ---
r.RayTracing=True
r.RayTracing.Shadows=True
r.RayTracing.UseTextureLod=True
r.RayTracing.AsyncBuild=1
r.RayTracing.AmbientOcclusion.EnableTwoSidedGeometry=1
r.RayTracing.AmbientOcclusion.SamplesPerPixel=1
r.RayTracing.AmbientOcclusion.Intensity=1.0
r.RayTracing.CacheShaderRecords=1
r.RayTracing.Culling=1
r.RayTracing.Culling.Radius=20000
r.RayTracing.Culling.Angle=20
r.RayTracing.LightCulling.CellSize=100
r.RayTracing.ParallelMeshBatchSize=64
r.RayTracing.NonBlockingPipelineCreation=0
r.RayTracing.Geometry.MaxBuiltPrimitivesPerFrame=500
r.RayTracing.PSOCacheSize=300
r.RayTracing.Shadows.EnableMaterials=1
r.RayTracing.Shadows.EnableFrontFaceCulling=1

; --- Graphics Quality Settings ---
r.MaterialQualityLevel=0 ; Alternatives: 2
r.DetailMode=0 ; Alternatives: 2, 3
r.ViewDistanceScale=0.6 ; Alternatives: 0.7, 1.0, 10
r.PostProcessAAQuality=0 ; Alternatives: 5, 6
r.MotionBlurQuality=0 ; Alternatives: 3
r.BloomQuality=0 ; Alternatives: 2, 5
r.ShadowQuality=0 ; Alternatives: 5
r.SSR.Quality=0 ; Alternatives: 2, 3, 4
r.SSS.Quality=0 ; Alternatives: 1
r.MaxAnisotropy=8 ; Alternatives: 16
r.VT.MaxAnisotropy=16
r.Color.Min=0.00
r.Color.Mid=0.80
r.Color.Max=1.05
r.TonemapperGamma=1.35 ; Alternatives: 2.5
r.Tonemapper.Quality=2
r.Tonemapper.Sharpen=0.2
r.SSGI.Enable=1
r.SSGI.HalfRes=0
r.SSGI.Quality=1 ; Alternatives: 2

; --- Ambient Occlusion Settings ---
r.AmbientOcclusion.Method=1
r.GTAO.Downsample=1
r.AmbientOcclusion.Compute=2
r.AmbientOcclusion.AsyncComputeBudget=2
r.AOComputeShaderNormalCalculation=1
r.AOAsyncBuildQueue=1
r.AOFillGapsHighQuality=2

; --- Lumen Extended Settings ---
r.Lumen.HardwareRayTracing=1
r.Lumen.HardwareRayTracing.LightingMode=1
r.Lumen.TraceMeshSDFs=1
r.Lumen.TraceMeshSDFs.Allow=1
r.Lumen.TraceMeshSDFs.TraceDistance=200 ; Alternatives: 225, 250
r.Lumen.Reflections.SmoothBias=0.8
r.Lumen.Reflections.RadianceCache=1
r.Lumen.TranslucencyReflections.FrontLayer.Enable=1
r.Lumen.ScreenProbeGather.TracingOctahedronResolution=10
r.Lumen.ScreenProbeGather.ReferenceMode=1
r.Lumen.ScreenProbeGather.DownsampleFactor=24 ; Alternatives: 26
r.LumenScene.Radiosity.MaxRayIntensity=200
r.LumenScene.Radiosity.SpatialFilterProbes.KernelSize=0.1
r.LumenScene.Heightfield.MaxTracingSteps=16
r.LumenScene.GlobalSDF.Resolution=256
r.LumenScene.GlobalSDF.ClipmapExtent=2000

; --- Shadow Extended Settings ---
r.Shadow.Virtual.Enable=1
r.Shadow.Virtual.Cache=1
r.Shadow.Virtual.Cache.StaticSeparate=0
r.Shadow.Virtual.NonNaniteVSM=1
r.Shadow.Virtual.Cache.KeepOnlyRequestedPages=1
r.Shadow.Virtual.ForceOnlyVirtualShadowMaps=1
r.Shadow.Virtual.Cache.AllocateViaLRU=0
r.Shadow.Virtual.Cache.MaxUnreferencedLightAge=5
r.Shadow.Virtual.TranslucentQuality=1
r.Shadow.Virtual.SMRT.RayCountLocal=8
r.Shadow.Virtual.SMRT.SamplesPerRayLocal=8
r.Shadow.Virtual.MaxPhysicalPages=4096
r.Shadow.WholeSceneShadowCacheMb=682

; --- Water Settings ---
r.Water.SingleLayer.ShadersSupportVSMFiltering=1
r.Water.SingleLayerWater.SupportCloudShadow=1
r.Water.SingleLayer.VSMFiltering=1

; --- RHI Settings ---
RHI.SyncInterval=0
RHI.SyncSlackMS=0
RHI.MaximumFrameLatency=3
r.RHICmdWidth=12
r.RHICmdUseDeferredContexts=1
r.RHICmdMinDrawsPerParallelCmdList=128 ; Alternatives: 512
r.RHICmdMinCmdlistForParallelSubmit=4
r.RHICmdBalanceParallelLists=1
r.RHICmdAsyncRHIThreadDispatch=1

; --- Shader Pipeline Settings ---
r.ShaderPipelineCache.StartupMode=1 ; Alternatives: 2, 3
r.ShaderPipelineCache.BatchSize=256 ; Alternatives: 10, 128
r.ShaderPipelineCache.PrecompileBatchSize=128
r.ShaderPipelineCache.BatchTime=64
r.ShaderPipelineCache.PrecompileBatchTime=52
r.D3D.ForceDXC=1
r.Shaders.ZeroInitialise=1
r.XGEShaderCompile=1
r.XGEShaderCompile.Mode=1
r.XGEShaderCompile.Xml.BatchGroupSize=256
r.XGEShaderCompile.Xml.BatchSize=16 ; Alternatives: 64

; --- FastVRAM Settings ---
r.FastVRam.GBufferA=1
r.FastVRam.GBufferB=1
r.FastVRam.GBufferC=1
r.FastVRam.GBufferD=1
r.FastVRam.GBufferE=1
r.FastVRam.GBufferF=1
r.FastVRam.GBufferVelocity=1
r.FastVRam.ForwardLightingCullingResources=1
r.FastVRam.HZB=0
r.FastVRam.ShadowPerObject=0
r.FastVRam.ShadowPointLight=1
r.FastVRam.SSR=1
r.FastVRam.CustomDepth=1
r.FastVRam.DistanceFieldShadows=0

; --- AutoExposure Settings ---
r.AutoExposure.IgnoreMaterials=true
r.AutoExposure.IgnoreMaterials.UsePrecalculatedIlluminance=false
r.AutoExposure.IgnoreMaterials.LuminanceScale=0.25
r.DefaultFeature.AutoExposure.ExtendDefaultLuminanceRange=0

; --- Nanite Settings ---
r.Nanite.MaxPixelsPerEdge=2
r.Nanite.DicingRate=1
r.Nanite.FastVisBufferClear=2
r.Nanite.MaterialVisibility=1
r.Shadow.Virtual.Nanite.MaterialVisibility=1

[/Script/Engine.GarbageCollectionSettings]
gc.MaxObjectsNotConsideredByGC=32
gc.SizeOfPermanentObjectPool=0
gc.MaxObjectsInEditor=8388608
gc.MaxObjectsInGame=2097152
gc.MinGCClusterSize=5
gc.ActorClusteringEnabled=True
gc.BlueprintClusteringEnabled=True
gc.UseDisregardForGCOnDedicatedServers=True

[/Script/Engine.Engine]
bEnableMouseSmoothing=False
bAllowMultiThreadedShaderCompile=True
MinDesiredFrameRate=60
bSmoothFrameRate=0
FrameRateLimit=0.000000
SmoothedFrameRateRange=(LowerBound=(Type="ERangeBoundTypes::Inclusive",Value=60),UpperBound=(Type="ERangeBoundTypes::
Exclusive",Value=144))

[/Script/Engine.InputSettings]
bEnableMouseSmoothing=False

[/Script/WindowsTargetPlatform.WindowsTargetSettings]
DefaultGraphicsRHI=DefaultGraphicsRHI_DX12
+D3D12TargetedShaderFormats=PCD3D_SM6

[ConsoleVariables]
AllowAsyncRenderThreadUpdates=1
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=1
AllowAsyncRenderThreadUpdatesEditor=1

[Core.Log]
; Disable all logging for performance
LogPluginManager=all off
LogOnlineIdentity=all off
LogOnlineSession=all off
LogMemory=all off
LogPakFile=all off
LogTemp=all off
LogLinker=all off
LogOnline=all off
LogOnlineGame=all off
LogAnalytics=all off
LogConfig=all off
LogInteractiveProcess=all off
LogInput=all off
LogOnlineEntitlement=all off
LogOnlineEvents=all off
LogOnlineFriend=all off
LogOnlinePresence=all off
LogOnlineTitleFile=all off
LogOnlineUser=all off
Global=off

[TextureStreaming]
BoostPlayerTextures=1.0
AllowStreamingLightmaps=False
r.Streaming.DropMips=0
r.Streaming.LimitPoolSizeToVRAM=0
r.Streaming.MipBias=0
r.Streaming.UseAllMips=1
r.Streaming.UseMaterialData=1
r.Streaming.UseNewMetrics=1
r.Streaming.UsePerTextureBias=1