; ==============================================================
; PODSTAWOWY RENDERING
; ==============================================================

r.ViewDistanceScale=1.0 ;alternatives: 10.0 (najwyższa), 0.6 (najniższa)
r.ScreenPercentage=100 ;alternatives: 70 (optymalizacja), 99 (wysoka)
r.DefaultFeature.MotionBlur=False ;alternatives: True, 0
r.MotionBlur.Max=0
r.MotionBlurQuality=0
r.FastBlurThreshold=0 ;alternatives: 3
r.BloomQuality=2 ;alternatives: 0 (wyłączone), 5 (wysokie)
r.Tonemapper.Quality=2 ;alternatives: 1 (niska), 5 (wysoka)
r.Tonemapper.Sharpen=0.1 ;alternatives: 1.6 (max)
r.TonemapperFilm=1
r.TonemapperGamma=2.2 ;alternatives: 2.0, 2.5, Rec709
r.MaxAnisotropy=8 ;alternatives: 16

; ==============================================================
; OPTYMALIZACJE CPU/GPU
; ==============================================================

r.Shaders.Optimize=1
r.Shaders.FastMath=1
r.GTSyncType=1
r.OneFrameThreadLag=1 ;alternatives: 0 (mniejsze opóźnienie)
r.FinishCurrentFrame=0
r.CreateShadersOnLoad=1
r.OptimizeForUAVPerformance=1
r.DontLimitOnBattery=0 ;alternatives: 1
r.ClearWithExcludeRects=2
r.DeferSkeletalDynamicDataUpdateUntilGDME=1
r.DeferUniformBufferUpdatesUntilVisible=1
r.DoInitViewsLightingAfterPrepass=1

; ==============================================================
; ASYNC I MULTITHREAD
; ==============================================================

AllowAsyncRenderThreadUpdates=1
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=1
AllowAsyncRenderThreadUpdatesEditor=1
r.AsyncCompute=1
r.AsyncPipelineCompile=1
r.UseAsyncShaderPrecompilation=1
tick.AllowAsyncTickDispatch=1
tick.AllowAsyncTickCleanup=1
FX.BatchAsync=1
r.RHICmdAsyncRHIThreadDispatch=1

; ==============================================================
; SHADER PIPELINE
; ==============================================================

r.ShaderPipelineCache.Enabled=1
r.ShaderPipelineCache.StartupMode=2 ;alternatives: 1, 3
r.ShaderPipelineCache.PreOptimizeEnabled=1
r.ShaderPipelineCache.SaveUserCache=1
r.ShaderPipelineCache.BatchSize=128 ;alternatives: 10, 256
r.D3D.ForceDXC=0 ;alternatives: 1

; ==============================================================
; TEKSTUROWANIE
; ==============================================================

r.Streaming.PoolSize=2048 ;alternatives: -1 (auto), 0, 2000, 4096
r.Streaming.LimitPoolSizeToVRAM=1 ;alternatives: 0
r.VirtualTextureReducedMemory=1
r.VirtualTexture=1
r.VirtualTextures=1
r.Streaming.MaxTempMemoryAllowed=2048 ;alternatives: 682, 4096
r.Streaming.AmortizeCPUToGPUCopy=1
r.Streaming.MaxNumTexturesToStreamPerFrame=30 ;alternatives: 2, 3, 32

; ==============================================================
; SYSTEM CIENIOWANIA
; ==============================================================

r.Shadow.Virtual.Enable=1
r.Shadow.Virtual.Cache=1
r.Shadow.Virtual.Cache.StaticSeparate=0
r.Shadow.Virtual.NonNaniteVSM=1
r.Shadow.Virtual.Cache.AllocateViaLRU=1 ;alternatives: 0
r.Shadow.Virtual.Cache.KeepOnlyRequestedPages=1
r.Shadow.Virtual.ForceOnlyVirtualShadowMaps=1
r.Shadow.Virtual.TranslucentQuality=1
r.Shadow.Virtual.SMRT.RayCountLocal=8
r.Shadow.Virtual.SMRT.SamplesPerRayLocal=8
r.Shadow.Virtual.MaxPhysicalPages=4096
r.Shadow.DistanceScale=1.0 ;alternatives: 0.001, 1.5, 2, 4
r.Shadow.MaxResolution=2048 ;alternatives: 1024, 4096, 8192
r.Shadow.CSM.MaxCascades=4 ;alternatives: 0, 1, 2, 10
r.Shadow.CSM.TransitionScale=1.0
r.Shadow.CSM.MaxNearCascades=4 ;alternatives: 0
r.ContactShadows=1 ;alternatives: 0
r.DFShadowQuality=3 ;alternatives: 0
r.Shadow.CachePreshadow=1
r.Shadow.CacheWPOPrimitives=1
; ==============================================================
; LUMEN I GLOBAL ILLUMINATION
; ==============================================================

r.Lumen.TraceMeshSDFs=1
r.Lumen.TraceMeshSDFs.Allow=1
r.Lumen.TraceMeshSDFs.TraceDistance=200 ;alternatives: 250
r.Lumen.Reflections.RadianceCache=1
r.Lumen.Reflections.SmoothBias=0.8
r.Lumen.Reflections.MaxRoughness=0.3 ;alternatives: 0.5
r.Lumen.ScreenProbeGather.DownsampleFactor=24 ;alternatives: 26
r.Lumen.ScreenProbeGather.TracingOctahedronResolution=10
r.Lumen.ScreenProbeGather.Temporal.MaxFramesAccumulated=25 ;alternatives: 30
r.LumenScene.GPUDrivenUpdate=0
r.LumenScene.Lighting.AsyncCompute=1
r.LumenScene.SurfaceCache.CardTexelDensityScale=5000 ;alternatives: 1000, 10000
r.DynamicGlobalIlluminationMethod=1

; ==============================================================
; AMBIENT OCCLUSION I REFLECTIONS
; ==============================================================

r.AmbientOcclusion.Method=1
r.AmbientOcclusion.Compute=1 ;alternatives: 2
r.SSGI.Enable=1
r.SSGI.HalfRes=0
r.SSGI.Quality=1 ;alternatives: 2
r.SSR.Quality=3 ;alternatives: 4
r.ReflectionMethod=1
r.ReflectionCapture.EnableLightFunctions=1

; ==============================================================
; RAYTRACING
; ==============================================================

r.RayTracing=True
r.RayTracing.Shadows=True
r.RayTracing.UseTextureLod=True
r.Lumen.HardwareRayTracing=True
r.Lumen.HardwareRayTracing.LightingMode=2
r.RayTracing.AsyncBuild=1
r.RayTracing.ParallelMeshBatchSize=64
r.RayTracing.PSOCacheSize=300
r.RayTracing.Culling=1
r.RayTracing.Culling.Radius=20000
r.RayTracing.Culling.Angle=20
r.RayTracing.Geometry.MaxBuiltPrimitivesPerFrame=500

; ==============================================================
; FSR (FidelityFX Super Resolution)
; ==============================================================

r.FidelityFX.FSR3.Sharpness=0.7
r.FidelityFX.FSR3.DeDither=1
r.FidelityFX.FSR3.CreateReactiveMask=1
r.FidelityFX.FSR3.UseSSRExperimentalDenoiser=1
r.FidelityFX.FSR3.AutoExposure=1
r.FidelityFX.FSR3.QualityMode=1
r.FidelityFX.FI.Enabled=0
r.FidelityFX.FI.OverrideSwapChainDX12=1
r.FidelityFX.FI.AllowAsyncWorkloads=1
r.FidelityFX.FI.RHIPacingMode=1

; ==============================================================
; VRS (Variable Rate Shading)
; ==============================================================

r.VRS.Enable=true
r.VRS.EnableImage=true
r.Nanite.SoftwareVRS=1
r.VRS.NaniteEmitGBuffer=1
r.VRS.BasePass=1
r.VRS.Decals=1
r.VRS.SSAO=1
r.VRS.SSR=1

; ==============================================================
; NANITE
; ==============================================================

r.Nanite.MaxPixelsPerEdge=2
r.Nanite.DicingRate=1
r.Nanite.FastVisBufferClear=2
r.Nanite.MaterialVisibility=1

; ==============================================================
; POST-PROCESSING I EFEKTY
; ==============================================================

r.PostProcessAAQuality=5 ;alternatives: 0, 6
r.DefaultFeature.Bloom=False
r.DefaultFeature.AmbientOcclusion=False
r.DefaultFeature.AmbientOcclusionStaticFraction=False
r.DefaultFeature.AutoExposure=False
r.DefaultFeature.LensFlare=True
r.DefaultFeature.MotionBlur=False ;alternatives: True
r.DepthOfFieldQuality=0
r.EyeAdaptationQuality=3
r.LensFlareQuality=0 ;alternatives: 3
r.LightShaftQuality=0 ;alternatives: 1
r.RefractionQuality=3 ;alternatives: 5
r.SceneColorFringeQuality=0
r.TemporalAA.Upsampling=1
r.TemporalAACatmullRom=0 ;alternatives: 1
r.TemporalAACurrentFrameWeight=0.05 ;alternatives: 0, 0.25
r.TemporalAASamples=4 ;alternatives: 32
r.TemporalAASharpness=1 ;alternatives: 1.4

; ==============================================================
; FOLIAGE I TRAWA
; ==============================================================

grass.DensityScale=0.25 ;alternatives: 2.5
grass.TickInterval=10
grass.Enable=1
foliage.DensityScale=0.1 ;alternatives: 2.5
sg.FoliageLoadDistance=2 ;alternatives: 4

; ==============================================================
; LOD I MESH SETTINGS
; ==============================================================

r.StaticMeshLODBias=0 ;alternatives: -2
r.SkeletalMeshLODBias=-1 ;alternatives: -2, 0
r.LandscapeLODBias=0 ;alternatives: -1, 0.5
r.ParticleLODBias=0 ;alternatives: -1
r.SkinCache.Allow=true
r.SkinCache.CompileShaders=1
r.SkinCache.Mode=1

; ==============================================================
; VOLUMETRIC EFFECTS
; ==============================================================

r.VolumetricFog=1 ;alternatives: 0
r.VolumetricFog.GridPixelSize=4 ;alternatives: 0
r.VolumetricFog.GridSizeZ=64 ;alternatives: 0
r.VolumetricFog.Jitter=1
r.VolumetricFog.HistoryMissSupersampleCount=16
r.VolumetricCloud=1 ;alternatives: 0

; ==============================================================
; DX12 SPECYFICZNE
; ==============================================================

D3D12.PSO.DiskCache=1
D3D12.PSO.DriverOptimizedDiskCache=1
D3D12.AFRSyncTemporalResources=1
D3D12.AFRUseFramePacing=1
D3D12.AsyncDeferredDeletion=1
D3D12.ForceThirtyHz=0
D3D12.InsertOuterOcclusionQuery=1
D3D12.ResidencyManagement=1
D3D12.StablePowerState=0
D3D12.SyncWithDWM=0 ;alternatives: 1
D3D12.TexturePoolOnlyAccountStreamableTexture=1
D3D12.UseUpdateTexture3DComputeShader=0 ;alternatives: 1
D3D12.ZeroBufferSizeInMB=32 ;alternatives: 128
D3D12.MaximumFrameLatency=2 ;alternatives: 0, 3
d3d12.FastAllocator.MinPagesToRetain=2 ;alternatives: 4
; ==============================================================
; GARBAGE COLLECTION
; ==============================================================

gc.ActorClusteringEnabled=1
gc.AssetClusteringEnabled=1
gc.BlueprintClusteringEnabled=True
gc.CreateGCClusters=1
gc.NumRetriesBeforeForcingGC=3 ;alternatives: 5
gc.MinDesiredObjectsPerSubTask=4 ;alternatives: 2, 20
gc.MaxObjectsNotConsideredByGC=32
gc.MultithreadedDestructionEnabled=1
gc.TimeBetweenPurgingPendingKillObjects=30 ;alternatives: 15, 60, 300
gc.LowMemory.TimeBetweenPurgingPendingLevels=15 ;alternatives: 60
gc.LowMemory.TimeBetweenPurgingPendingKillObjects=60 ;alternatives: 300
gc.LowMemory.MemoryThresholdMB=2048 ;alternatives: 512, 4096, 8000

; ==============================================================
; NETWORK SETTINGS
; ==============================================================

TotalNetBandwidth=128000
MaxDynamicBandwidth=16000
MinDynamicBandwidth=8000

; ==============================================================
; ENGINE CORE SETTINGS
; ==============================================================

bEnableMouseSmoothing=0
bUseVSync=false
bSmoothFrameRate=0
bAllowMultiThreadedShaderCompile=True
FrameRateLimit=0.000000
MinDesiredFrameRate=60

; ==============================================================
; IO SETTINGS
; ==============================================================

s.IoDispatcherCacheSizeMB=512 ;alternatives: 256, 2048
s.MaxIncomingRequestsToStall=10 ;alternatives: 1, 24, 100
s.AsyncLoadingTimeLimit=2.0
s.LevelStreamingActorsUpdateTimeLimit=1.00 ;alternatives: 2.00
s.UnregisterComponentsTimeLimit=0.00 ;alternatives: 2.00
s.AsyncLoadingUseFullTimeLimit=0
s.ContinuouslyIncrementalGCWhileLevelsPendingPurge=0 ;alternatives: 1
s.PriorityAsyncLoadingExtraTime=10.00

; ==============================================================
; SZCZEGÓŁOWE USTAWIENIA STREAMINGU
; ==============================================================

r.Streaming.DropMips=0 ;alternatives: 1, 2
r.Streaming.HLODStrategy=0 ;alternatives: 2
r.Streaming.UseFixedPoolSize=0 ;alternatives: 1
r.Streaming.PoolSize.VRAMPercentageClamp=1024
r.Streaming.UseAsyncRequestsForDDC=1
r.Streaming.FullyLoadUsedTextures=0 ;alternatives: 1
r.Streaming.DefragDynamicBounds=1
r.bForceCPUAccessToGPUSkinVerts=True
r.Streaming.FramesForFullUpdate=1 ;alternatives: 2
r.Streaming.MinMipForSplitRequest=2 ;alternatives: 0, 15
r.Streaming.ScaleTexturesByGlobalMyBias=1.0 ;alternatives: 0.5
r.Streaming.Boost=1.0 ;alternatives: 6, 1024

; ==============================================================
; RHI SETTINGS
; ==============================================================

rhi.SyncInterval=0
rhi.SyncSlackMS=0
RHI.MaximumFrameLatency=0 ;alternatives: 3
r.RHICmdUseDeferredContexts=1
r.RHICmdMinDrawsPerParallelCmdList=128 ;alternatives: 512
r.RHICmdMinCmdlistForParallelSubmit=4
r.RHICmdBalanceParallelLists=1 ;alternatives: 2
r.RHICmdBalanceTranslatesAfterTasks=0 ;alternatives: 1
r.RHICmdFlushRenderThreadTasksPrePass=1
r.RHICmdFlushRenderThreadTasksShadowPass=1
r.RHICmdFlushRenderThreadTasksBasePass=1
r.RHICmdFlushRenderThreadTasksTranslucentPass=1
r.RHICmdFlushRenderThreadTasksVelocityPass=1

; ==============================================================
; GEOMETRY I PHYSICS
; ==============================================================

GeometryCache.InterpolateFrames=1
GeometryCache.OffloadUpdate=1
r.GenerateMeshDistanceFields=1
r.DistanceField.MaxPerMeshResolution=128
r.AOGlobalDistanceField.MinMeshSDFRadius=1 ;alternatives: 5, 10
CSS.Conveyor.EnableLodOptimizations=1

; ==============================================================
; NIAGARA I FX
; ==============================================================

fx.Niagara.DelayScriptAsyncOptimization=false
fx.Niagara.GpuProfiling.Enabled=false
Niagara.GPUSorting.CPUToGPUThreshold=200
FX.AllowGPUParticles=1
FX.AllowAsyncTick=1
FX.EarlyScheduleAsync=1
FX.BatchAsyncBatchSize=1 ;alternatives: 8
FX.AllowGPUSorting=1
FX.FXAllowParticleMeshLODs=1
FX.AllowCulling=0 ;alternatives: 1

; ==============================================================
; WATER SETTINGS
; ==============================================================

r.Water.SingleLayer.ShadersSupportVSMFiltering=1
r.Water.SingleLayerWater.SupportCloudShadow=1
r.Water.SingleLayer.VSMFiltering=1

; ==============================================================
; MATERIAŁY I SHADERY
; ==============================================================

bShareMaterialShaderCode=True
bSharedMaterialNativeLibraries=True
r.MaterialQualityLevel=1 ;alternatives: 0
r.BasePassOutputsVelocity=0
r.CustomDepth.Order=0
r.CustomDepthEnableFastClear=1
r.GBufferFormat=3 ;alternatives: 5
; ==============================================================
; SYSTEM SETTINGS CORE
; ==============================================================

r.DBuffer=0 ;alternatives: 1
r.HZBOcclusion=0 ;alternatives: 1
r.VolumetricCloud.HzbCulling=0
r.FrustumCullNumWordsPerTask=32
UIManager.PauseMenuStreamingMemoryClear=2048
r.AllowOcclusionQueries=0 ;alternatives: 1
r.AllowSubPrimitiveQueries=0 ;alternatives: 1
r.ForceHighestMipOnUITextures=1
r.HairStrands.Cull=2
r.HairStrands.Visibility.UseFastPath=1

; ==============================================================
; AUTO-EXPOSURE & TONE MAPPING
; ==============================================================

r.AutoExposure.IgnoreMaterials=true
r.AutoExposure.IgnoreMaterials.UsePrecalculatedIlluminance=false
r.AutoExposure.IgnoreMaterials.LuminanceScale=0.25
r.DefaultFeature.AutoExposure.ExtendDefaultLuminanceRange=0
r.Color.Min=0.00
r.Color.Mid=0.80
r.Color.Max=1.05

; ==============================================================
; ADAPTIVE PERFORMANCE
; ==============================================================

r.DynamicRes.OperationMode=0
r.DynamicRes.MinScreenPercentage=100
r.OptimizeForUAVPerformance=1
a.ParallelAnimUpdate=1
r.EnableAsyncComputeVolumetricFog=1

; ==============================================================
; ADVANCED CULLING
; ==============================================================

r.AllowDownsampledStandardTranslucency=1
r.CullBeforeFetch=1
r.HZBOcclusion=1
r.EnableComputeBuildHZB=1
r.GBufferDiffuseSampleOcclusion=1
r.GPUScene.ParallelUpdate=1

; ==============================================================
; SHADER COMPILATION
; ==============================================================

r.XGEShaderCompile=1
r.XGEShaderCompile.Mode=1
r.XGEShaderCompile.Xml.BatchGroupSize=256 ;alternatives: 64
r.XGEShaderCompile.Xml.BatchSize=16 ;alternatives: 64
r.XGEShaderCompile.Xml.JobTimeout=0.500000

; ==============================================================
; FAST VRAM SETTINGS
; ==============================================================

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
r.FastVRam.DistanceFieldAODownsampledBentNormal=0
r.FastVRam.DistanceFieldAOHistory=0
r.FastVRam.DistanceFieldAOScreenGridResources=0
r.FastVRam.DistanceFieldNormal=0
r.FastVRam.DistanceFieldTileIntersectionResources=0

; ==============================================================
; LUMEN SZCZEGÓŁOWE USTAWIENIA
; ==============================================================

r.LumenScene.SurfaceCache.MeshCardsMinSize=5 ;alternatives: 10
r.LumenScene.SurfaceCache.CardMinResolution=1
r.LumenScene.SurfaceCache.Compress=2
r.LumenScene.SurfaceCache.AtlasSize=1024
r.LumenScene.SurfaceCache.ResetEveryNthFrame=2400
r.Lumen.Reflections.MaxBounces=3 ;alternatives: 6
r.Lumen.Reflections.ScreenSpaceReconstruction.NumSamples=10
r.Lumen.Reflections.Temporal.MaxFramesAccumulated=128
r.Lumen.Reflections.ScreenTraces=1
r.Lumen.Reflections.HierarchicalScreenTraces.MaxIterations=200 ;alternatives: 400
r.Lumen.ScreenProbeGather.ImportanceSample.MinPDFToTrace=0.5
r.Lumen.ScreenProbeGather.ScreenTraces.MinimumOccupancy=1
r.Lumen.ScreenProbeGather.ShortRangeAO.ScreenSpace.SlopeCompareToleranceScale=1
r.Lumen.RadianceCache.SortTraceTiles=1
r.Lumen.RadianceCache.SpatialFilterMaxRadianceHitAngle=10 ;alternatives: 30
r.Lumen.TranslucencyVolume.RadianceCache.FarField=1

; ==============================================================
; RAY TRACING ROZSZERZONE
; ==============================================================

r.RayTracing.Geometry.GetEachLODForHISM=0
r.RayTracing.Geometry.NiagaraSprites=0
r.RayTracing.Geometry.NiagaraRibbons=0
r.RayTracing.Geometry.NiagaraMeshes=0
r.RayTracing.Geometry.InstancedStaticMeshes.SimulationCount=8 ;alternatives: 16
r.RayTracing.GlobalIllumination=0
r.RayTracing.GlobalIllumination.ScreenPercentage=10
r.RayTracing.GlobalIllumination.SamplesPerPixel=1
r.RayTracing.GlobalIllumination.MaxRayDistance=100000
r.RayTracing.GlobalIllumination.MaxLightCount=32
r.RayTracing.GlobalIllumination.MaxBounces=3
r.RayTracing.GlobalIllumination.EnableTransmission=1
r.RayTracing.GlobalIllumination.FireflySuppression=1
r.RayTracing.GlobalIllumination.Lights.SpotLight=1
r.RayTracing.GlobalIllumination.Lights.SkyLight=1
r.RayTracing.GlobalIllumination.Lights.RectLight=1
r.RayTracing.GlobalIllumination.Lights.DirectionalLight=1
r.RayTracing.GlobalIllumination.Lights.PointLight=1
; ==============================================================
; PATH TRACING SETTINGS
; ==============================================================

r.PathTracing.MaxBounces=8
r.PathTracing.RandomSequence=1
r.PathTracing.EnableEmissive=1
r.PathTracing.EnableEnvironmentMap=1
r.PathTracing.EnableCameraBackfaceCulling=1

; ==============================================================
; DENOISING SETTINGS
; ==============================================================

r.Reflections.Denoiser=0 ;alternatives: 3
r.AmbientOcclusion.Denoiser=0 ;alternatives: 1, 3
r.Shadow.Denoiser=2
r.GlobalIllumination.Denoiser=2

; ==============================================================
; OPTYMALIZACJE WYDAJNOŚCI
; ==============================================================

r.RenderTargetPool.AllowMultipleAliasingDiscardsPerFrame=1
r.PreTileTextures=1
r.UseParallelGetDynamicMeshElementsTasks=1
r.Emitter.FastPoolEnable=1
r.MultithreadedLightmapEncode=1
r.MultithreadedShadowmapEncode=1
r.HZB.BuildUseCompute=1
r.AccelPredrawBatchTime=0 ;alternatives: 13
r.PredrawBatchTime=-1 ;alternatives: 13
r.TargetPrecompileFrameTime=2.00 ;alternatives: 16

; ==============================================================
; ZARZĄDZANIE PAMIĘCIĄ
; ==============================================================

s.UnregisterComponentsTimeLimit=0.00 ;alternatives: 2.00
s.MinBulkDataSizeForAsyncLoading=0 ;alternatives: 1000
r.RenderTargetPoolMin=4096 ;alternatives: 5
LevelStreaming.UnloadNotVisibleLevelsWhenLowRAM=1
LevelStreaming.UnloadNotVisibleLevelsWhenLowRAMNormalizedPercentage=80
r.Streaming.ParallelRenderAssetsNumWorkgroups=4
r.Shadow.WholeSceneShadowCacheMb=682

; ==============================================================
; LANDSCAPE & FOLIAGE SETTINGS
; ==============================================================

r.GenerateLandscapeGIData=1
r.LandscapeUseTessellation=0
r.LandscapeMaxLOD=8
foliage.MinVertsToSplitNode=8000
grass.MaxAsyncTasks=4
grass.MaxComputeShaderTiles=64

; ==============================================================
; ASYNC COMPUTE SETTINGS
; ==============================================================

r.AsyncCompute.MaxNonComputeTaskCost=10
r.AsyncCompute.MaxNumBackgroundThreads=4
r.AsyncCompute.bAllowBackgroundThreads=1
r.AsyncCompute.bAllowAsyncDataReadback=1

; ==============================================================
; SHADER PIPELINE SZCZEGÓŁOWE
; ==============================================================

r.Shaders.OmitFramePointers=1
r.Shaders.AllowUNorm=1
r.Shaders.AllowSNorm=1
r.Shaders.KeepDebugInfo=0
r.Shaders.ExtraStats=0
r.Shaders.Optimize=1
r.Shaders.FastMath=1
r.Shaders.ZeroInitialise=1
r.Shaders.BoundsChecking=0
r.Shaders.CompilePermutations=1
r.ShaderPipelineCache.PrefersDXC=0 ;alternatives: 1
r.ShaderPipelineCache.UseShaderBinaries=0 ;alternatives: 1
r.ShaderPipelineCache.InitialFlushDelay=0 ;alternatives: 15
r.ShaderPipelineCache.MaxPrecompileTime=30 ;alternatives: 60

; ==============================================================
; COMPUTE SHADER SETTINGS
; ==============================================================

r.ComputeShader.ForceParallelTranslation=1
r.ComputeShader.WaveOperations=1
r.ComputeShader.WaveSize=32 ;alternatives: 64
r.ComputeShader.bAllowAsyncCompile=1
r.ComputeShader.AsyncCompileThreshold=10 ;alternatives: 100

; ==============================================================
; ASYNC LOADING & STREAMING
; ==============================================================

s.AsyncLoadingThreadPriority=0 ;alternatives: 1
s.AsyncLoadingThreadStackSizeKB=32 ;alternatives: 128
s.AsyncLoadingTimeLimit=2.0 ;alternatives: 5.0
s.AsyncIOBandwidthLimit=0 ;alternatives: 50
s.AsyncLoadingUseFullTimeLimit=0 ;alternatives: 1
s.AsyncLoading.MaxConcurrentResLoads=2 ;alternatives: 8, 16

; ==============================================================
; THREADING & TASK GRAPH
; ==============================================================

TaskGraph.NumWorkerThreads=0 ;alternatives: 4, 8 (0=auto)
TaskGraph.MaxWorkerThreads=8 ;alternatives: 16
TaskGraph.UseSharedProducerConsumerQueue=1
TaskGraph.AllowBulkShortTasks=1
TaskGraph.ShowPerTaskTimingInfo=0
TaskGraph.EnableProfiling=0

; ==============================================================
; FINAL SYSTEM OPTIMIZATIONS
; ==============================================================

r.ForceStaticMeshLOD=-1 ;alternatives: 0, 1
r.ForceLOD=-1 ;alternatives: 0, 1
r.SkeletalMeshLODRadiusScale=1.0 ;alternatives: 2.0
r.ViewDistanceScale=1.0 ;alternatives: 0.6, 10.0
r.Nanite.MaxPixelsPerEdge=1 ;alternatives: 2, 4
r.Nanite.ForceLOD=-1 ;alternatives: 0, 1
r.Nanite.MinimumViewDistance=0 ;alternatives: 100
r.Nanite.CullAllRenderersCVar=0 ;alternatives: 1

; ==============================================================
; FINAL FSR SETTINGS
; ==============================================================

r.FidelityFX.FSR3.HistoryFormat=0 ;alternatives: 1 (balanced), 2 (quality)
r.FidelityFX.FSR3.Sharpness=0.5 ;alternatives: 0.3, 0.7
r.FidelityFX.FSR3.AdjustMipBias=0 ;alternatives: 1
r.FidelityFX.FSR3.ReparentMode=0 ;alternatives: 1, 2

; ==============================================================
; FINAL CORE SETTINGS
; ==============================================================

[Core.System]
bUseBackgroundLevelStreaming=True
bUseFixedTimeStep=False
bSmoothFrameRate=False
MinDesiredFrameRate=60.000000
bUseThreadedPhysics=True
PhysicsUpdateDeltaTime=0.016667

[SystemSettings]
r.ForceDebugViewModes=0
r.ShaderDevelopmentMode=0
r.CompileShadersForDevelopment=0
r.ClearWithExcludeRects=2

; ==============================================================
; END OF CONFIG
; ==============================================================