; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 1
; ==============================================================

; -------------------------------------
; PODSTAWOWY RENDERING
; -------------------------------------
r.ViewDistanceScale=

- 10 (najwyższa wartość)
- 1.0 (standardowa)
- 0.6 (najniższa wartość)

r.ScreenPercentage=

- 70 (optymalizacja)
- 99 (wysoka jakość)
- 100 (pełna jakość)

r.DefaultFeature.MotionBlur=

- True
- False
- 0

r.MotionBlur.Max=0
r.MotionBlurQuality=0
r.FastBlurThreshold=

- 0
- 3

r.BloomQuality=

- 0 (wyłączone)
- 2 (średnie)
- 5 (wysokie)

r.Tonemapper.Quality=

- 1
- 2
- 5

r.Tonemapper.Sharpen=

- 0.1
- 1.6

r.TonemapperFilm=1
r.TonemapperGamma=

- 2.0
- 2.5
- Rec709

r.MaxAnisotropy=

- 8
- 16

; -------------------------------------
; OPTYMALIZACJE CPU/GPU
; -------------------------------------
r.Shaders.Optimize=1
r.Shaders.FastMath=1
r.GTSyncType=1
r.OneFrameThreadLag=

- 0 (mniejsze opóźnienie)
- 1 (lepszy performance)

r.FinishCurrentFrame=0
r.CreateShadersOnLoad=1
r.OptimizeForUAVPerformance=1
r.DontLimitOnBattery=

- 0
- 1

r.ClearWithExcludeRects=2
r.DeferSkeletalDynamicDataUpdateUntilGDME=1
r.DeferUniformBufferUpdatesUntilVisible=1
r.DoInitViewsLightingAfterPrepass=1

; -------------------------------------
; ASYNC I MULTITHREAD
; -------------------------------------
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

; -------------------------------------
; SHADER PIPELINE
; -------------------------------------
r.ShaderPipelineCache.Enabled=1
r.ShaderPipelineCache.StartupMode=

- 1
- 2
- 3

r.ShaderPipelineCache.PreOptimizeEnabled=1
r.ShaderPipelineCache.SaveUserCache=1
r.ShaderPipelineCache.BatchSize=

- 10
- 128
- 256

r.D3D.ForceDXC=

- 0
- 1

; -------------------------------------
; TEKSTUROWANIE PODSTAWOWE
; -------------------------------------
r.Streaming.PoolSize=

- -1 (auto)
- 0
- 2000
- 2048
- 4096

r.Streaming.LimitPoolSizeToVRAM=

- 0
- 1

r.VirtualTextureReducedMemory=1
r.VirtualTexture=1
r.VirtualTextures=1

r.Streaming.MaxTempMemoryAllowed=

- 682
- 2048
- 4096

r.Streaming.AmortizeCPUToGPUCopy=1

r.Streaming.MaxNumTexturesToStreamPerFrame=

- 2
- 3
- 30
- 32

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 2
; ==============================================================

; -------------------------------------
; SYSTEM CIENIOWANIA
; -------------------------------------
r.Shadow.Virtual.Enable=1
r.Shadow.Virtual.Cache=1
r.Shadow.Virtual.Cache.StaticSeparate=0
r.Shadow.Virtual.NonNaniteVSM=1
r.Shadow.Virtual.Cache.AllocateViaLRU=

- 0
- 1

r.Shadow.Virtual.Cache.KeepOnlyRequestedPages=1
r.Shadow.Virtual.ForceOnlyVirtualShadowMaps=1
r.Shadow.Virtual.TranslucentQuality=1
r.Shadow.Virtual.SMRT.RayCountLocal=8
r.Shadow.Virtual.SMRT.SamplesPerRayLocal=8
r.Shadow.Virtual.MaxPhysicalPages=4096

r.Shadow.DistanceScale=

- 0.001
- 1
- 1.5
- 2
- 4

r.Shadow.MaxResolution=

- 1024
- 2048
- 4096
- 8192

r.Shadow.CSM.MaxCascades=

- 0
- 1
- 2
- 4
- 10

r.Shadow.CSM.TransitionScale=1.0
r.Shadow.CSM.MaxNearCascades=

- 0
- 4

r.ContactShadows=

- 0
- 1

r.DFShadowQuality=

- 0
- 3

r.Shadow.CachePreshadow=1
r.Shadow.CacheWPOPrimitives=1

; -------------------------------------
; LUMEN I GLOBAL ILLUMINATION
; -------------------------------------
r.Lumen.TraceMeshSDFs=1
r.Lumen.TraceMeshSDFs.Allow=1
r.Lumen.TraceMeshSDFs.TraceDistance=

- 200
- 250

r.Lumen.Reflections.RadianceCache=1
r.Lumen.Reflections.SmoothBias=0.8
r.Lumen.Reflections.MaxRoughness=

- 0.3
- 0.5

r.Lumen.ScreenProbeGather.DownsampleFactor=

- 24
- 26

r.Lumen.ScreenProbeGather.TracingOctahedronResolution=10
r.Lumen.ScreenProbeGather.Temporal.MaxFramesAccumulated=

- 25
- 30

r.LumenScene.GPUDrivenUpdate=0
r.LumenScene.Lighting.AsyncCompute=1
r.LumenScene.SurfaceCache.CardTexelDensityScale=

- 1000
- 5000
- 10000

r.DynamicGlobalIlluminationMethod=1

; -------------------------------------
; AMBIENT OCCLUSION I REFLECTIONS
; -------------------------------------
r.AmbientOcclusion.Method=1
r.AmbientOcclusion.Compute=

- 1
- 2

r.SSGI.Enable=1
r.SSGI.HalfRes=0
r.SSGI.Quality=

- 1
- 2

r.SSR.Quality=

- 3
- 4

r.ReflectionMethod=1
r.ReflectionCapture.EnableLightFunctions=1

; -------------------------------------
; RAYTRACING
; -------------------------------------
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
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 3
; ==============================================================

; -------------------------------------
; FSR (FidelityFX Super Resolution)
; -------------------------------------
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

; -------------------------------------
; VRS (Variable Rate Shading)
; -------------------------------------
r.VRS.Enable=true
r.VRS.EnableImage=true
r.Nanite.SoftwareVRS=1
r.VRS.NaniteEmitGBuffer=1
r.VRS.BasePass=1
r.VRS.Decals=1
r.VRS.SSAO=1
r.VRS.SSR=1

; -------------------------------------
; NANITE
; -------------------------------------
r.Nanite.MaxPixelsPerEdge=2
r.Nanite.DicingRate=1
r.Nanite.FastVisBufferClear=2
r.Nanite.MaterialVisibility=1

; -------------------------------------
; POST-PROCESSING I EFEKTY
; -------------------------------------
r.PostProcessAAQuality=

- 0
- 5
- 6

r.DefaultFeature.Bloom=False
r.DefaultFeature.AmbientOcclusion=False
r.DefaultFeature.AmbientOcclusionStaticFraction=False
r.DefaultFeature.AutoExposure=False
r.DefaultFeature.LensFlare=True
r.DefaultFeature.MotionBlur=

- True
- False

r.DepthOfFieldQuality=0
r.EyeAdaptationQuality=3
r.LensFlareQuality=

- 0
- 3

r.LightShaftQuality=

- 0
- 1

r.RefractionQuality=

- 3
- 5

r.SceneColorFringeQuality=0

r.TemporalAA.Upsampling=1
r.TemporalAACatmullRom=

- 0
- 1

r.TemporalAACurrentFrameWeight=

- 0
- 0.05
- 0.25

r.TemporalAASamples=

- 4
- 32

r.TemporalAASharpness=

- 1
- 1.4

; -------------------------------------
; FOLIAGE I TRAWA
; -------------------------------------
grass.DensityScale=

- 0.25
- 2.5

grass.TickInterval=10
grass.Enable=1

foliage.DensityScale=

- 0.1
- 2.5

sg.FoliageLoadDistance=

- 2
- 4

; -------------------------------------
; LOD I MESH SETTINGS
; -------------------------------------
r.StaticMeshLODBias=

- -2
- 0

r.SkeletalMeshLODBias=

- -2
- -1
- 0

r.LandscapeLODBias=

- -1
- 0.5

r.ParticleLODBias=

- -1
- 0

r.SkinCache.Allow=true
r.SkinCache.CompileShaders=1
r.SkinCache.Mode=1

; -------------------------------------
; VOLUMETRIC EFFECTS
; -------------------------------------
r.VolumetricFog=

- 0
- 1

r.VolumetricFog.GridPixelSize=

- 0
- 4

r.VolumetricFog.GridSizeZ=

- 0
- 64

r.VolumetricFog.Jitter=1
r.VolumetricFog.HistoryMissSupersampleCount=16
r.VolumetricCloud=

- 0
- 1

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 4
; ==============================================================

; -------------------------------------
; DX12 SPECYFICZNE
; -------------------------------------
[/Script/WindowsTargetPlatform.WindowsTargetSettings]
DefaultGraphicsRHI=DefaultGraphicsRHI_DX12
+D3D12TargetedShaderFormats=PCD3D_SM6

D3D12.PSO.DiskCache=1
D3D12.PSO.DriverOptimizedDiskCache=1
D3D12.AFRSyncTemporalResources=1
D3D12.AFRUseFramePacing=1
D3D12.AsyncDeferredDeletion=1
D3D12.ForceThirtyHz=0
D3D12.InsertOuterOcclusionQuery=1
D3D12.ResidencyManagement=1
D3D12.StablePowerState=0
D3D12.SyncWithDWM=

- 0
- 1

D3D12.TexturePoolOnlyAccountStreamableTexture=1
D3D12.UseUpdateTexture3DComputeShader=

- 0
- 1

D3D12.ZeroBufferSizeInMB=

- 32
- 128

D3D12.MaximumFrameLatency=

- 0
- 2
- 3

d3d12.FastAllocator.MinPagesToRetain=

- 2
- 4

; -------------------------------------
; GARBAGE COLLECTION
; -------------------------------------
[/script/engine.garbagecollectionsettings]
gc.ActorClusteringEnabled=1
gc.AssetClusteringEnabled=1
gc.BlueprintClusteringEnabled=True
gc.CreateGCClusters=1
gc.NumRetriesBeforeForcingGC=

- 3
- 5

gc.MinDesiredObjectsPerSubTask=

- 2
- 4
- 20

gc.MaxObjectsNotConsideredByGC=32
gc.MultithreadedDestructionEnabled=1

gc.TimeBetweenPurgingPendingKillObjects=

- 15
- 30
- 60
- 300

gc.LowMemory.TimeBetweenPurgingPendingLevels=

- 15
- 60

gc.LowMemory.TimeBetweenPurgingPendingKillObjects=

- 60
- 300

gc.LowMemory.MemoryThresholdMB=

- 512
- 2048
- 4096
- 8000

; -------------------------------------
; NETWORK SETTINGS
; -------------------------------------
[/script/engine.gamenetworkmanager]
TotalNetBandwidth=128000
MaxDynamicBandwidth=16000
MinDynamicBandwidth=8000

; -------------------------------------
; ENGINE CORE SETTINGS
; -------------------------------------
[/script/engine.engine]
bEnableMouseSmoothing=0
bUseVSync=false
bSmoothFrameRate=0
bAllowMultiThreadedShaderCompile=True
FrameRateLimit=0.000000

MinDesiredFrameRate=60
SmoothedFrameRateRange=(LowerBound=(Type="ERangeBoundTypes::Inclusive",Value=60),UpperBound=(Type="ERangeBoundTypes::
Exclusive",Value=144))

; -------------------------------------
; IO SETTINGS
; -------------------------------------
s.IoDispatcherCacheSizeMB=

- 256
- 512
- 2048

s.MaxIncomingRequestsToStall=

- 1
- 10
- 24
- 100

s.AsyncLoadingTimeLimit=2.0
s.LevelStreamingActorsUpdateTimeLimit=

- 1.00
- 2.00

s.UnregisterComponentsTimeLimit=

- 0.00
- 2.00

s.AsyncLoadingUseFullTimeLimit=0
s.ContinuouslyIncrementalGCWhileLevelsPendingPurge=

- 0
- 1

s.PriorityAsyncLoadingExtraTime=10.00

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 5
; ==============================================================

; -------------------------------------
; SZCZEGÓŁOWE USTAWIENIA STREAMINGU
; -------------------------------------
[TextureStreaming]
BoostPlayerTextures=1.0
AllowStreamingLightmaps=False
r.Streaming.DropMips=

- 0
- 1
- 2

r.Streaming.HLODStrategy=

- 0
- 2

r.Streaming.UseFixedPoolSize=

- 0
- 1

r.Streaming.PoolSize.VRAMPercentageClamp=1024
r.Streaming.UseAsyncRequestsForDDC=1
r.Streaming.FullyLoadUsedTextures=

- 0
- 1

r.Streaming.DefragDynamicBounds=1
r.bForceCPUAccessToGPUSkinVerts=True
r.Streaming.FramesForFullUpdate=

- 1
- 2

r.Streaming.MinMipForSplitRequest=

- 0
- 2
- 15

r.Streaming.ScaleTexturesByGlobalMyBias=

- 0.5
- 1.0

r.Streaming.Boost=

- 1.0
- 6
- 1024

; -------------------------------------
; RHI SETTINGS
; -------------------------------------
rhi.SyncInterval=0
rhi.SyncSlackMS=0
RHI.MaximumFrameLatency=

- 0
- 3

r.RHICmdUseDeferredContexts=1
r.RHICmdMinDrawsPerParallelCmdList=

- 128
- 512

r.RHICmdMinCmdlistForParallelSubmit=4
r.RHICmdBalanceParallelLists=

- 1
- 2

r.RHICmdBalanceTranslatesAfterTasks=

- 0
- 1

r.RHICmdFlushRenderThreadTasksPrePass=1
r.RHICmdFlushRenderThreadTasksShadowPass=1
r.RHICmdFlushRenderThreadTasksBasePass=1
r.RHICmdFlushRenderThreadTasksTranslucentPass=1
r.RHICmdFlushRenderThreadTasksVelocityPass=1

; -------------------------------------
; GEOMETRY I PHYSICS
; -------------------------------------
GeometryCache.InterpolateFrames=1
GeometryCache.OffloadUpdate=1

r.GenerateMeshDistanceFields=1
r.DistanceField.MaxPerMeshResolution=128
r.AOGlobalDistanceField.MinMeshSDFRadius=

- 1
- 5
- 10

CSS.Conveyor.EnableLodOptimizations=1

; -------------------------------------
; NIAGARA I FX
; -------------------------------------
fx.Niagara.DelayScriptAsyncOptimization=false
fx.Niagara.GpuProfiling.Enabled=false
Niagara.GPUSorting.CPUToGPUThreshold=200

FX.AllowGPUParticles=1
FX.AllowAsyncTick=1
FX.EarlyScheduleAsync=1
FX.BatchAsyncBatchSize=

- 1
- 8

FX.AllowGPUSorting=1
FX.FXAllowParticleMeshLODs=1
FX.AllowCulling=

- 0
- 1

; -------------------------------------
; WATER SETTINGS
; -------------------------------------
r.Water.SingleLayer.ShadersSupportVSMFiltering=1
r.Water.SingleLayerWater.SupportCloudShadow=1
r.Water.SingleLayer.VSMFiltering=1

; -------------------------------------
; MATERIAŁY I SHADERY
; -------------------------------------
[/Script/UnrealEd.ProjectPackagingSettings]
bShareMaterialShaderCode=True
bSharedMaterialNativeLibraries=True

r.MaterialQualityLevel=

- 0
- 1

r.BasePassOutputsVelocity=0
r.CustomDepth.Order=0
r.CustomDepthEnableFastClear=1
r.GBufferFormat=

- 3
- 5

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 6
; ==============================================================

; -------------------------------------
; SYSTEM SETTINGS CORE
; -------------------------------------
[SystemSettings]
r.DBuffer=

- 0
- 1

r.HZBOcclusion=

- 0
- 1

r.VolumetricCloud.HzbCulling=0
r.FrustumCullNumWordsPerTask=32
UIManager.PauseMenuStreamingMemoryClear=2048

r.AllowOcclusionQueries=

- 0
- 1

r.AllowSubPrimitiveQueries=

- 0
- 1

r.ForceHighestMipOnUITextures=1
r.HairStrands.Cull=2
r.HairStrands.Visibility.UseFastPath=1

; -------------------------------------
; AUTO-EXPOSURE & TONE MAPPING
; -------------------------------------
r.AutoExposure.IgnoreMaterials=true
r.AutoExposure.IgnoreMaterials.UsePrecalculatedIlluminance=false
r.AutoExposure.IgnoreMaterials.LuminanceScale=0.25
r.DefaultFeature.AutoExposure.ExtendDefaultLuminanceRange=0

r.Color.Min=0.00
r.Color.Mid=0.80
r.Color.Max=1.05

; -------------------------------------
; ADAPTIVE PERFORMANCE
; -------------------------------------
r.DynamicRes.OperationMode=0
r.DynamicRes.MinScreenPercentage=100
r.OptimizeForUAVPerformance=1
a.ParallelAnimUpdate=1
r.EnableAsyncComputeVolumetricFog=1

; -------------------------------------
; ADVANCED CULLING
; -------------------------------------
r.AllowDownsampledStandardTranslucency=1
r.CullBeforeFetch=1
r.HZBOcclusion=1
r.EnableComputeBuildHZB=1
r.GBufferDiffuseSampleOcclusion=1
r.GPUScene.ParallelUpdate=1

; -------------------------------------
; SHADER COMPILATION
; -------------------------------------
r.XGEShaderCompile=1
r.XGEShaderCompile.Mode=1
r.XGEShaderCompile.Xml.BatchGroupSize=

- 256
- 64

r.XGEShaderCompile.Xml.BatchSize=

- 16
- 64

r.XGEShaderCompile.Xml.JobTimeout=0.500000

; -------------------------------------
; FAST VRAM SETTINGS
; -------------------------------------
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

; -------------------------------------
; AUDIO SETTINGS
; -------------------------------------
[/script/akaudio.aksettings]
bEnableMultiCoreRendering=True

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 7
; ==============================================================

; -------------------------------------
; LUMEN SZCZEGÓŁOWE USTAWIENIA
; -------------------------------------
[/Script/Engine.RendererSettings]
r.LumenScene.SurfaceCache.MeshCardsMinSize=

- 5
- 10

r.LumenScene.SurfaceCache.CardMinResolution=1
r.LumenScene.SurfaceCache.Compress=2
r.LumenScene.SurfaceCache.AtlasSize=1024
r.LumenScene.SurfaceCache.ResetEveryNthFrame=2400

r.Lumen.Reflections.MaxBounces=

- 3
- 6

r.Lumen.Reflections.ScreenSpaceReconstruction.NumSamples=10
r.Lumen.Reflections.Temporal.MaxFramesAccumulated=128
r.Lumen.Reflections.ScreenTraces=1
r.Lumen.Reflections.HierarchicalScreenTraces.MaxIterations=

- 200
- 400

r.Lumen.ScreenProbeGather.ImportanceSample.MinPDFToTrace=0.5
r.Lumen.ScreenProbeGather.ScreenTraces.MinimumOccupancy=1
r.Lumen.ScreenProbeGather.ShortRangeAO.ScreenSpace.SlopeCompareToleranceScale=1

r.Lumen.RadianceCache.SortTraceTiles=1
r.Lumen.RadianceCache.SpatialFilterMaxRadianceHitAngle=

- 10
- 30

r.Lumen.TranslucencyVolume.RadianceCache.FarField=1

r.LumenScene.Radiosity.MaxRayIntensity=200
r.LumenScene.Radiosity.SpatialFilterProbes.KernelSize=0.1
r.LumenScene.Heightfield.MaxTracingSteps=16
r.LumenScene.GlobalSDF.Resolution=256
r.LumenScene.GlobalSDF.ClipmapExtent=2000
r.LumenScene.GlobalSDF.CoveredExpandSurfaceScale=1
r.LumenScene.GlobalSDF.NotCoveredExpandSurfaceScale=1

; -------------------------------------
; RAY TRACING ROZSZERZONE
; -------------------------------------
r.RayTracing.Geometry.GetEachLODForHISM=0
r.RayTracing.Geometry.NiagaraSprites=0
r.RayTracing.Geometry.NiagaraRibbons=0
r.RayTracing.Geometry.NiagaraMeshes=0
r.RayTracing.Geometry.InstancedStaticMeshes.SimulationCount=

- 8
- 16

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

r.RayTracing.Translucency=0
r.RayTracing.Translucency.SamplesPerPixel=1
r.RayTracing.Translucency.Shadows=1
r.RayTracing.Translucency.MaxRefractionRays=2
r.RayTracing.Translucency.MaxRayDistance=10000
r.RayTracing.Translucency.MaxRoughness=0.2
r.RayTracing.Translucency.MinRayDistance=100
r.RayTracing.Translucency.Refraction=0

; -------------------------------------
; PATH TRACING SETTINGS
; -------------------------------------
r.PathTracing.MaxBounces=8
r.PathTracing.RandomSequence=1
r.PathTracing.EnableEmissive=1
r.PathTracing.EnableEnvironmentMap=1
r.PathTracing.EnableCameraBackfaceCulling=1

; -------------------------------------
; DENOISING SETTINGS
; -------------------------------------
r.Reflections.Denoiser=

- 0
- 3

r.AmbientOcclusion.Denoiser=

- 0
- 1
- 3

r.Shadow.Denoiser=2
r.GlobalIllumination.Denoiser=2

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 8
; ==============================================================

; -------------------------------------
; OPTYMALIZACJE WYDAJNOŚCI
; -------------------------------------
[SystemSettings]
r.RenderTargetPool.AllowMultipleAliasingDiscardsPerFrame=1
r.PreTileTextures=1
r.UseParallelGetDynamicMeshElementsTasks=1
r.Emitter.FastPoolEnable=1
r.MultithreadedLightmapEncode=1
r.MultithreadedShadowmapEncode=1
r.HZB.BuildUseCompute=1

r.AccelPredrawBatchTime=

- 0
- 13

r.PredrawBatchTime=

- -1
- 13

r.TargetPrecompileFrameTime=

- 2.00
- 16

; -------------------------------------
; ZARZĄDZANIE PAMIĘCIĄ
; -------------------------------------
s.UnregisterComponentsTimeLimit=

- 0.00
- 2.00

s.MinBulkDataSizeForAsyncLoading=

- 0
- 1000

r.RenderTargetPoolMin=

- 4096
- 5

LevelStreaming.UnloadNotVisibleLevelsWhenLowRAM=1
LevelStreaming.UnloadNotVisibleLevelsWhenLowRAMNormalizedPercentage=80
r.Streaming.ParallelRenderAssetsNumWorkgroups=4
r.Shadow.WholeSceneShadowCacheMb=682

; -------------------------------------
; OPTYMALIZACJE CACHE
; -------------------------------------
r.Cache.DrawDirectionalShadowing=1
r.Cache.UpdateEveryFrame=1
r.Cache.DrawInterpolationPoints=1
r.TransientResourceAliasing.Buffers=1
r.StencilLODMode=2

; -------------------------------------
; LANDSCAPE & FOLIAGE SETTINGS
; -------------------------------------
r.GenerateLandscapeGIData=1
r.LandscapeUseTessellation=0
r.LandscapeMaxLOD=8
foliage.MinVertsToSplitNode=8000
grass.MaxAsyncTasks=4
grass.MaxComputeShaderTiles=64

; -------------------------------------
; ASYNC COMPUTE SETTINGS
; -------------------------------------
r.AsyncCompute.MaxNonComputeTaskCost=10
r.AsyncCompute.MaxNumBackgroundThreads=4
r.AsyncCompute.bAllowBackgroundThreads=1
r.AsyncCompute.bAllowAsyncDataReadback=1

; -------------------------------------
; SHADER COMPILATION & LOADING
; -------------------------------------
r.Shaders.Synchronous=0
r.Shaders.AllowAsyncLoading=1
r.Shaders.AllowPrecompiled=1
r.ShaderPipelineCache.LazyLoadShadersWhenPSOCacheIsPresent=

- 0
- 1

r.ShaderPipelineCache.BatchTime=

- 10
- 32
- 64

r.ShaderPipelineCache.BackgroundBatchTime=

- 1
- 32

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 9
; ==============================================================

; -------------------------------------
; DX12 ROZSZERZONE USTAWIENIA
; -------------------------------------
D3D12.EnableDynamicBarriers=1
D3D12.PSO.StablePSO=1
D3D12.EvictAllResidentResourcesInBackground=true
D3D12.UseAllowTearing=1
D3D12.AdjustTexturePoolSizeBasedOnBudget=1
D3D12.Force16BitBufferGPUReads=1
D3D12.PreferD32DepthBuffer=0
D3D12.PreferDXCCompiler=1
D3D12.EnableAsyncCompileShader=1
D3D12.EnablePipelineStateCache=1
D3D12.MaxDescriptorHeapSize=1000000
D3D12.NumBufferedFrames=

- 2
- 3

D3D12.PSO.Cache=

- 0
- 1

D3D12.GPUTimeout=

- 0
- 60

; -------------------------------------
; SHADER PIPELINE SZCZEGÓŁOWE
; -------------------------------------
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

r.ShaderPipelineCache.PrefersDXC=

- 0
- 1

r.ShaderPipelineCache.UseShaderBinaries=

- 0
- 1

r.ShaderPipelineCache.InitialFlushDelay=

- 0
- 15

r.ShaderPipelineCache.MaxPrecompileTime=

- 30
- 60

; -------------------------------------
; COMPUTE SHADER SETTINGS
; -------------------------------------
r.ComputeShader.ForceParallelTranslation=1
r.ComputeShader.WaveOperations=1
r.ComputeShader.WaveSize=

- 32
- 64

r.ComputeShader.bAllowAsyncCompile=1
r.ComputeShader.AsyncCompileThreshold=

- 10
- 100

; -------------------------------------
; ASYNC LOADING & STREAMING
; -------------------------------------
s.AsyncLoadingThreadPriority=

- 0
- 1

s.AsyncLoadingThreadStackSizeKB=

- 32
- 128

s.AsyncLoadingTimeLimit=

- 2.0
- 5.0

s.AsyncIOBandwidthLimit=

- 0
- 50

s.AsyncLoadingUseFullTimeLimit=

- 0
- 1

s.AsyncLoading.MaxConcurrentResLoads=

- 2
- 8
- 16

; -------------------------------------
; THREADING & TASK GRAPH
; -------------------------------------
TaskGraph.NumWorkerThreads=

- 0 (auto)
- 4
- 8

TaskGraph.MaxWorkerThreads=

- 8
- 16

TaskGraph.UseSharedProducerConsumerQueue=1
TaskGraph.AllowBulkShortTasks=1
TaskGraph.ShowPerTaskTimingInfo=0
TaskGraph.EnableProfiling=0

; -------------------------------------
; CULLING & OCCLUSION
; -------------------------------------
r.AllowOcclusionQueries=1
r.AllowSubPrimitiveQueries=0
r.BasePassOutputsVelocity=0
r.EarlyZPass=2
r.EarlyZPassMovable=1
r.EarlyZPassOnlyMaterialMasking=1

r.FrustumCulling=

- 1
- 2

r.HZBOcclusion.MaxProcessingBatchSize=

- 8
- 16

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 10
; ==============================================================

; -------------------------------------
; ZARZĄDZANIE PAMIĘCIĄ ROZSZERZONE
; -------------------------------------
r.Streaming.PoolSize.VRAMPercentageClamp=

- 512
- 1024

r.Streaming.MaxTempMemoryAllowed=

- 682
- 2048
- 4096

r.Streaming.MaxEffectiveScreenSize=

- 0
- 1920
- 2560

r.Streaming.MaxNumTexturesToStreamInFrame=

- 2
- 10
- 15
- 32

r.Streaming.DefragDynamicBounds=1
r.Streaming.UseAllMips=1
r.Streaming.UseFixedPoolSize=1
r.Streaming.FramesForFullUpdate=

- 1
- 2

; -------------------------------------
; CACHE ROZSZERZONE
; -------------------------------------
r.CacheDrawCommands=1
r.CacheShaders=1
r.CacheComputeShaders=1
r.CachePSOs=1

r.ShaderCache.Enabled=

- 0
- 1

r.ShaderCache.XDCCacheSize=

- 256
- 512
- 1024

r.PSOCache.Enabled=

- 0
- 1

r.PSOCache.MaxEntries=

- 1000
- 2000
- 4000

; -------------------------------------
; POOL MANAGEMENT
; -------------------------------------
r.RenderTargetPool.Events=0
r.RenderTargetPool.MaxTimeInPool=

- 1
- 5

r.RenderTargetPool.MinNumberOfWidthsToAlloc=

- 8
- 16

r.RenderTargetPool.PreallocateTargets=

- 0
- 1

; -------------------------------------
; BUFFER MANAGEMENT
; -------------------------------------
r.BufferVisualizationDumpFramesAsHDR=0
r.TranslucentLightingVolume.InnerDistance=

- 64
- 128

r.TranslucentLightingVolume.OuterDistance=

- 128
- 256

r.MaxGPUParticlesSpawnedPerFrame=

- 512
- 1024

; -------------------------------------
; MEMORY TRACKING
; -------------------------------------
r.TrackRenderTargetPoolMemory=0
r.TrackShaderPoolMemory=0
r.TrackTexturePoolMemory=0
r.TrackPrimitivePoolMemory=0

Memory.EnablePurgeRequests=1
Memory.IncrementalPurgeMaxSizeThreshold=

- 10
- 100

Memory.PurgeSleepTimeMsec=

- 0
- 100

; -------------------------------------
; POOLING & ALLOCATION
; -------------------------------------
r.RenderTargetPoolMin=

- 300
- 400
- 512

r.SceneRenderTargetResizeMethod=

- 0
- 2

r.SeparateTranslucencyAutoDownsample=

- 0
- 1

r.CustomDepthTemporalAAJitter=

- 0
- 1

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 11
; ==============================================================

; -------------------------------------
; SZCZEGÓŁOWE USTAWIENIA RENDEROWANIA
; -------------------------------------
r.DetailMode=

- 0
- 1
- 2

r.RefractionQuality=

- 0
- 2
- 5

r.SSR.Quality=

- 0
- 3
- 4

r.SSR.MaxRoughness=

- -1
- 0.8
- 1.0

r.SSGI.Quality=

- 1
- 2
- 4

r.SSS.Quality=

- 0
- 1
- 2

r.SSS.Scale=

- 0.5
- 1.0

r.SSS.SampleSet=

- 0
- 2

r.SSS.HalfRes=

- 0
- 1

; -------------------------------------
; ZAAWANSOWANE USTAWIENIA OŚWIETLENIA
; -------------------------------------
r.LightMaxDrawDistanceScale=

- 0
- 1
- 2

r.LightFunctionQuality=

- 0
- 1
- 3

r.LightShaftQuality=

- 0
- 1
- 4

r.LightShaftDownSampleFactor=

- 2
- 4

r.SkyLight.RealTimeReflectionCapture=

- 0
- 1

r.DynamicGlobalIlluminationMethod=

- 0
- 1
- 2

; -------------------------------------
; AMBIENT OCCLUSION SZCZEGÓŁOWE
; -------------------------------------
r.AmbientOcclusion.Compute=

- 0
- 1
- 2

r.AmbientOcclusion.Method=

- 0
- 1
- 2

r.AOAsyncBuildQueue=1
r.AOComputeShaderNormalCalculation=1
r.AOMaxViewDistance=16384
r.AOMinLevel=0
r.AOQuality=

- 0
- 1
- 2

r.AOScatterInterpolation=1
r.AOSpecularOcclusionMode=

- 0
- 1
- 2

; -------------------------------------
; TEMPORAL AA SZCZEGÓŁOWE
; -------------------------------------
r.TemporalAA.Algorithm=

- 0
- 1
- 2

r.TemporalAA.Upsampling=

- 0
- 1

r.TemporalAA.FilterSize=

- 0.25
- 1.0

r.TemporalAA.HistoryWeight=

- 0.05
- 0.95

r.TemporalAA.MinFraction=

- 0.1
- 0.9

r.TSR.ShadingRaysPerPixel=

- 1
- 2
- 4

r.TSR.History.FramesForFullRes=

- 4
- 8
- 16

; -------------------------------------
; ADVANCED RENDERING FEATURES
; -------------------------------------
r.CustomDepth.Stencil=1
r.CustomDepth.Order=0
r.CustomDepth.Encoding=

- 0
- 1

r.ClearSceneMethod=

- 0
- 1

r.BasePassForceOutputsVelocity=

- 0
- 1

r.SelectiveBasePassOutputs=

- 0
- 1

r.DeferredShadingCSMExponent=

- 1
- 2
- 4

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 12
; ==============================================================

; -------------------------------------
; OPTYMALIZACJE WYDAJNOŚCI ROZSZERZONE
; -------------------------------------
r.ParallelGatherLighting=1
r.ParallelTranslucency=1
r.ParallelBasePass=1
r.ParallelInitViews=1
r.ParallelInitViewsRefCount=1

r.RHICmdWidth=

- 8
- 12
- 16

r.RHICmdUseDeferredContexts=1
r.RHICmdUseParallelAlgorithms=1
r.RHICmdUseThread=1

r.DynamicRes.OperationMode=

- 0
- 1
- 2

r.DynamicRes.MinScreenPercentage=

- 50
- 70
- 100

; -------------------------------------
; DEBUG I PROFILING
; -------------------------------------
r.ProfileGPU=0
r.ProfileGPU.Pattern=""
r.ProfileGPU.AssetSummary=0
r.ProfileGPU.Sort=1
r.ProfileGPU.MaxHistory=

- 30
- 60

r.ShaderPrintEnable=0
r.ShaderPrintUniform=0
r.ShaderPrintStats=0

; -------------------------------------
; EXPERIMENTAL FEATURES
; -------------------------------------
r.experimental.Nanite.EnableMeshes=1
r.experimental.Nanite.EnableStreaming=1
r.experimental.Nanite.MaxPixelsPerEdge=

- 2
- 4
- 8

r.experimental.GenerateMeshDistanceFields=

- 0
- 1

r.experimental.DiscardUnusedQuality=

- 0
- 1

; -------------------------------------
; MESH OPTIMIZATION
; -------------------------------------
r.SkeletalMesh.MinLodLevel=

- 0
- 1

r.SkeletalMesh.MaxBonesPerChunk=

- 85
- 128

r.StaticMesh.MaxLODLevel=

- 4
- 8

r.StaticMesh.MinMobileHardwareTexCoordCount=

- 1
- 2
- 3

; -------------------------------------
; RENDERING THREAD CONTROL
; -------------------------------------
r.RenderThread.Mode=

- 0 (disabled)
- 1 (single threaded)
- 2 (multi threaded)

r.RenderThread.NumWorkerThreads=

- 1
- 2
- 4

r.RenderThread.FrameOffsets=

- 0
- 1

r.ThreadedRendering=

- 0
- 1

r.ThreadedShaderCompile=

- 0
- 1

; -------------------------------------
; SYSTEM MONITORING
; -------------------------------------
r.GPUCrashDebugging=0
r.GPUHitchDebug=0
r.GPUHitchThreshold=

- 100
- 1000

r.MemoryLoadStatsUpdateInterval=

- 0.5
- 1.0

r.VSync.MonitorSyncInterval=

- 0
- 1
- 2

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 13
; ==============================================================

; -------------------------------------
; ZAAWANSOWANE USTAWIENIA GRAFICZNE
; -------------------------------------
r.GenerateLandscapeGIData=

- 0
- 1

r.AllowStaticLighting=

- 0
- 1

r.AllowGlobalClipPlane=1
r.DiscardUnusedQuality=1
r.Mobile.TonemapperFilm=1
r.Mobile.EnableStaticAndCSMShadowReceivers=1

r.MobileMSAA=

- 1
- 2
- 4

r.NGX.DLSS.Sharpness=

- 0.3
- 0.5
- 0.7

; -------------------------------------
; SYSTEM DISTANCE FIELDS
; -------------------------------------
r.DistanceFields.DefaultQuality=

- 1
- 2
- 3

r.DistanceFields.MaxPerMeshResolution=

- 64
- 128
- 256

r.DistanceFields.ParallelAtlasUpdate=

- 0
- 1

r.DistanceFields.AtlasSizeX=

- 512
- 1024
- 2048

r.DistanceFields.AtlasSizeY=

- 512
- 1024
- 2048

; -------------------------------------
; GLOBAL ILLUMINATION SETTINGS
; -------------------------------------
r.LPV.RSMResolution=

- 256
- 512
- 1024

r.LPV.IntensityBoost=

- 1.0
- 2.0
- 3.0

r.LPV.GeometryVolumeBias=

- 0.1
- 0.5
- 1.0

r.LPV.EmissiveMultiplier=

- 1.0
- 1.5
- 2.0

; -------------------------------------
; SYSTEM OPTIMIZATIONS
; -------------------------------------
r.RenderTargetPoolMin=

- 300
- 400
- 512

r.SceneRenderTargetResizeMethod=

- 0
- 1
- 2

r.SeparateTranslucencyAutoDownsample=

- 0
- 1

r.TranslucencyVolumeBlur=

- 0
- 1

r.TranslucencyLightingVolumeDim=

- 16
- 32
- 64

; -------------------------------------
; PHYSICS & ANIMATION SETTINGS
; -------------------------------------
p.RigidBody.MaxAngularVelocity=

- 100
- 500
- 1000

p.RigidBody.MaxDepenetrationVelocity=

- 100
- 1000

a.URO.DisableInterpolation=

- 0
- 1

a.URO.ForceInterpolation=

- 0
- 1

; -------------------------------------
; ADVANCED SYSTEM FEATURES
; -------------------------------------
s.ForceGCAfterLevelStreamedOut=

- 0
- 1

s.ContinuouslyIncrementalGCWhileLevelsPendingPurge=

- 0
- 1

s.MaxIncomingRequestsToStall=

- 10
- 24
- 100

s.AsyncLoadingThreadEnabled=

- 0
- 1

; -------------------------------------
; VERTEX DEFORMATION
; -------------------------------------
r.GPUSkin.Limit2BoneInfluences=

- 0
- 1

r.GPUSkin.UnlimitedBoneInfluences=

- 0
- 1

r.SkinCache.BlendLimit=

- 1
- 2
- 4

r.SkinCache.BehaviorNonMatchingLODs=

- 0
- 1
- 2

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 14
; ==============================================================

; -------------------------------------
; DODATKOWE OPTYMALIZACJE SYSTEMOWE
; -------------------------------------
r.RDG.AsyncCompute=1
r.Bloom.AsyncCompute=1
r.D3D12.AllowAsyncCompute=1
r.DFShadowAsyncCompute=1
r.EnableAsyncComputeTranslucencyLightingVolumeClear=1
r.Lumen.AsyncCompute=1
r.Nanite.Streaming.AsyncCompute=1
r.SceneDepthHZBAsyncCompute=1
r.SkyAtmosphereASyncCompute=1
r.SparseVolumeTexture.Streaming.AsyncCompute=1
r.TSR.AsyncCompute=3
r.VolumetricRenderTarget.PreferAsyncCompute=1

; -------------------------------------
; ROZSZERZONE USTAWIENIA PAMIĘCI
; -------------------------------------
r.Streaming.PoolSize.VRAMPercentageClamp=

- 512
- 1024
- 2048

r.Streaming.MaxEffectiveScreenSize=

- 0
- 1920
- 2560

r.Streaming.DefragDynamicBounds=1
r.Streaming.UseAllMips=1
r.Streaming.UseFixedPoolSize=1

; -------------------------------------
; SHADER COMPILATION EXTENDED
; -------------------------------------
r.Shaders.Optimize=1
r.Shaders.FastMath=1
r.ShaderPipelineCache.StartupMode=

- 1
- 2
- 3

r.ShaderPipelineCache.BatchSize=

- 64
- 128
- 256

r.ShaderPipelineCache.LazyLoadShadersWhenPSOCacheIsPresent=

- 0
- 1

; -------------------------------------
; RENDERING THREAD MANAGEMENT
; -------------------------------------
r.RHICmdMinDrawsPerParallelCmdList=

- 128
- 256
- 512

r.RHICmdMinCmdlistForParallelSubmit=

- 2
- 4
- 8

r.RHICmdBalanceParallelLists=

- 1
- 2

; -------------------------------------
; SYSTEM PERFORMANCE MONITORING
; -------------------------------------
r.ProfileGPU.MaxHistory=

- 30
- 60
- 120

r.ProfileGPU.Sort=

- 0
- 1

r.GPUHitchThreshold=

- 100
- 500
- 1000

; -------------------------------------
; ADVANCED RENDERING FEATURES
; -------------------------------------
r.CustomDepth.Encoding=

- 0
- 1

r.BasePassForceOutputsVelocity=

- 0
- 1

r.SelectiveBasePassOutputs=

- 0
- 1

; ==============================================================
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 15
; ==============================================================

; -------------------------------------
; ADVANCED MATERIAL SETTINGS
; -------------------------------------
r.MaterialQualityLevel=

- 0 (low)
- 1 (medium)
- 2 (high)

r.BasePassOutputsVelocity=

- 0
- 1

r.CacheShaders=

- 0
- 1

r.CompileShadersForDevelopment=

- 0
- 1

; -------------------------------------
; LANDSCAPE & VEGETATION SETTINGS
; -------------------------------------
grass.Enable=1
grass.DensityScale=

- 0.25
- 2.5

foliage.DensityScale=

- 0.1
- 2.5

foliage.LODDistanceScale=2
r.LandscapeUseOptionalLODs=1
r.LandscapeLODForcedDistanceOffset=0
r.LandscapeLODBiasMultiplier=1.0

; -------------------------------------
; ADVANCED TEXTURE SETTINGS
; -------------------------------------
[TextureStreaming]
r.Streaming.MipBias=

- 0
- 1
- 2

r.Streaming.UseAllMips=

- 0
- 1

r.Streaming.UseMaterialData=1
r.Streaming.UseNewMetrics=1
r.Streaming.UsePerTextureBias=1

; -------------------------------------
; EXTENDED SHADOW SETTINGS
; -------------------------------------
r.Shadow.DistanceScale=

- 0.001
- 1.0
- 1.5
- 2.0

r.Shadow.RadiusThreshold=

- 0.1
- 0.5

r.Shadow.CSM.TransitionScale=

- 0
- 1.0

r.Shadow.MaxCSMResolution=

- 1024
- 2048
- 4096

; -------------------------------------
; ADDITIONAL PERFORMANCE SETTINGS
; -------------------------------------
[SystemSettings]
r.ForceStaticMeshLOD=

- -1
- 0
- 1

r.ForceLOD=

- -1
- 0
- 1

r.SkeletalMeshLODRadiusScale=

- 1.0
- 2.0

r.ViewDistanceScale=

- 0.6
- 1.0
- 10.0

; -------------------------------------
; EXTENDED NANITE SETTINGS
; -------------------------------------
r.Nanite.MaxPixelsPerEdge=

- 1
- 2
- 4

r.Nanite.ForceLOD=

- -1
- 0
- 1

r.Nanite.MinimumViewDistance=

- 0
- 100

r.Nanite.CullAllRenderersCVar=

- 0
- 1

; -------------------------------------
; EXTENDED FSR SETTINGS
; -------------------------------------
r.FidelityFX.FSR3.HistoryFormat=

- 0 (Performance)
- 1 (balanced)
- 2 (quality)

r.FidelityFX.FSR3.Sharpness=

- 0.3
- 0.5
- 0.7

r.FidelityFX.FSR3.AdjustMipBias=

- 0
- 1

r.FidelityFX.FSR3.ReparentMode=

- 0
- 1
- 2

; -------------------------------------
; FINAL SYSTEM OPTIMIZATIONS
; -------------------------------------
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
; PEŁNA ANALIZA KONFIGURACJI SILNIKA - CZĘŚĆ 16 (KOŃCOWA)
; ==============================================================

; -------------------------------------
; FINAL NETWORK SETTINGS
; -------------------------------------
[/Script/Engine.NetDriver]
NetServerMaxTickRate=60
LanServerMaxTickRate=120
MaxNetTickRate=120
MaxInternetClientRate=104857600
MaxClientRate=104857600
InitialConnectTimeout=30.0
ConnectionTimeout=30.0
TimeoutMultiplierForUnoptimizedBuilds=1
SpawnPrioritySeconds=1.0
RelevantTimeout=5.0
KeepAliveTime=0.2
InitialDelay=0.5
MaxClientRate=15000
MaxInternetClientRate=10000

; -------------------------------------
; FINAL ENGINE SETTINGS
; -------------------------------------
[/Script/Engine.Engine]
bUseFixedFrameRate=False
bSmoothFrameRate=False
SmoothedFrameRateRange=(LowerBound=(Type="ERangeBoundTypes::Inclusive",Value=60),UpperBound=(Type="ERangeBoundTypes::
Exclusive",Value=144))
MinDesiredFrameRate=60.000000
bEnableClientVisibilityTracking=True
bUseBackgroundLevelStreaming=True
AsyncLoadingTimeLimit=5.000000
PriorityAsyncLoadingExtraTime=15.000000
LevelStreamingActorsUpdateTimeLimit=5.000000
bPauseOnLossOfFocus=False
MaximumLoopIterationCount=1000000
NumPawnsAllowedToBeSpawnedInAFrame=20

; -------------------------------------
; FINAL PHYSICS SETTINGS
; -------------------------------------
[/Script/Engine.PhysicsSettings]
DefaultGravityZ=-980.000000
DefaultTerminalVelocity=4000.000000
DefaultFluidFriction=0.300000
SimulateScratchMemorySize=262144
RagdollAggregateThreshold=4
TriangleMeshTriangleMinAreaThreshold=5.000000
bEnableShapeSharing=False
bEnablePCM=True
bEnableStabilization=False
bWarnMissingLocks=True
bEnable2DPhysics=False
PhysicErrorCorrection=(
PingExtrapolation=0.100000,PingLimit=100.000000,ErrorPerLinearDifference=1.000000,ErrorPerAngularDifference=1.000000,MaxRestoredStateError=1.000000,MaxLinearHardSnapDistance=400.000000,PositionLerp=0.000000,AngleLerp=0.400000,LinearVelocityCoefficient=100.000000,AngularVelocityCoefficient=10.000000,ErrorAccumulationSeconds=0.500000,ErrorAccumulationDistanceSq=15.000000,ErrorAccumulationSimilarity=100.000000)
LockedAxis=Invalid
DefaultDegreesOfFreedom=Full3D
BounceThresholdVelocity=200.000000
FrictionCombineMode=Average
RestitutionCombineMode=Average
MaxAngularVelocity=3600.000000
MaxDepenetrationVelocity=0.000000
ContactOffsetMultiplier=0.020000
MinContactOffset=2.000000
MaxContactOffset=8.000000
bSimulateSkeletalMeshOnDedicatedServer=True
DefaultShapeComplexity=CTF_UseSimpleAndComplex
bDefaultHasComplexCollision=True
bSuppressFaceRemapTable=False
bSupportUVFromHitResults=False
bDisableActiveActors=False
bDisableKinematicStaticPairs=False
bDisableKinematicKinematicPairs=False
bDisableCCD=False
bEnableEnhancedDeterminism=False
AnimPhysicsMinDeltaTime=0.000000
bSimulateAnimPhysicsAfterReset=False
MaxPhysicsDeltaTime=0.033333
bSubstepping=False
bSubsteppingAsync=False
MaxSubstepDeltaTime=0.016667
MaxSubsteps=6
SyncSceneSmoothingFactor=0.000000
InitialAverageFrameRate=0.016667
PhysXTreeRebuildRate=10
DefaultBroadphaseSettings=(bUseMBPOnClient=False,bUseMBPOnServer=False,bUseMBPOuterBounds=False,MBPBounds=(Min=(
X=0.000000,Y=0.000000,Z=0.000000),Max=(X=0.000000,Y=0.000000,Z=0.000000),IsValid=0),MBPOuterBounds=(Min=(
X=0.000000,Y=0.000000,Z=0.000000),Max=(X=0.000000,Y=0.000000,Z=0.000000),IsValid=0),MBPNumSubdivs=2)
ChaosSettings=(
DefaultThreadingModel=DedicatedThread,DedicatedThreadTickMode=VariableCappedWithTarget,DedicatedThreadBufferMode=Double)

; ==============================================================
; KONIEC ANALIZY KONFIGURACJI SILNIKA
; ==============================================================