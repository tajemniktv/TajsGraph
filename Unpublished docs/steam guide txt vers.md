[h2] These are others tweaks, that are yet to be tested more extensively and organized with the construct of this
guide [h2]

[h3] Full Lumen [h3]

[code]
r.Lumen.ScreenProbeGather.ReferenceMode=1
[code]
It has extreme performance impact - strips Lumen out of most optimizations it has and traces 1024 uniform rays per probe
with no filtering, importance sampling or radiance cache

[h3] Cinematic Global Illumination [h3]

[code]
sg.Globalillumination=4
[code]

[h3] Reduce noise and flickering [h3]

[code]
r.Lumen.ScreenProbeGather.Temporal.MaxFramesAccumulated=25
[code]

[h3] Tracing Mesh SDFs [h3]

[code]
r.Lumen.TraceMeshSDFs=1
r.Lumen.TraceMeshSDFs.TraceDistance=250
[code]

[h3] Disable Film Grain [h3]

[code]
r.FilmGrain=0
[code]

[h3] Increase GI resolution [h3]

[code]
r.Lumen.ScreenProbeGather.DownsampleFactor=24
[code]

[h3] Increase HZB iterations [h3]

[code]
r.Lumen.ScreenProbeGather.ScreenTraces.HZBTraversal.MaxIterations=200
[code]

[h3] Enable front layer and transluecents [h3]

[code]
r.Lumen.TranslucencyReflections.FrontLayer.Enable=1
[code]

[h3] Radiance Cache for Reflections [h3]

[code]
r.Lumen.Reflections.RadianceCache=1
[code]

[h3] TBD [h3]
[code]
r.Shadow.DistanceScale=2
[code]

[h3] Improve Shadows resolution [h3]
[code]
r.Shadow.MaxCSMResolution=2048
[code]
2k removes flickering but doesn't cause frames to go down to 20 lol

[code]
r.Shadow.MaxResolution=1024
[code]
Game doesn't seem to be using non cascaded a lot

[h3] View disstance [h3]

[code]
r.ViewDistanceScale=
[code]

[h3] is it even working and is it worth it [h3]
[code]
r.AmbientOcclusion.Method=1
r.GTAO.Downsample=1
[code]

[code]
r.SSR.Quality=4
r.SSR
[code]

[code]
D3D12.AdjustTexturePoolSizeBasedOnBudget=1
[code]
(slows down loading of textures when low on budget)

[code]
r.DFShadow.TwoSidedMeshDistanceBiasScale=0 (seems like it's being overwritten by something)
[code]
[code]
r.DFShadowQuality=3
[code]
[code]
r.GBufferDiffuseSampleOcclusion=1
[code]
We could touch on shadows soonTM, but that might be a big performance hit lol
[code]
r.Shadow.CSM.MaxCascades=4
r.Shadow.CSM.MaxNearCascades=4
[code]
Godly Autoexposure
[code]
r.AutoExposure.IgnoreMaterials=true
r.AutoExposure.IgnoreMaterials.UsePrecalculatedIlluminance=false
r.AutoExposure.IgnoreMaterials.LuminanceScale=0.25
[code]
[code]
r.Lumen.SkylightLeaking.Roughness=0.5
[code]
[code]
fx.Niagara.DelayScriptAsyncOptimization=false
[code]
[code]
r.Shadow.Virtual.Enable=1
r.Shadow.Virtual.Cache=1
[code]

[code]
D3D12.EvictAllResidentResourcesInBackground=true
[code]

[code]
fx.Niagara.GpuProfiling.Enabled=false
[code]

[code]
r.LumenScene.GPUDrivenUpdate=1
[code]

Compat.UseDXT5NormalMaps=1
[code]

[code]
r.D3D.Force.DXC=1
r.Shaders.ForceDXC=1
r.OpenGL.ForceDXC=1
[code]

[code][strike]
D3D12.PSO.DiskCache=1
D3D12.PSO.DriverOptimizedDiskCache=1
D3D12.PSOPrecache.KeepLowLevel=1 ()
[strike][code]
(crashes)

[code]
D3D12.PSOPrecache.KeepLowLevel=1 ()
[code]

[code]
r.Shadow.CSMCaching=1
[code]

[code]
r.DFShadowQuality=0 (disables distance field shadows, so requires another shading method like virtual)
[code]

[code]
r.Shadow.Virtual.Cache.AllocateViaLRU=1
[code]

[code]
r.Shadow.Virtual.UseFarShadowCulling=0
[code]

[code]
r.Shadow.Virtual.Cache.KeepOnlyRequestedPages=0
[code]

[code]
r.GBufferFormat=3 (causes some black shit, 2 might work but prolly fallbacks to 1)
[code]

[code]
r.HFShadowQuality=0
[code]

[code]
r.Shadow.CSM.MaxCascades=0
[code]

[code]
10k for cardtexelscale
[code]

[code]
r.LumenScene.SurfaceCache.FarField.CardTexelDensity=0.1 (default 0.001)
[code]

[code]
r.LumenScene.SurfaceCache.MeshCardsMinSize=5
[code]

[code]
grass.DensityScale=2.5
[code]

[code]
grass.TickInterval=10
[code]

[code]
r.ViewDistanceScale=1
[code]

[code]
r.Shadow.Virtual.Cache.StaticSeparate=0
[code]

[code]
r.Shadow.Virtual.ForceOnlyVirtualShadowMaps=1
[code]

[code]
r.Shadow.Use.OctreeForCulling=0
[code]

[code]
r.ContactShadows=0
[code]

[code]
r.Nanite.MaterialVisibility=1
[code]

[code]
r.Shadow.Virtual.Nanite.MaterialVisibility=1
[code]

[code]
r.VirtualTextureReducedMemory=1
[code]

[code]
sg.FoliageLoadDistance=2
[code]

[code]
r.DistanceFieldShadowing=0
[code]

[code]
r.HeighFieldShadowing=0
[code]

[code]
r.Shadow.Virtual.Cache.KeepOnlyRequestedPages=0
[code]

29.09 1918
[code]
r.Streaming.LimitPoolSizeToVRAM=1
D3D12.AdjustTexturePoolSizeBasedOnBudget=1
d3d12.FastAllocator.MinPagesToRetain=2
D3D12.InsertOuterOcclusionQuery=1
D3D12.UseUpdateTexture3DComputeShader=1
r.FidelityFX.FSR3.QuantizeInternalTextures=1
[code]

[code]

[code]

[h3] Shouldn't work, unless... [h3]
[code]
r.Lumen.HardwareRayTracing=1
r.Lumen.HardwareRayTracing.LightingMode=1
[code]
In theory it should enable hardware raytracing on supported GPUs. I'm not sure how effective enabling it simply via cvar
is.
Hardware raytracing is more expensive, so if it actually works on some architectures, expect a performance hit!

note to self check out fastvram

r.RayTracing=True
r.RayTracing.Shadows=True
r.RayTracing.UseTextureLod=True
r.Lumen.HardwareRayTracing=True
r.Lumen.HardwareRayTracing.LightingMode=2
bEnableRayTracing=true

[h1] Stop rendering one frame behind [/h1]
[code]
r.OneFrameThreadLag=0
[/code]

[h1] Make things work in async [/h1]
[code]
FX.BatchAsync=1
r.LumenScene.Lighting.AsyncCompute=1
r.Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.Reflections.AsyncCompute=1
tick.AllowAsyncTickDispatch=1
tick.AllowAsyncTickCleanup=1
r.DFShadowAsyncCompute=1
[/code]
Performance benefit might either not be visible, or it might cause a low to medium performance hit on some
architectures. Especially on NVIDIAs GPUs. Honestly, it's "try it on your own". When tweaking, I use my own hardware for
reference, so your mileage may vary!

[h1] DX12 Outer Occlusion [/h1]
Could improve performance on some architectures
[code]
D3D12.InsertOuterOcclusionQuery=1
[/code]

[h1] FSR3+FG [/h1]
These should fix any "white screen + black dots artifacts", console flickering or other issues related to FSR being used
as upscaling method.

[h2] FSR [/h2]

[code]
r.FidelityFX.FSR3.DeDither=1
r.FidelityFX.FSR3.CreateReactiveMask=1
r.FidelityFX.FSR3.UseSSRExperimentalDenoiser=1
r.FidelityFX.FSR3.AutoExposure=1
r.FidelityFX.FSR3.Sharpness=0.7
r.FidelityFX.FSR3.QualityMode=1
[/code]

[h2] Frame Gen [/h2]

[code]
r.FidelityFX.FI.Enabled=1
r.FidelityFX.FI.OverrideSwapChainDX12=1
r.FidelityFX.FI.AllowAsyncWorkloads=1
[/code]
I'm keeping first line for anyone who wishes to use only FSR as upscaling without FG, as disabling it is as simple as
swapping out 1 for 0.

[h1] DLSS [/h1]
This could be TBD, only way for me to access a RTX card is via GeForce Now (which doesn't work for me, so... :)), but
also setting up hundreds of commands on it is... Less than desirable.
FSR feels superior, sorry NVIDIA.

It's obviously pretty hard to "categorize" them all as either x tweaks or y tweaks, but I'm gonna try to include the
most visual changing ones here.

This chapter is gonna be dedicated to documentation ([b]VERY[/b] WIP), so if you wish, you could go for "DIY" or
changing some settings on your own.

[h1] Bring back old sign light! [/h1]
[code]
r.AOGlobalDistanceField.MinMeshSDFRadius=10
r.LumenScene.SurfaceCache.CardTexelDensityScale=1000
[/code]

[h1] Mmmm, crispy reflections [/h1]
[code]
r.Lumen.Reflections.SmoothBias=0.8
[/code]

[h1] Aliasing [/h1]
[code]
r.MaxAnisotropy=16
r.TemporalAACurrentFrameWeight=0
r.TemporalAASamples=32
r.PostProcessAAQuality=5
[/code]

[h1] AO [/h1]
[code]
[strike]r.Lumen.DiffuseIndirect.SSAO=1
r.Lumen.ScreenProbeGather.ShortRangeAO=0 [/strike][/code]
(This is terrible, don't use this)

[h1] GI [/h1]
[code]
r.DynamicGlobalIlluminationMethod=1
[/code]

UPDATE 0.3
[code]
[SystemSettings]
r.AOGlobalDistanceField.MinMeshSDFRadius=5
r.AmbientOcclusion.Method=1
r.GTAO.Downsample=1
r.Lumen.Reflections.SmoothBias=0.8
r.Lumen.Reflections.RadianceCache=1
r.Lumen.ScreenProbeGather.TracingOctahedronResolution=10
r.Lumen.ScreenProbeGather.Temporal.MaxFramesAccumulated=30
r.Lumen.ScreenProbeGather.DownsampleFactor=24
r.Lumen.ScreenProbeGather.ScreenTraces.HZBTraversal.MaxIterations=200
r.Lumen.TraceMeshSDFs=1
r.Lumen.TraceMeshSDFs.TraceDistance=250
r.Lumen.TranslucencyReflections.FrontLayer.Enable=1
r.LumenScene.SurfaceCache.CardTexelDensityScale=1000
r.LumenScene.Lighting.AsyncCompute=1
r.Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.Reflections.AsyncCompute=1
r.Shadow.MaxCSMResolution=2048
r.Shadow.MaxResolution=1024
r.Shadow.DistanceScale=2
r.DynamicGlobalIlluminationMethod=1
r.FilmGrain=0
r.SSR.Quality=4
r.FastVram.SSR=1
r.FidelityFX.FSR3.CreateReactiveMask=1
r.FidelityFX.FSR3.UseSSRExperimentalDenoiser=1
r.FidelityFX.FSR3.DeDither=1
r.FidelityFX.FSR3.Sharpness=0.7
r.FidelityFX.FSR3.AutoExposure=1
r.FidelityFX.FI.Enabled=0
r.FidelityFX.FI.OverrideSwapChainDX12=1
r.FidelityFX.FI.AllowAsyncWorkloads=1
r.GBufferDiffuseSampleOcclusion=1
r.ViewDistanceScale=1
tick.AllowAsyncTickDispatch=1
tick.AllowAsyncTickCleanup=1
FX.BatchAsync=1
Compat.UseDXT5NormalMaps=1
D3D12.InsertOuterOcclusionQuery=1
r.DFShadow.TwoSidedMeshDistanceBiasScale=0
r.DFShadowAsyncCompute=1
r.DFShadowQuality=3
[/code]

UPDATE 0.4p
[code]
[SystemSettings]
r.AOGlobalDistanceField.MinMeshSDFRadius=5
r.AmbientOcclusion.Method=1
r.GTAO.Downsample=1
r.Lumen.Reflections.SmoothBias=0.8
r.Lumen.Reflections.RadianceCache=1
r.Lumen.ScreenProbeGather.TracingOctahedronResolution=10
r.Lumen.ScreenProbeGather.Temporal.MaxFramesAccumulated=30
r.Lumen.ScreenProbeGather.DownsampleFactor=26
r.Lumen.ScreenProbeGather.ScreenTraces.HZBTraversal.MaxIterations=200
r.Lumen.TraceMeshSDFs=1
r.Lumen.TraceMeshSDFs.TraceDistance=250
r.Lumen.TranslucencyReflections.FrontLayer.Enable=1
r.LumenScene.SurfaceCache.CardTexelDensityScale=1000
r.LumenScene.Lighting.AsyncCompute=1
r.Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.Reflections.AsyncCompute=1
r.Shadow.MaxCSMResolution=1536
r.Shadow.DistanceScale=2
r.FilmGrain=0
r.SSR.Quality=3
r.FidelityFX.FSR3.CreateReactiveMask=1
r.FidelityFX.FSR3.UseSSRExperimentalDenoiser=1
r.FidelityFX.FSR3.DeDither=1
r.FidelityFX.FSR3.Sharpness=0.7
r.FidelityFX.FSR3.AutoExposure=1
r.FidelityFX.FI.Enabled=0
r.FidelityFX.FI.OverrideSwapChainDX12=1
r.FidelityFX.FI.AllowAsyncWorkloads=1
r.ViewDistanceScale=0.7
tick.AllowAsyncTickDispatch=1
tick.AllowAsyncTickCleanup=1
FX.BatchAsync=1
r.DFShadow.TwoSidedMeshDistanceBiasScale=0
r.DFShadowAsyncCompute=1
r.DFShadowQuality=2
[/code]

Update V0.5
[code]
[SystemSettings]
r.AOGlobalDistanceField.MinMeshSDFRadius=5
r.Lumen.Reflections.SmoothBias=0.8
r.Lumen.Reflections.RadianceCache=1
r.Lumen.ScreenProbeGather.TracingOctahedronResolution=10
r.Lumen.ScreenProbeGather.Temporal.MaxFramesAccumulated=30
r.Lumen.ScreenProbeGather.DownsampleFactor=26
r.Lumen.ScreenProbeGather.ScreenTraces.HZBTraversal.MaxIterations=100
r.Lumen.TraceMeshSDFs=1
r.Lumen.TraceMeshSDFs.TraceDistance=225
r.Lumen.TranslucencyReflections.FrontLayer.Enable=1
r.LumenScene.SurfaceCache.CardTexelDensityScale=1000
r.LumenScene.Lighting.AsyncCompute=1
r.Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.Reflections.AsyncCompute=1
r.Shadow.MaxCSMResolution=1536
r.Shadow.DistanceScale=2
r.SSR.Quality=3
r.FidelityFX.FSR3.CreateReactiveMask=1
r.FidelityFX.FSR3.UseSSRExperimentalDenoiser=1
r.FidelityFX.FSR3.DeDither=1
r.FidelityFX.FSR3.Sharpness=0.7
r.FidelityFX.FI.Enabled=1
r.FidelityFX.FI.OverrideSwapChainDX12=1
r.FidelityFX.FI.AllowAsyncWorkloads=1
r.ViewDistanceScale=0.7
tick.AllowAsyncTickDispatch=1
tick.AllowAsyncTickCleanup=1
FX.BatchAsync=1
r.DFShadow.TwoSidedMeshDistanceBiasScale=0
r.DFShadowAsyncCompute=1
r.DFShadowQuality=2
fx.Niagara.DelayScriptAsyncOptimization=false
r.AutoExposure.IgnoreMaterials=true
r.AutoExposure.IgnoreMaterials.UsePrecalculatedIlluminance=false
r.AutoExposure.IgnoreMaterials.LuminanceScale=0.25
r.Lumen.SkylightLeaking.Roughness=0.5
[/code]

Update V0.6s
[h3] IMPORTANT! [/h3]
Starting from this update, it's going to require you to manually enable shadows via console command:
[code] r.Shadow.Virtual.Enable 1 [/code]
on EVERY startup. You will easily notice this when the shadows are disabled (there... won't be any shadows). On second
startup, you'll just have to enter console (default: ~) and press arrow up and then enter. That should save you some
time from looking for the command and/or pasting it.
If you haven't disabled contact shadows, you should do so, other way there might be some visual glitches (related
directly to contact shadows, not VSM per se)

[code]
[SystemSettings]
tick.AllowAsyncTickDispatch=1
tick.AllowAsyncTickCleanup=1
FX.BatchAsync=1
fx.Niagara.DelayScriptAsyncOptimization=false
fx.Niagara.GpuProfiling.Enabled=false
D3D12.EvictAllResidentResourcesInBackground=true
Compat.UseDXT5NormalMaps=1
grass.DensityScale=2.5
grass.TickInterval=10
sg.FoliageLoadDistance=2
r.Streaming.LimitPoolSizeToVRAM=1
D3D12.AdjustTexturePoolSizeBasedOnBudget=1
d3d12.FastAllocator.MinPagesToRetain=2
D3D12.InsertOuterOcclusionQuery=1
D3D12.UseUpdateTexture3DComputeShader=1

[/Script/Engine.RendererSettings]
r.Shadow.Virtual.UseFarShadowCulling=0
r.Shadow.Virtual.Cache.KeepOnlyRequestedPages=1
r.Shadow.Virtual.ForceOnlyVirtualShadowMaps=1
r.Shadow.Virtual.Enable=1
r.Shadow.Virtual.Cache=1
r.Shadow.CSM.MaxCascades=0
r.Shadow.CSM.MaxNearCascades=0
r.Shadow.DistanceScale=2
r.DFShadowQuality=0
r.DFShadowAsyncCompute=1
r.DFShadow.TwoSidedMeshDistanceBiasScale=0
r.AOGlobalDistanceField.MinMeshSDFRadius=5
r.Lumen.Reflections.SmoothBias=0.8
r.Lumen.Reflections.RadianceCache=1
r.Lumen.ScreenProbeGather.TracingOctahedronResolution=10
r.Lumen.ScreenProbeGather.Temporal.MaxFramesAccumulated=30
r.Lumen.ScreenProbeGather.DownsampleFactor=26
r.Lumen.TraceMeshSDFs=1
r.Lumen.TraceMeshSDFs.TraceDistance=200
r.Lumen.TranslucencyReflections.FrontLayer.Enable=1
r.LumenScene.GPUDrivenUpdate=0
r.LumenScene.SurfaceCache.MeshCardsMinSize=5
r.LumenScene.SurfaceCache.CardTexelDensityScale=5000
r.LumenScene.Lighting.AsyncCompute=1
r.Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.Reflections.AsyncCompute=1
r.Lumen.SkylightLeaking.Roughness=0.5
r.FidelityFX.FSR3.CreateReactiveMask=1
r.FidelityFX.FSR3.UseSSRExperimentalDenoiser=1
r.FidelityFX.FSR3.DeDither=1
r.FidelityFX.FSR3.Sharpness=0.7
r.FidelityFX.FI.Enabled=1
r.FidelityFX.FI.OverrideSwapChainDX12=1
r.FidelityFX.FI.AllowAsyncWorkloads=1
r.AutoExposure.IgnoreMaterials=true
r.AutoExposure.IgnoreMaterials.UsePrecalculatedIlluminance=false
r.AutoExposure.IgnoreMaterials.LuminanceScale=0.25
r.SSR.Quality=3
r.ViewDistanceScale=1
r.Shadow.Virtual.Cache.StaticSeparate=1
r.ContactShadows=0
r.Nanite.MaterialVisibility=1
r.Shadow.Virtual.Nanite.MaterialVisibility=1
r.Shadow.Virtual.NonNaniteVSM=1
r.HeighFieldShadowing=0
r.DistanceFieldShadowing=0
r.VirtualTextureReducedMemory=1
r.FidelityFX.FSR3.QuantizeInternalTextures=1
r.Cache.DrawDirectionalShadowing=0
r.Shadow.Virtual.Cache.MaxUnreferencedLightAge=5
r.Shadow.Virtual.Cache.AllocateViaLRU=0
r.Shadow.Virtual.Cache.InvalidateUseHZB=0
r.Nanite.Culling.HZB=0
r.Shadow.NaniteUseHZB=0
r.Shadow.Virtual.UseHZB=0
r.Shadow.Virtual.NonNanite.UseHZB=0
[/code]

[h3] Supported rendering APIs [/h3]
I'm mostly gonna try stuff out with DirectX12, as it's the one that's recommended by devs.

Although I'm keen on testing Vulkan rendering API, since in [i]theory[/i] it should perform better on AMD platforms with
many stuff working in async way.
Support for it is not guaranteed, it works internally, as of update 0.6s, but might break (or might keep working).
I will consider experimenting with it, after I feel like "I'm done" with basic, overall experience on DX12.

[h1] How to? [/h1]
[h2] Changing engine.ini [/h2]
You can either edit your engine.ini file located in:
[code]C:\Users\(username)\AppData\Local\FactoryGame\Saved\Config\Windows[/code]
Quick way to go there (paste in Explorer):
[code]%LocalAppData%\FactoryGame\Saved\Config\Windows[/code]
[h2] Using console [/h2]
Or use those as commands. Mind you, you have to replace the "=" with space!

[h2] Notice [/h2]
If the performance goal will not be met - I might fallback to frame generation sooner or later, which might have it's
own drawbacks. (I'm already using FG in locations that are resource hungry, so some of the performance impact might be
worse with it off)
Second one is that the visual quality will not be as good as I was hoping it might be - There will have to be some
compromises, such as lower render distance, upscaling from lower resolution or some other.

[h1] ⚠ WARNING ⚠ [/h1]
I wanted to get this guide out as quickly as possible, that's why you might see empty sections and/or "TBD" (To be
done). While I'm working on a general construct of this guide, you can already try out some of the cvars (more to come
soonTM!)

[h1] How did we end up here? [/h1]
TBD
(basically write a rant about how update 1.0 "broke" (aka fixed) signs with Lumen, how I made a notepad with all the
tweaks but kinda got lost in it, so I made it more of a guide, then I somehow ended up helping some person with flickery
and noisy lights so I decided to write a guide, especially after seeing how other guides are just copy paste without
either testing stuff or are outdated)

When tweaking some stuff, I look if something has already been set by something called "scalability" - It's when you set
your... let's say shadow quality in game graphic settings to high, it changes not only one, but a few settings "under
the hood". So since I use "high" on shadows, if I tweak a cvar, see something that has been set by scalability to a
value I'm happy with, I'm not going to add it to the tweak list, as for me, personally, it's just useless and might make
troubleshooting harder. And on top of that, I feel like it just makes the list unnecessarily long. So beware of that.

[h2] Changelogs [/h2]
[h3] Update 0.1 [/h3]
[i]Initial release.[/i]

[h3] Update 0.2 [/h3]
[i]No changelog provided.[/i]

[h3] Update 0.3 [/h3]
[i]No changelog provided.[/i]

[h3] Update 0.4p [/h3]
Update 0.4p should bring some performance when comparing to version 0.3

[h3] Update 0.5 [/h3]
[i]No changelog provided.[/i]

[h3] Update 0.6s [/h3]
Important: This update requires some manual in-game steps, be sure to read important info.
[list]
[*]Update 0.6s is out now, and comes with "reworked" shadows!
[*]They are (subjectively) better and more realistic.
[*]Performance has been improved, mostly due to:
[*]Tweaks related to FSR3, DX12, grass, autoexposure (eye-adaptation), Lumen, Nanite, caching, VRAM handling
[/list]

[h3] Update 0.7t [/h3]
Pretty huge update, especially for people with lower end rigs (low VRAM). Touches mostly added earlier shadows, should
improve performance by a fair bit (but mostly should mitigate "stutter that doesn't go away until reloading the game".
It's a huge issue that most likely won't be fixable, so I can only mitigate it. If you start experiencing it, try to
stay in one place for a while, it should be gone in a while, if that doesn't help - restarting the game might be the
only option)

Bear in mind, even though this is a "build your own tweaks" kind of guide, on my end they are tested to work with each
other and complement each other. Latest one can be always found at the top of this section, please make sure you use the
latest one if you wish to report issues

As of update 0.6s, that introduced other way of shading, manual step is required everytime, you open up the game. It's
going to require you to manually enable shadows via console command:
[code] r.Shadow.Virtual.Enable 1 [/code]
on EVERY startup.
You will easily notice this when the shadows are disabled (there... won't be any shadows). On second startup, you'll
just have to enter console (default: ~) and press arrow up and then enter. That should save you some time from looking
for the command and/or pasting it.
If you haven't disabled contact shadows, you should do so, other way there might be some visual glitches (related
directly to contact shadows, not VSM per se)

[h1] Final engine.ini file [/h1]
While these are my "for now" engine.ini tweaks, I keep on working on more and more tweaks. So for now, these are "stable
enough" to play with.

[h2] Update 0.7t (LATEST) [/h2]
As of last update (0.6s), it requires manually enabling VSM via console.

[code]
[SystemSettings]
tick.AllowAsyncTickDispatch=1
tick.AllowAsyncTickCleanup=1
FX.BatchAsync=1
fx.Niagara.DelayScriptAsyncOptimization=false
fx.Niagara.GpuProfiling.Enabled=false
D3D12.EvictAllResidentResourcesInBackground=true
grass.DensityScale=2.5
grass.TickInterval=10
sg.FoliageLoadDistance=2
d3d12.FastAllocator.MinPagesToRetain=4
D3D12.InsertOuterOcclusionQuery=1
D3D12.UseUpdateTexture3DComputeShader=1
s.ForceGCAfterLevelStreamedOut=1
s.ContinuouslyIncrementalGCWhileLevelsPendingPurge=1
s.MaxIncomingRequestsToStall=10
D3D12.AdjustTexturePoolSizeBasedOnBudget=1

[/Script/UnrealEd.ProjectPackagingSettings]
bShareMaterialShaderCode=True
bSharedMaterialNativeLibraries=True

[/Script/Engine.RendererSettings]
r.Shadow.Virtual.Cache.KeepOnlyRequestedPages=1
r.Shadow.Virtual.Enable=1
r.Shadow.Virtual.Cache=1
r.Shadow.CSM.MaxCascades=0
r.Shadow.CSM.MaxNearCascades=0
r.Shadow.DistanceScale=2
r.DFShadowAsyncCompute=1
r.DFShadow.TwoSidedMeshDistanceBiasScale=0
r.AOGlobalDistanceField.MinMeshSDFRadius=5
r.Lumen.Reflections.SmoothBias=0.8
r.Lumen.Reflections.RadianceCache=1
r.Lumen.ScreenProbeGather.TracingOctahedronResolution=10
r.Lumen.ScreenProbeGather.Temporal.MaxFramesAccumulated=30
r.Lumen.ScreenProbeGather.DownsampleFactor=26
r.Lumen.TraceMeshSDFs=1
r.Lumen.TraceMeshSDFs.TraceDistance=200
r.Lumen.TranslucencyReflections.FrontLayer.Enable=1
r.LumenScene.GPUDrivenUpdate=0
r.LumenScene.SurfaceCache.MeshCardsMinSize=5
r.LumenScene.SurfaceCache.CardTexelDensityScale=5000
r.LumenScene.Lighting.AsyncCompute=1
r.Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.Reflections.AsyncCompute=1
r.Lumen.SkylightLeaking.Roughness=0.5
r.FidelityFX.FSR3.CreateReactiveMask=1
r.FidelityFX.FSR3.UseSSRExperimentalDenoiser=1
r.FidelityFX.FSR3.DeDither=1
r.FidelityFX.FI.Enabled=0
r.FidelityFX.FI.OverrideSwapChainDX12=1
r.FidelityFX.FI.AllowAsyncWorkloads=1
r.AutoExposure.IgnoreMaterials=true
r.AutoExposure.IgnoreMaterials.UsePrecalculatedIlluminance=false
r.AutoExposure.IgnoreMaterials.LuminanceScale=0.25
r.ViewDistanceScale=1
r.Shadow.Virtual.Cache.StaticSeparate=0
r.ContactShadows=0
r.Shadow.Virtual.NonNaniteVSM=1
r.VirtualTextureReducedMemory=1
r.VirtualTexture=1
r.VirtualTextures=1
r.Cache.DrawDirectionalShadowing=1
r.Shadow.Virtual.Cache.AllocateViaLRU=0
r.VRS.Enable=true
r.VRS.EnableImage=true
r.Nanite.SoftwareVRS=1
r.ReflectionMethod=1
r.GTSyncType=1
r.ScreenPercentage=70
r.OneFrameThreadLag=0
r.ShaderPipelineCache.StartupMode=2
r.Water.SingleLayer.ShadersSupportVSMFiltering=1
r.Lumen.TraceMeshSDFs.Allow=1
r.Lumen.RadianceCache.SpatialFilterMaxRadianceHitAngle=30
r.Shadow.Virtual.ShowLightDrawEvents=1
r.SkinCache.Allow=true
r.SkinCache.CompileShaders=1
r.HeightFieldShadowing=0
r.DistanceFieldShadowing=0
r.Nanite.MaxPixelsPerEdge=2
r.Nanite.DicingRate=1
r.Nanite.FastVisBufferClear=2
r.VT.MaxUploadsPerFrame=4
r.VT.MaxContinuousUpdatesPerFrame=1
r.VT.RVT.TileCountBias=-1
r.VT.ParallelFeedbackTasks=1
r.Lumen.ScreenProbeGather.StochasticInterpolation=4
r.Lumen.TranslucencyReflections.FrontLayer.EnableForProject=1
r.Lumen.ScreenProbeGather.TwoSidedFoliageBackfaceDiffuse=1
r.Lumen.SampleFog=1
r.EnableAsyncComputeTranslucencyLightingVolumeClear=1
r.VRS.NaniteEmitGBuffer=1
r.VRS.BasePass=1
r.VRS.Decals=1
r.VRS.SSAO=1
r.VRS.SSR=1
r.Lumen.ScreenProbeGather.TemporalFilterProbes=1
r.DynamicGlobalIlluminationMethod=1
r.ReflectionCapture.EnableLightFunctions=1
r.AllowGlobalClipPlane=1
r.Emitter.FastPoolEnable=1

[Core.Log]
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

[/Script/Engine.InputSettings]
bEnableMouseSmoothing=False

[CrashReportClient]
bAgreeToCrashUpload=false
bImplicitSend=False

[Engine.ErrorHandling]
bPromptForRemoteDebugging=False
bPromptForRemoteDebugOnEnsure=False

[/Script/Engine.Engine]
bAllowMultiThreadedShaderCompile=True

[/script/akaudio.aksettings]
bEnableMultiCoreRendering=True

# Max Framerate / Less Quality

r.VolumetricCloud 0
r.Lumen.AsyncCompute 0
r.Lumen.ScreenProbeGather.RadianceCache.ProbeResolution 8
r.DynamicGlobalIlluminationMethod 3
r.Lumen.Reflections.Allow 0
r.ReflectionMethod 0
r.Lumen.DiffuseIndirect.Allow 0
r.Lumen.ScreenProbeGather.DiffuseIntegralMethod 1
r.Shadow.Virtual.Enable 1
r.Shadow.CSM.MaxCascades 1
r.AntiAliasingMethod 2
r.Nanite.MaxPixelsPerEdge 2
r.lightshaftquality 0
r.LightShaftDownSampleFactor 1
r.BloomQuality 0
r.shadowquality 0
r.contactshadows 0
foliage.DensityScale 0
grass.enable 0
r.Streaming.PoolSize 0

# Better Framerate / Better Quality

r.VolumetricCloud 0
r.Lumen.AsyncCompute 1
r.DynamicGlobalIlluminationMethod 1
r.Lumen.Reflections.Allow 0
r.ReflectionMethod 2
r.Lumen.DiffuseIndirect.Allow 1
r.Lumen.ScreenProbeGather.DiffuseIntegralMethod 1
r.Shadow.Virtual.Enable 1
r.Shadow.CSM.MaxCascades 2
r.AntiAliasingMethod 2
r.Nanite.MaxPixelsPerEdge 2
r.lightshaftquality 1
r.LightShaftDownSampleFactor 4
r.BloomQuality 0
r.contactshadows 1
r.Streaming.PoolSize 0

# Soft Boost

r.VolumetricCloud 0
r.VolumetricFog 0
r.Lumen.AsyncCompute 1
r.Streaming.PoolSize 0
r.Nanite.MaxPixelsPerEdge 2

# Async

Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.Reflections.AsyncCompute=1
r.Bloom.AsyncCompute=1
r.D3D12.AllowAsyncCompute=1
r.DFShadowAsyncCompute=1
r.EnableAsyncComputeTranslucencyLightingVolumeClear=1
r.Lumen.AsyncCompute=1
r.LumenScene.Lighting.AsyncCompute=1
r.Nanite.Streaming.AsyncCompute=1
r.SceneDepthHZBAsyncCompute=1
r.SkyAtmosphereASyncCompute=1
r.SparseVolumeTexture.Streaming.AsyncCompute=1
r.TSR.AsyncCompute=3
r.VolumetricRenderTarget.PreferAsyncCompute=1

ADD this:
sg.FoliageLoadDistance=4
r.Lumen.Reflections.AsyncCompute=1
r.TSR.AsyncCompute=1
r.SkyAtmosphereASyncCompute=1
r.EnableAsyncComputeTranslucencyLightingVolumeClear
r.DFShadowAsyncCompute=1

# THE CHANT

Add the following text to your Appdata/Local/Chant/Saved/Config/WindowsNoEditor/Engine.ini file to hopefully reduce the
Unreal Engine 4 shader compilation stutters in The Chant. Your mileage may vary but it massively reduced stutters for
me:

[SystemSettings]
r.MaxAnisotropy=16
r.SSS.HalfRes=0
r.SSR.HalfRes=0
r.TemporalAACurrentFrameWeight=0.05
r.TemporalAAsamples=32
r.TemporalAASharpness=1.4
r.TemporalAADynamicSharpen=1.8
r.PostProcessAAQuality=6
r.TemporalAACatmullRom=1
r.tonemapperfilm=1
r.Tonemapper.Quality=5
r.Tonemapper.Sharpen=1.6
r.MSAA.CompositingSampleCount=0
r.FastBlurThreshold=0
r.MotionBlurQuality=0
r.MotionBlur.Amount=0
r.MotionBlurSeparable=0
r.DepthOfFieldQuality=0
r.DepthOfField.FarBlur=0
r.DefaultFeature.MotionBlur=0
r.SceneColorFringe.Max=0
r.SceneColorFringeQuality=0
r.MotionBlur.Max=0
r.BloomQuality=5
r.ParticleLightQuality=3
r.RefractionQuality=3
r.RefractionQuality=5
r.HighQualityLightMaps=1
r.TrueSkyQuality=1
r.SkyLightingQuality=1
r.SceneColorFormat=4
r.EyeAdaptationQuality=3
r.FastVRam.EyeAdaptation=1
r.LightFunctionQuality=3
r.Emitter.FastPoolEnable=1
r.VolumetricFog.Jitter=1
r.VolumetricFog.InjectShadowedLightsSeparately=1
r.VolumetricFog.InverseSquaredLightDistanceBiasScale=1.000000
r.VolumetricFog.LightFunctionSupersampleScale=2.000000
r.NGX.DLSS.Sharpness=0.3
r.TriangleOrderOptimization=1
r.Shaders.Optimize=1
r.OptimizeForUAVPerformance=1
r.AOAsyncBuildQueue=1
r.AOComputeShaderNormalCalculation=1
r.VirtualTexture=1
r.FastVRam.BokehDOF=1
r.FastVRam.CircleDOF=1
r.FastVRam.CombineLUTs=1
r.FastVRam.Distortion=1
r.FastVRam.HZB=1
r.FastVRam.Histogram=1
r.FastVRam.SSR=1
r.FastVRam.ShadowPointLight=1
r.FastVRam.Tonemap=1
r.FastVRam.Upscale=1
r.DX11NVAfterMathEnabled=0
D3D12.PSO.DiskCache=1
D3D12.PSO.DriverOptimizedDiskCache=1
D3D11.AFRSyncTemporalResources=1
D3D11.AFRUseFramePacing=1
D3D11.AdjustTexturePoolSizeBasedOnBudget=1
D3D11.AsyncDeferredDeletion=1
D3D11.ForceThirtyHz=0
D3D11.InsertOuterOcclusionQuery=1
D3D11.ResidencyManagement=1
D3D11.StablePowerState=0
D3D11.SyncWithDWM=1
D3D11.TexturePoolOnlyAccountStreamableTexture=1
D3D11.UseUpdateTexture3DComputeShader=1
d3d11.ZeroBufferSizeInMB=32
D3D12.AFRSyncTemporalResources=1
D3D12.AFRUseFramePacing=1
D3D12.AsyncDeferredDeletion=1
D3D12.ForceThirtyHz=0
D3D12.InsertOuterOcclusionQuery=1
D3D12.ResidencyManagement=1
D3D12.ResidencyManagement=1
D3D12.StablePowerState=0
D3D12.StablePowerState=0
D3D12.SyncWithDWM=1
D3D12.SyncWithDWM=1
D3D12.TexturePoolOnlyAccountStreamableTexture=1
D3D12.UseUpdateTexture3DComputeShader=1
D3D12.UseUpdateTexture3DComputeShader=0
D3D12.ZeroBufferSizeInMB=32
D3D12.ZeroBufferSizeInMB=128
D3D12.MaximumFrameLatency=0
r.ShaderPipelineCache.PreOptimizeEnabled=1
r.ShaderPipelineCache.SaveUserCache=1
r.ShaderPipelines=1
r.Shaders.ZeroInitialise=1
r.Shadow.CachePreshadow=1
r.Shadow.CacheWPOPrimitives=1
r.SceneRenderTargetResizeMethod=2
r.SceneRenderTargetResizeMethodForceOverride=1
r.ShaderComplexity.CacheShaders=1
r.ShaderPipelineCache.StartupMode=3
r.ShaderPipelineCache.Enabled=1
r.ShaderPipelineCache.LogPSO=1
r.ShaderPipelineCache.SaveBoundPSOLog=1
r.ShaderPipelineCache.ReportPSO=1
r.ShaderPipelineCache.GameFileMaskEnabled=1
r.ShaderPipelineCache.LazyLoadShadersWhenPSOCacheIsPresent=1
r.ShaderPipelineCache.BatchSize=10
r.Vulkan.PipelineCacheFromShaderPipelineCache=1
r.D3D.ForceDXC=1
r.GTSyncType=1
niagara.CreateShadersOnLoad=1
r.ShaderPipelineCache.AlwaysGenerateOSCache=0
r.XGEShaderCompile=1
r.XGEShaderCompile.Mode=1
r.XGEShaderCompile.Xml.BatchGroupSize=256
r.XGEShaderCompile.Xml.BatchSize=16
r.XGEShaderCompile.Xml.JobTimeout=0.500000
r.RHICmdAsyncRHIThreadDispatch=1
r.RHICmdBalanceParallelLists=2
r.RHICmdBalanceTranslatesAfterTasks=1
r.RHICmdBasePassDeferredContexts=1
r.RHICmdCollectRHIThreadStatsFromHighLevel=0
r.RHICmdDeferSkeletalLockAndFillToRHIThread=1
r.RHICmdPrePassDeferredContexts=1
r.RHICmdShadowDeferredContexts=1
r.RHICmdSpewParallelListBalance=0
r.RHICmdTranslucencyPassDeferredContexts=1
r.RHICmdUseDeferredContexts=1
r.RHICmdUseParallelAlgorithms=1
r.RHICmdVelocityPassDeferredContexts=1
r.RHICmdUseThread=1
r.RHICmdWidth=12
r.ParallelRendering=1
r.ParallelShadows=1
r.ParallelTranslucency=1
r.ParallelVelocity=1
r.SkeletalMeshLODBias=-2
r.CreateShadersOnLoad=1
r.HZBOcclusion=0
r.AllowOcclusionQueries=1

[/script/engine.engine]
FrameRateLimit=0.000000
bEnableMouseSmoothing=False
r.OneFrameThreadLag=0
bAllowMultiThreadedShaderCompile=true

[ConsoleVariables]
AllowAsyncRenderThreadUpdates=1
AllowAsyncRenderThreadUpdates=1
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=1
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=1
AllowAsyncRenderThreadUpdatesEditor=1
AllowAsyncRenderThreadUpdatesEditor=1
s.AsyncLoadingThreadEnabled=True

[TextureStreaming]
r.Streaming.Boost=1
r.Streaming.FullyLoadUsedTextures=1
r.Streaming.HLODStrategy=2
r.bForceCPUAccessToGPUSkinVerts=True
r.Streaming.FramesForFullUpdate=1
r.Streaming.UseBackgroundThreadPool=1
r.Cache.UpdateEveryFrame=1
r.Streaming.DropMips=0
r.Streaming.LimitPoolSizeToVRAM=0
r.Streaming.MaxTempMemoryAllowed=1024
r.Streaming.MinMipForSplitRequest=0
r.Streaming.HiddenPrimitiveScale=1
r.DetailMode=3
r.LODFadeTime=0.2
r.Streaming.MipBias=0
r.Streaming.UseAllMips=1
r.Streaming.UseMaterialData=1
r.Streaming.UseNewMetrics=1
r.Streaming.UsePerTextureBias=1
r.Shaders.Optimize=1
r.Shaders.FastMath=1
r.UseShaderCaching=1
r.UseShaderPredraw=1
FX.AllowGPUParticles=1
FX.AllowAsyncTick=1
FX.EarlyScheduleAsync=1
r.ForceAllCoresForShaderCompiling=1
FX.BatchAsync=1
FX.BatchAsyncBatchSize=1
r.Emitter.FastPoolEnable=1
FX.AllowGPUSorting=1
FX.AllowCulling=0
GeometryCache.InterpolateFrames=1
GeometryCache.OffloadUpdate=1
r.UseAsyncShaderPrecompilation=1
r.AmbientOcclusion.AsyncComputeBudget=1
r.HZB.BuildUseCompute=1
r.MultithreadedLightmapEncode=1
r.MultithreadedShadowmapEncode=1
r.RDG.AsyncCompute=1
RHI.MaximumFrameLatency=3
r.TargetPrecompileFrameTime=2.00
r.PredrawBatchTime=13
r.AccelPredrawBatchTime=0
r.AccelTargetPrecompileFrameTime=0
r.Streaming.DefragDynamicBounds=1
s.AsyncLoadingThreadEnabled=True
s.AsyncLoadingThreadEnabled=1
r.Streaming.PoolSize=0
r.VirtualTextures=1
r.Streaming.AmortizeCPUToGPUCopy=1
r.Streaming.MaxNumTexturesToStreamPerFrame=3
r.Streaming.NumStaticComponentsProcessedPerFrame=3
s.AsyncLoadingTimeLimit=2.0
s.LevelStreamingActorsUpdateTimeLimit=1.00
s.UnregisterComponentsTimeLimit=0.00
s.AsyncLoadingUseFullTimeLimit=0
s.ContinuouslyIncrementalGCWhileLevelsPendingPurge=1
s.PriorityAsyncLoadingExtraTime=10.00
s.IoDispatcherCacheSizeMB=2048
s.LevelStreamingComponentsRegistrationGranularity=1
s.LevelStreamingComponentsUnregistrationGranularity=1
s.MaxIncomingRequestsToStall=100
s.MaxReadyRequestsToStallMB=0
s.MinBulkDataSizeForAsyncLoading=1000

# Hogwarts Legacy

[SystemSettings]
r.HZBOcclusion =
r.VolumetricCloud.HzbCulling = 0
r.D3D.ForceDXC = 1
r.D3D12.ExecuteCommandListTask = 1
D3D12.MaximumFrameLatency = 2
s.ContinuouslyIncrementalGCWhileLevelsPendingPurge = 1
gc.CreateGCClusters = 1
gc.MaxObjectsNotConsideredByGC = 32
gc.LowMemory.TimeBetweenPurgingPendingLevels = 60
gc.LowMemory.TimeBetweenPurgingPendingKillObjects = 300
gc.TimeBetweenPurgingPendingKillObjects = 300
gc.NumRetriesBeforeForcingGC = 3
gc.MinDesiredObjectsPerSubTask = 4
gc.LowMemory.MemoryThresholdMB = 4096
gc.MultithreadedDestructionEnabled = 1
r.DontLimitOnBattery = 1
r.Emitter.FastPoolEnable = 1
r.UseClusteredDeferredShading = 1
r.AllowOcclusionQueries = 1
r.AllowSubPrimitiveQueries=0
UIManager.PauseMenuStreamingMemoryClear=2048
r.ForceHighestMipOnUITextures=1
r.HairStrands.Cull=2
r.HairStrands.Visibility.UseFastPath=1
r.GBufferFormat=3
r.GTSyncType=1
r.OneFrameThreadLag=1
r.FinishCurrentFrame=0
r.DiffuseIndirect.Denoiser=1
r.ForceSceneHasDecals=1
rhi.SyncInterval=0
rhi.SyncSlackMS=0
r.RHICmdUseDeferredContexts=1
r.RHICmdMinDrawsPerParallelCmdList=512
r.RHICmdMinCmdlistForParallelSubmit=4
r.RHICmdBalanceTranslatesAfterTasks=0
r.RHICmdBalanceParallelLists=1
r.RHICmdFlushRenderThreadTasksPrePass=1
r.RHICmdFlushRenderThreadTasksShadowPass=1
r.RHICmdFlushRenderThreadTasksBasePass=1
r.RHICmdFlushRenderThreadTasksTranslucentPass=1
r.RHICmdFlushRenderThreadTasksVelocityPass=1
r.RenderTargetPool.AllowMultipleAliasingDiscardsPerFrame=1
r.RHICmdAsyncRHIThreadDispatch=1
GeometryCache.OffloadUpdate=1
Niagara.GPUSorting.CPUToGPUThreshold=200
s.AsyncLoadingThreadEnabled=1
s.AsyncLoadingUseFullTimeLimit=0
r.UseParallelGetDynamicMeshElementsTasks=1
s.IoDispatcherCacheSizeMB=512
s.MaxIncomingRequestsToStall=24
s.MaxReadyRequestsToStallMB=6
r.Shadow.MaxNumSpotShadowCacheUpdatesPerFrame=1
r.Shadow.MaxNumPointShadowCacheUpdatesPerFrame=1
TaskGraph.ForkedProcessMaxWorkerThreads=16
r.MaxAnisotropy=16
r.VT.MaxAnisotropy=16
s.LevelStreamingActorsUpdateTimeLimit=2
r.Streaming.OverlapAssetAndLevelTicks=1
r.Streaming.AbandonedDDCHandlePurgeFrequency=100
r.Streaming.MaxTempMemoryAllowed=2048
r.Streaming.AmortizeCPUToGPUCopy=1
r.Streaming.MaxNumTexturesToStreamPerFrame=32
r.Streaming.NumStaticComponentsProcessedPerFrame=4096
r.Streaming.UseMaterialData=1
r.Streaming.UseNewMetrics=1
r.Streaming.UsePerTextureBias=1
r.Streaming.HiddenPrimitiveScale=0.0
r.Streaming.MinMipForSplitRequest=2
r.Streaming.UseBackgroundThreadPool=1
r.Streaming.FramesForFullUpdate=2
r.Streaming.PoolSize=-1
r.Streaming.UseFixedPoolSize=1
r.Streaming.LimitPoolSizeToVRAM=1
r.Streaming.PoolSize.VRAMPercentageClamp=1024
r.Streaming.UseAsyncRequestsForDDC=1
r.Streaming.FullyLoadUsedTextures=0
r.Streaming.Boost=1.0
r.TextureStreaming=1
r.Shaders.Optimize=1
r.Shaders.FastMath=1
r.ShaderPipelineCache.StartupMode=1
r.ShaderPipelineCache.Enabled=1
r.ShaderPipelineCache.ReportPSO=0
r.ShaderPipelineCache.GameFileMaskEnabled=0
r.ShaderPipelineCache.LazyLoadShadersWhenPSOCacheIsPresent=0
r.ShaderPipelineCache.BatchSize=256
r.ShaderPipelineCache.BatchTime=10
r.ShaderPipelineCache.BackgroundBatchTime=1
r.SkinCache.CompileShaders=1
r.VirtualTexturedLightmaps=0
r.VirtualTextureReducedMemory=1
r.XGEShaderCompile=1
r.XGEShaderCompile.Mode=1
r.XGEShaderCompile.Xml.BatchGroupSize=256
r.XGEShaderCompile.Xml.BatchSize=64
r.GPUParticle.Simulate=1
r.MotionBlur.Max=0
r.MotionBlurQuality=0
r.FastBlurThreshold=0
r.BlurGBuffer=0
r.Color.Min=0.00
r.Color.Mid=0.80
r.Color.Max=1.05
r.TonemapperGamma=1.35
r.Tonemapper.Quality=2
r.Tonemapper.Sharpen=0.2
r.SSGI.Enable=1
r.SSGI.HalfRes=0
r.SSGI.Quality=1
r.BloomQuality=2

# Hogwarts Legacy RayTracing

r.RayTracing.AsyncBuild=1
r.RayTracing.AmbientOcclusion.EnableTwoSidedGeometry=1
r.RayTracing.AmbientOcclusion.SamplesPerPixel=1
r.RayTracing.AmbientOcclusion.Intensity=1.0
r.RayTracing.CacheShaderRecords=1 ; helps some stutters
r.RayTracing.Culling=1 ; 0=Culling disabled (default) 1=Culling by distance and angle
r.RayTracing.Culling.Radius=20000
r.RayTracing.Culling.Angle=20
r.RayTracing.LightCulling.CellSize=100
r.RayTracing.ParallelMeshBatchSize=64
r.RayTracing.NonBlockingPipelineCreation=0 ; wait for rays to complete. if enabled it will "BLACK" anything that is not
finished.
r.RayTracing.DebugVisualizationMode.OpaqueOnly=0
r.RayTracing.Geometry.GetEachLODForHISM=0
r.RayTracing.Geometry.NiagaraSprites=0
r.RayTracing.Geometry.NiagaraRibbons=0
r.RayTracing.Geometry.NiagaraMeshes=0
r.RayTracing.Geometry.MaxBuiltPrimitivesPerFrame=500
r.RayTracing.Geometry.InstancedStaticMeshes.SimulationCount=8
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
r.RayTracing.PSOCacheSize=300
r.RayTracing.SkyLight=0
r.RayTracing.SkyLight.SamplesPerPixel=4
r.RayTracing.SkyLight.denoiser=1
r.RayTracing.SkyLight.MaxShadowThickness=5
r.RayTracing.SkyLight.EnableMaterials=0
r.RayTracing.Shadow.SamplesPerPixel=1
r.RayTracing.Shadow.MaxBatchSize=64
r.RayTracing.Shadows.EnableMaterials=1
r.RayTracing.Shadows.EnableFrontFaceCulling=1
r.RayTracing.Shadows.EnableTwoSidedGeometry=1
r.RayTracing.Shadows.DefaultLightSourceRadius=0.7
r.RayTracing.Shadows.Decals=1
r.RayTracing.Shadows.Lights.Directional=1 ; disabled on <ULTRA RT and breaks Grand Staircase shadows.
r.RayTracing.Shadows.Lights.Point=1
r.RayTracing.Shadows.Lights.Rect=1
r.RayTracing.Shadows.Lights.Spot=1
r.RayTracing.Translucency=0
r.RayTracing.Translucency.SamplesPerPixel=1
r.RayTracing.Translucency.Shadows=1
r.RayTracing.Translucency.MaxRefractionRays=2
r.RayTracing.Translucency.MaxRayDistance=10000
r.RayTracing.Translucency.MaxRoughness=0.2
r.RayTracing.Translucency.MinRayDistance=100
r.RayTracing.Translucency.Refraction=0 ; broken in trophy room.
r.RayTracing.Reflections.ScreenPercentage=100
r.RayTracing.Reflections.SamplesPerPixel=1
r.RayTracing.Reflections.MaxRoughness=0.3 ; closest to NON-RT reflections IMO
r.RayTracing.Reflections.DirectLighting=1
r.RayTracing.Reflections.HeightFog=1
r.RayTracing.Reflections.Shadows=2
r.RayTracing.Reflections.MaxRayDistance=6000
r.RayTracing.Reflections.MaxBounces=6
r.RayTracing.Reflections.ReflectionCaptures=1 ;Show other reflections in the reflection
r.RayTracing.Reflections.Hybrid=1 ; mix screenspace and RT
r.RayTracing.Reflections.TestPathRoughness=1 ; end ray early if roughness accumulates over "MaxRoughness"
r.RayTracing.Reflections.ExperimentalDeferred.SpatialResolve=0 ; this is what makes RT mirrors look like trash.
r.RayTracing.UseTextureLod=1
r.Reflections.Denoiser=0 ; this also makes it look like trash.

# Hogwarts Legacy 2

[SystemSettings]
r.DBuffer=1
gc.LowMemory.MemoryThresholdMB=512
r.AllowOcclusionQueries=1
r.AllowSubPrimitiveQueries=1
r.HZBOcclusion=0
r.VolumetricCloud.HzbCulling=0
r.D3D.ForceDXC=1
r.D3D12.ExecuteCommandListTask=1
r.FrustumCullNumWordsPerTask=32
UIManager.PauseMenuStreamingMemoryClear=2048
gc.LowMemory.TimeBetweenPurgingPendingLevels=60
gc.LowMemory.TimeBetweenPurgingPendingKillObjects=60
gc.MinDesiredObjectsPerSubTask=2
gc.MultithreadedDestructionEnabled=1
gc.MaxObjectsNotConsideredByGC=32
r.ForceHighestMipOnUITextures=1
r.HairStrands.Cull=2
r.HairStrands.Visibility.UseFastPath=1
r.GBufferFormat=3
r.GTSyncType=1
r.OneFrameThreadLag=1
r.FinishCurrentFrame=0
rhi.SyncInterval=0
rhi.SyncSlackMS=0
r.RHICmdUseDeferredContexts=1
r.RHICmdMinDrawsPerParallelCmdList=128
r.RHICmdMinCmdlistForParallelSubmit=4
r.RHICmdBalanceParallelLists=1
r.RenderTargetPool.AllowMultipleAliasingDiscardsPerFrame=1
r.RHICmdAsyncRHIThreadDispatch=1
GeometryCache.OffloadUpdate=1
TaskGraph.ForkedProcessMaxWorkerThreads=16
s.IoDispatcherCacheSizeMB=512
r.Streaming.OverlapAssetAndLevelTicks=1
r.Streaming.AbandonedDDCHandlePurgeFrequency=100
r.Streaming.MaxTempMemoryAllowed=4096
r.Streaming.AmortizeCPUToGPUCopy=1
r.Streaming.MaxNumTexturesToStreamPerFrame=32
r.Streaming.NumStaticComponentsProcessedPerFrame=6144
r.Streaming.UseMaterialData=1
r.Streaming.UseNewMetrics=1
r.Streaming.UsePerTextureBias=1
r.Streaming.HiddenPrimitiveScale=0.1
r.Streaming.MinMipForSplitRequest=2
r.Streaming.UseBackgroundThreadPool=1
r.Streaming.PoolSize=-1
r.Streaming.UseFixedPoolSize=1
r.Streaming.LimitPoolSizeToVRAM=1
r.Streaming.PoolSize.VRAMPercentageClamp=1024
r.Streaming.UseAsyncRequestsForDDC=1
r.Streaming.FullyLoadUsedTextures=0
r.TextureStreaming=1
r.GenerateMeshDistanceFields=1
r.Shadow.ForceSerialSingleRenderPass=1
r.TiledDeferredShading.MinimumCount=1
r.SkyAtmosphere.DistantSkyLightLUT=0
r.RayTracing.AsyncBuild=1
r.RayTracing.ParallelMeshBatchSize=64
r.RayTracing.NonBlockingPipelineCreation=1
r.RayTracing.PSOCacheSize=300
r.RayTracing.AmbientOcclusion.EnableTwoSidedGeometry=1
r.RayTracing.AmbientOcclusion.SamplesPerPixel=1
r.RayTracing.AmbientOcclusion.Intensity=1.0
r.RayTracing.CacheShaderRecords=1
r.RayTracing.Culling=1
r.RayTracing.DebugVisualizationMode.OpaqueOnly=0
r.RayTracing.Geometry.GetEachLODForHISM=0
r.RayTracing.Geometry.NiagaraSprites=0
r.RayTracing.Geometry.NiagaraRibbons=0
r.RayTracing.Geometry.NiagaraMeshes=0
r.RayTracing.Geometry.MaxBuiltPrimitivesPerFrame=500
r.RayTracing.Geometry.InstancedStaticMeshes.SimulationCount=16
r.SkyLight.RealTimeReflectionCapture=0
r.RayTracing.Shadow.SamplesPerPixel=1
r.RayTracing.Shadow.MaxBatchSize=64
r.RayTracing.Shadows.EnableMaterials=1
r.RayTracing.Shadows.EnableFrontFaceCulling=1
r.RayTracing.Shadows.EnableTwoSidedGeometry=1
r.RayTracing.Shadows.DefaultLightSourceRadius=0.7
r.RayTracing.Shadows.Decals=1
r.RayTracing.Shadows.Lights.Directional=1
r.RayTracing.Shadows.Lights.Point=1
r.RayTracing.Shadows.Lights.Rect=1
r.RayTracing.Shadows.Lights.Spot=1
r.RayTracing.Reflections.ScreenPercentage=100
r.RayTracing.Reflections.SamplesPerPixel=1
r.RayTracing.Reflections.MaxRoughness=0.5
r.RayTracing.Reflections.DirectLighting=1
r.RayTracing.Reflections.EmissiveAndIndirectLighting=1
r.RayTracing.Reflections.RayTraceSkyLightContribution=0
r.RayTracing.Reflections.HeightFog=1
r.RayTracing.Reflections.Shadows=2
r.RayTracing.Reflections.MaxRayDistance=5000
r.RayTracing.Reflections.MaxBounces=6
r.RayTracing.Reflections.ReflectionCaptures=1
r.RayTracing.Reflections.Hybrid=0
r.RayTracing.Reflections.TestPathRoughness=1
r.RayTracing.Reflections.ExperimentalDeferred.SpatialResolve=0
r.RayTracing.Reflections.ExperimentalDeferred.GenerateRaysWithRGS=0
r.RayTracing.UseTextureLod=0
r.Reflections.Denoiser=0
r.DiffuseIndirect.Denoiser=1
r.SkinCache.CompileShaders=1
r.VirtualTexturedLightmaps=0
r.VirtualTextureReducedMemory=1
r.GPUParticle.Simulate=1
r.SSGI.Enable=1
r.SSGI.HalfRes=0
r.SSGI.Quality=2
r.BloomQuality=2

# IDK

An option that I think you should try is r.ShaderPipelineCache.BatchSize ans r.ShaderPipelineCache.BatchTime. This also
helps to reduce stutterings in game. =) I am williing to learn something about these parameters in the game. I am
massing with these parameters a bit. Wierdly, setting It to 10 causes FPS gains, But on the other side, setting It too
low seems also to cause stutterings. I tried to increase It a bit to gain FPS whilst It can reduce stutterings. I am not
happy with such a parameter right now.
ou made me think something interesting. What If I decrease MaxNumTexturesToStreamPerFrame, but increase the batchsize?
You seem to be good with a batch size of 256. For me, It seems a huge number.
I tested It. It indeed almost remove stutterings, but also reduces FPS while walking around. Mostly because It causes
textures to be loading all the time.

# Hogwarts Legacy from some dude who knows his shit

[TextureStreaming]
r.UseShaderCaching=1
r.UseShaderPredraw=1
r.Streaming.FullyLoadUsedTextures=1
r.Streaming.AmortizeCPUToGPUCopy=1
r.TransientResourceAliasing.Buffers=1
r.StencilLODMode=2
r.Streaming.MaxReferenceChecksBeforeStreamOut=2
r.Streaming.Boost=6
r.Streaming.DefragDynamicBounds=1
r.Streaming.DropMips=1
r.Streaming.FlushTimeOut=2
r.Streaming.MaxNumTexturesToStreamPerFrame=2
r.Streaming.NumStaticComponentsProcessedPerFrame=2
r.Streaming.FramesForFullUpdate=1
r.Streaming.ScaleTexturesByGlobalMyBias=0.5
r.Streaming.HLODStrategy=0
r.Streaming.HiddenPrimitiveScale=0.0333
r.Streaming.MaxHiddenPrimitiveViewBoost=1.5
r.Streaming.MinBoost=0.5
r.Streaming.MinMipForSplitRequest=15
r.Streaming.PoolSizeForMeshes=-1
r.Streaming.PrioritizeMeshLODRetention=1
r.Streaming.UseBackgroundThreadPool=1
r.Streaming.UseFixedPoolSize=0
r.Streaming.UseMaterialData=1
r.Streaming.UseNewMetrics=1
r.Streaming.UseAsyncRequestsForDDC=1
r.Emitter.FastPoolEnable=1
r.MultithreadedLightmapEncode=1
r.MultithreadedShadowmapEncode=1
s.ContinuouslyIncrementalGCWhileLevelsPendingPurge=0
s.AsyncLoadingThreadEnabled=True
s.AsyncLoadingThreadEnabled=1
r.Cache.UpdateEveryFrame=1
r.Cache.DrawInterpolationPoints=1
r.Shaders.FastMath=1
r.Shaders.Optimize=1
r.Shaders.ZeroInitialise=1
GeometryCache.InterpolateFrames=1
GeometryCache.OffloadUpdate=1
r.HZB.BuildUseCompute=1
r.AccelPredrawBatchTime=0
r.PredrawBatchTime=-1
r.UseAsyncShaderPrecompilation=1
r.AccelTargetPrecompileFrameTime=0
r.TargetPrecompileFrameTime=16
s.UnregisterComponentsTimeLimit=2
s.AsyncLoadingUseFullTimeLimit=0
s.LevelStreamingComponentsRegistrationGranularity=1
s.LevelStreamingComponentsUnregistrationGranularity=1
s.MaxIncomingRequestsToStall=1
s.MaxReadyRequestsToStallMB=0
s.MinBulkDataSizeForAsyncLoading=0
r.bForceCPUAccessToGPUSkinVerts=True
r.RDG.AsyncCompute=1
r.AsyncCompute=1
r.AsyncCreateLightPrimitiveInteractions=1
r.AsyncPipelineCompile=1
r.AmbientOcclusion.AsyncComputeBudget=1
r.AmbientOcclusion.Compute=2
r.VirtualTextures=1
r.Bloom.HalfResolutionFFT=1
r.Filter.SizeScale=1.0
r.Atmosphere=0
r.PreTileTextures=1

[/script/engine.renderersettings]
D3D12.PSO.DriverOptimizedDiskCache=1
D3D12.AFRSyncTemporalResources=1
D3D12.AFRUseFramePacing=1
D3D12.AsyncDeferredDeletion=1
D3D12.ForceThirtyHz=0
D3D12.InsertOuterOcclusionQuery=1
D3D12.ResidencyManagement=1
D3D12.StablePowerState=0
D3D12.SyncWithDWM=0
D3D12.TexturePoolOnlyAccountStreamableTexture=1
D3D12.UseUpdateTexture3DComputeShader=0
D3D12.ZeroBufferSizeInMB=128
D3D12.MaximumFrameLatency=3
RHI.MaximumFrameLatency=3
RHI.SyncWithDWM=0
RHI.SyncInterval=0
r.ShaderPipelines=1
r.ShaderPipelineCache.PreOptimizeEnabled=1
r.ShaderPipelineCache.SaveUserCache=1
r.ShaderPipelineCache.LogPSO=0
r.ShaderPipelineCache.ReportPSO=0
r.ShaderPipelineCache.SaveBoundPSOLog=1
r.ShaderPipelineCache.GameFileMaskEnabled=1
r.ShaderPipelineCache.Enabled=1
r.ShaderPipelineCache.StartupMode=3
r.ShaderPipelineCache.BatchSize=128
r.ShaderPipelineCache.PrecompileBatchSize=128
r.ShaderPipelineCache.BackgroundBatchSize=4
r.ShaderPipelineCache.BatchTime=64
r.ShaderPipelineCache.BackgroundBatchTime=32
r.ShaderPipelineCache.PrecompileBatchTime=52
r.ShaderPipelineCache.AlwaysGenerateOSCache=0
r.ShaderPipelineCache.LazyLoadShadersWhenPSOCacheIsPresent=1
r.Shadow.CachePreshadow=1
r.Shadow.CacheWPOPrimitives=1
r.SceneRenderTargetResizeMethod=2
r.SceneRenderTargetResizeMethodForceOverride=1
r.ShaderComplexity.CacheShaders=1
r.D3D.ForceDXC=0
r.D3D.ForceShaderConductorDXCRewrite=0
r.D3D12.ExecuteCommandListTask=1
niagara.CreateShadersOnLoad=1
r.RHICmdAsyncRHIThreadDispatch=1
r.RHICmdBalanceParallelLists=2
r.RHICmdBalanceTranslatesAfterTasks=1
r.RHICmdBufferWriteLocks=0
r.RHICmdBypass=0
r.RHICmdBasePassDeferredContexts=1
r.RHICmdCollectRHIThreadStatsFromHighLevel=0
r.RHICmdDeferSkeletalLockAndFillToRHIThread=1
r.RHICmdPrePassDeferredContexts=1
r.RHICmdShadowDeferredContexts=1
r.RHICmdSpewParallelListBalance=0
r.RHICmdTranslucencyPassDeferredContexts=1
r.RHICmdUseDeferredContexts=1
r.RHICmdUseParallelAlgorithms=1
r.RHICmdVelocityPassDeferredContexts=1
r.RHICmdUseThread=1
r.RHICmdWidth=12
r.XGEShaderCompile=1
r.XGEShaderCompile.Mode=1
r.XGEShaderCompile.Xml.BatchGroupSize=256
r.XGEShaderCompile.Xml.BatchSize=16
r.XGEShaderCompile.Xml.JobTimeout=0.500000
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

[SystemSettings]
r.DynamicRes.OperationMode=0
r.DynamicRes.MinScreenPercentage=100
r.AllowOcclusionQueries=1
r.OptimizeForUAVPerformance=1
a.ParallelAnimUpdate=1
r.EnableAsyncComputeVolumetricFog=1
r.SkinCache.CompileShaders=1
r.FinishCurrentFrame=0
r.VSync=0
r.GTSyncType=1
r.MipMapLODBias=0
r.SkeletalMeshLODBias=-1
r.LandscapeLODBias=-1
r.ParticleLODBias=-1
s.AllowLevelRequestsWhileAsyncLoadingInMatch=1
tick.AllowAsyncComponentTicks=1
tick.AllowAsyncTickCleanup=1
tick.AllowAsyncTickDispatch=1
tick.AllowConcurrentTickQueue=1
tick.AnimationDelaysEndGroup=1
tick.HiPriSkinnedMeshes=1
r.AllowDownsampledStandardTranslucency=1
r.FastBlurThreshold=3
r.CustomDepth.Order=0
r.Streaming.AmortizeCPUToGPUCopy=1
s.AsyncLoadingThreadEnabled=1
r.StencilLODMode=2
r.DiffuseIndirect.Denoiser=0
r.Atmosphere=0
r.SeparateTranslucency=0
r.SeparateTranslucencyAutoDownsample=0
r.DefaultFeature.AmbientOcclusionStaticFraction=0
r.VolumetricCloud.HzbCulling=0
r.CapsuleShadows=0
r.CapsuleDirectShadows=0
r.CapsuleIndirectShadows=0
r.Shadow.Preshadows=0
LevelStreaming.UnloadNotVisibleLevelsWhenLowRAM=1
LevelStreaming.UnloadNotVisibleLevelsWhenLowRAMNormalizedPercentage=80
r.Streaming.ParallelRenderAssetsNumWorkgroups=4
r.Shadow.WholeSceneShadowCacheMb=682
r.Streaming.LimitPoolSizeToVRAM=0
r.Streaming.PoolSize=2048
r.Streaming.MaxTempMemoryAllowed=682
s.IoDispatcherCacheSizeMB=256 #Set 512 or 1024 if you have more VRAM
FX.AllowGPUParticles=1
FX.AllowAsyncTick=1
FX.EarlyScheduleAsync=1
FX.BatchAsync=1
FX.BatchAsyncBatchSize=8
FX.AllowGPUSorting=1
FX.FXAllowParticleMeshLODs=1
FX.AllowCulling=0

[/script/engine.engine]
r.OneFrameThreadLag=0
bAllowMultiThreadedShaderCompile=true
bSmoothFrameRate=0

[Core.Log]
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

[ConsoleVariables]
AllowAsyncRenderThreadUpdates=1
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=1
AllowAsyncRenderThreadUpdatesEditor=1
s.AsyncLoadingThreadEnabled=True

Setting r.Streaming.HLODStrategy=0 might help avoid getting low-resolution textures in the game if you are experiencing
It. For example, in a low-end graphics card, It might happen. Of course, this will reduce the FPS a bit, but it is a
good compromise. If you perceive some FPS drops in foggy areas, you can remove completely it by putting the following
code in [SystemSettings]:

# GC For Hogwarts legacy

[SystemSettings]
s.ContinuouslyIncrementalGCWhileLevelsPendingPurge=0

[/Script/Engine.GarbageCollectionSettings]
gc.MaxObjectsNotConsideredByGC=32
gc.LowMemory.TimeBetweenPurgingPendingLevels=15
gc.LowMemory.TimeBetweenPurgingPendingKillObjects=60
gc.TimeBetweenPurgingPendingKillObjects=60
gc.NumRetriesBeforeForcingGC=3
gc.MinDesiredObjectsPerSubTask=4
gc.LowMemory.MemoryThresholdMB=2048
gc.MultithreadedDestructionEnabled=1

# If you are having problems maintaining the RAM usage low, it is possible to add the following parameter:

[SystemSettings]
r.DiscardUnusedQuality=1

# IDEA 1 vs 2

## IDEA 1

[SystemSettings]
r.Streaming.MaxNumTexturesToStreamPerFrame=2 (you can try a higher value also, like 10 or 15 here)
r.Streaming.AmortizeCPUToGPUCopy=1
r.GTSyncType=1
r.TextureStreaming=1
r.Streaming.HLODStrategy=0
r.Streaming.LimitPoolSizeToVRAM=1
r.Streaming.PoolSize=4096

[ConsoleVariables]
AllowAsyncRenderThreadUpdates=1
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=1
AllowAsyncRenderThreadUpdatesEditor=1

## IDEA 2

[SystemSettings]
r.Streaming.MaxNumTexturesToStreamPerFrame=30 (try 20 or even lower to reduce GPU usage)
r.Streaming.AmortizeCPUToGPUCopy=1
r.GTSyncType=1
r.TextureStreaming=1
r.Streaming.LimitPoolSizeToVRAM=1
r.Streaming.PoolSize=4096

[ConsoleVariables]
AllowAsyncRenderThreadUpdates=1
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=1
AllowAsyncRenderThreadUpdatesEditor=1

# Thos dude might be onto smth

r.Streaming.LimitPoolSizeToVRAM=1
r.Streaming.PoolSize.VRAMPercentageClamp=512
It's not necessary. The game "r.streaming.poolsize=-1" by default meaning "use it all" this option limits the amount
they leave available. So with 512 set it will reserve 512mb. So if you have a 8gb card it will allow 7.5gb and 5.5gb for
a 6gb card automatically. There is also a "r.streaming.fixedpoolsize=1" (double check it's right) that tells it not to
dynamically adjust the poolsize so it will just allocate the max at launch (leaving the reserve available) it is handy
when you need some vram available for use with encoding or such for streaming.

In other words. Remove the poolsize, set fixedpoolsize and set the reserve option and it will show you exactly what I
mean. It's a "1 config" instead of having to manually set for each

# True Maximum Settings

[/script/engine.engine]
MinDesiredFrameRate=60
SmoothedFrameRateRange=(LowerBound=(Type="ERangeBoundTypes::Inclusive",Value=60),UpperBound=(Type="ERangeBoundTypes::
Exclusive",Value=144))

[/script/engine.renderersettings]
b.NetMoveEnableByDistance=-1
Compat.UseDXT5NormalMaps=0
r.AllowLandscapeShadows=1
r.AllowOcclusionQueries=0
r.HZBOcclusion=0
r.AllowPointLightCubemapShadows=1
r.AllowStaticLighting=0
r.AmbientOcclusion.Compute=1
r.AmbientOcclusion.Denoiser=3
r.AmbientOcclusion.FadeRadiusScale=2
r.AmbientOcclusion.HalfRes=1
r.AmbientOcclusion.Method=1
r.AmbientOcclusionLevels=25
r.AmbientOcclusionMaxQuality=100
r.AmbientOcclusionMipLevelFactor=0.5
r.AmbientOcclusionRadiusScale=1
r.AmbientOcclusionSampleSetQuality=3
r.AOAsyncBuildQueue=1
r.AOMaxViewDistance=16384
r.AOMinLevel=0
r.AOQuality=1
r.AOScatterInterpolation=1
r.AOSpecularOcclusionMode=2
r.BloomQuality=5
r.BlurGBuffer=-1
r.CapsuleShadows=1
r.ContactShadows=1
r.DBuffer=0
r.DefaultFeature.AmbientOcclusion=0
r.DefaultFeature.AmbientOcclusionStaticFraction=0
r.DefaultFeature.AutoExposure.ExtendDefaultLuminanceRange=0
r.DefaultFeature.AutoExposure.Method=0
r.DefaultFeature.AutoExposure=0
r.DefaultFeature.Bloom=0
r.DefaultFeature.LensFlare=0
r.DefaultFeature.MotionBlur=0
r.DepthOfFieldQuality=0
r.DetailMode=2
r.DFFullResolution=1
r.DistanceFieldAO=1
r.DistanceFieldGI=1
r.DistanceFieldShadowing=1
r.DontLimitOnBattery=0
r.EarlyZPassMovable=0
r.EyeAdaptation.MethodOveride=1
r.EyeAdaptationQuality=3
r.Gamma=1.2
r.GBuffer=1
r.GBufferFormat=5
r.GenerateLandscapeGIData=1
r.GenerateMeshDistanceFields=0
r.LensFlareQuality=0
r.LightFunctionQuality=3
r.LightPropagationVolume=1
r.LightShaftQuality=1
r.MaterialQualityLevel=1
r.MaxAnisotropy=16
r.MotionBlur.Max=0
r.MotionBlurQuality=0
r.OneFrameThreadLag=1
r.PostProcessAAQuality=5
r.RefractionQuality=3
r.RenderTargetPoolMin=4096
r.SceneColorFormat=5
r.SceneColorFringe.Max=0
r.SceneColorFringeQuality=0
r.SceneRenderTargetResizeMethod=0
r.ScreenPercentage=99
r.Shadow.CSM.MaxCascades=10
r.Shadow.CSM.MaxFarCascades=0
r.Shadow.AllowForegroundShadows=1
r.Shadow.CSM.TransitionScale=1.0
r.Shadow.DistanceScale=1.5
r.Shadow.DynamicInsetShadowEnable=1
r.Shadow.DynamicInsetShadowRange=16384
r.Shadow.FilterMethod=1
r.Shadow.GridTextureSize=4096 ; Going any higher seems unreasonable
r.Shadow.GridToCSM=2
r.Shadow.MaxCSMResolution=4096 ; This seems to be the engine max
r.Shadow.MaxResolution=8192 ; This seems to be the engine max
r.Shadow.PreShadowResolutionFactor=1.0
r.Shadow.RadiusThreshold=0
r.Shadow.TexelsPerPixel=16 ; Going any higher seems unreasonable
r.ShadowQuality=5
r.LightMaxDrawDistanceScale=1
r.SkinCaching=0
r.SSR.MaxRoughness=-1
r.SSR.Quality=3
r.SSR.Temporal=1
r.SSS.HalfRes=0
r.SSS.Quality=1
r.SSS.SampleSet=2
r.SSS.Scale=1
r.HighQualityLightMaps=1
foliage.DensityScale=0.1
grass.DensityScale=0.25
grass.Enable=1
r.Tonemapper.MergeWithUpscale.Mode=6
r.TemporalAA.AllowDownsampling=0
r.TemporalAACatmullRom=0
r.TemporalAAFilterSize=0.25
r.TemporalAACurrentFrameWeight=0.25
r.TemporalAASamples=4
r.TemporalAASharpness=1
r.TemporalAA.Upsampling=1
r.TemporalAAUpsampleFiltered=1
r.Tonemapper.GrainQuantization=0
r.Tonemapper.Quality=1
r.Tonemapper.Sharpen=1
r.TonemapperFilm=1
r.TonemapperGamma=Rec709
r.TranslucencyVolumeBlur=0
r.Upscale.Cylinder=0
r.Upscale.Panini.D=0.3
r.Upscale.Panini.S=0.005
r.Upscale.Panini.ScreenFit=1
r.Upscale.Quality=3
r.Upscale.Softness=0.3
r.VolumetricFog.GridPixelSize=4
r.VolumetricFog.GridSizeZ=64
r.VolumetricFog.HistoryMissSupersampleCount=16
r.VolumetricFog=1
r.StaticMeshLODDistanceScale=0.001
r.Streaming.Boost=1024
r.SkeletalMeshLODBias=0
r.Streaming.MipBias=0
r.MipMapLODBias=0
r.Streaming.PoolSize=0
r.Streaming.UsePerTextureBias=0
r.Streaming.FullyLoadUsedTextures=1
r.Streaming.UseAllMips=0
r.Streaming.DefragDynamicBounds=1
r.Streaming.LimitPoolSizeToVRAM=0
r.Streaming.ScaleTexturesByGlobalMyBias=0
r.Streaming.HLODStrategy=2
b.LevelStreamingDistanceRatioForLOD=1.0
b.LevelStreamingMaxLODLevel=3
r.LandscapeLODBias=0.5
r.ForceLOD=1
r.ViewDistanceScale=1.0
r.MobileMaxLoadedMips=1
r.Streaming.DropMips=2
r.Streaming.MinLevelTextureScreenSize=1024
r.HeightfieldGlobalIllumination=1
r.HeightfieldInnerBounceDistance=3000
r.HeightfieldOuterBounceDistanceScale=3
r.HeightfieldTargetUnitsPerTexel=200
r.AOComputeShaderNormalCalculation=1
r.AOFillGapsHighQuality=2
r.VPLMeshGlobalIllumination=1
r.LPV.RSMResolution=1024
r.Shadow.PerObject=1
r.CGV.CustomInterp=1
r.HBAO.Enable=1

[/script/engine.player]
ConfiguredInternetSpeed=100000
ConfiguredLanSpeed=100000

[/script/engine.localplayer]
AspectRatioAxisConstraint=AspectRatio_MaintainYFOV

[/script/onlinesubsystemutils.ipnetdriver]
MaxClientRate=100000
MaxInternetClientRate=100000