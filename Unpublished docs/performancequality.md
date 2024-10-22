; Paste under Engine.ini after last line
; Settings for Performance High Quality Mode
; Mid end PC required, assumes video card with at least 6gb vram

[SystemSettings]
; post processing
r.BlurGBuffer=-1
r.AmbientOcclusionLevels=2
r.AmbientOcclusionRadiusScale=1.0
r.DepthOfFieldQuality=2
r.RenderTargetPoolMin=400
r.LensFlareQuality=2
r.SceneColorFringeQuality=0
r.EyeAdaptationQuality=2
r.BloomQuality=5
r.FastBlurThreshold=7
r.Upscale.Quality=3
r.Tonemapper.GrainQuantization=1
r.HighQualityLightMaps=1
r.MotionBlurQuality=3
r.Tonemapper.Quality=5
r.ToneMapper.Sharpen=1
r.ParticleLightQuality=2

; LOD
r.ViewDistanceScale=10
r.SkeletalMeshLODBias=-1
r.MipMapLODBias=-1
foliage.LODDistanceScale=1
r.StaticMeshLODDistanceScale=1
r.SkeletalMeshLODRadiusScale=1
r.LandscapeLODBias=-1

; Foliage
foliage.DensityScale=0.8
grass.DensityScale=0.8
foliage.DitheredLOD=1

; shadow
r.ShadowQuality=5
r.Shadow.CSM.MaxCascades=10
r.Shadow.MaxResolution=2048
r.Shadow.MaxCSMResolution=2048
r.Shadow.RadiusThreshold=0.02
r.AllowLandscapeShadows=1
r.Shadow.DistanceScale=4
r.ContactShadows=1
; FilterMethod=0 is standard style. 1 = PCSS, for more diffused softer realistic shadows. Pick based on preference.
r.Shadow.FilterMethod=0
r.LightMaxDrawDistanceScale=5

; Adds cool bloom cross lighting
r.Bloom.Cross=0.6

; Texture
r.MaxAnisotropy=16

; effects quality
r.RefractionQuality=2
r.SSR.Quality=2
r.SceneColorFormat=4
r.DetailMode=2
r.TranslucencyVolumeBlur=1
r.MaterialQualityLevel=2
r.SSS.Quality=0

; DLSS - Enable to use DLSS when you need more performance. Turn off for better image and no motion blur.
; r.NGX.Enable=1
; DLSS sharpness 0-1
r.NGX.DLSS.Sharpness=0.5

; Temporal AA upgrades, only works with dlss off.
; r.PostProcessAAQuality=5
; r.TemporalAA.Algorithm=1
; r.TemporalAASharpness=1
; Increase to reduce ghosting at cost of performance: (4, 8=default, 16, 32, 64).
; r.TemporalAASamples=16
; Set 1 when using r.ScreenPercentage < 100 to use TAA for better upscaling.
; r.TemporalAA.Upsampling=1
; Use to scale with TAA. Set < 100 when FPS goals not met. 80 is generally decent for higher resolution.
; r.ScreenPercentage=80

; optimizations
r.Shaders.Optimize=1
r.GTSyncType=1

; might help with lag and stutter at cost of input lag
r.OneFrameThreadLag=1

; Allow tearing when you have a VRR capable monitor
r.D3D11.UseAllowTearing=1
r.D3D12.UseAllowTearing=1

; Extra stutter optimizations for streaming and GC
;gc.LowMemory.MemoryThresholdMB=8000
gc.TimeBetweenPurgingPendingKillObjects=30
;gc.NumRetriesBeforeForcingGC=5
;gc.MinDesiredObjectsPerSubTask=20
; Improves stutter but will cause glitches in minigame. (Crane games)
s.ForceGCAfterLevelStreamedOut=0
; Improves stutter but will cause glitches in minigame. (Crane games)
;s.ContinuouslyIncrementalGCWhileLevelsPendingPurge=0

[ConsoleVariables]
AllowAsyncRenderThreadUpdates=1
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=1
AllowAsyncRenderThreadUpdatesEditor=1

; Maintain aspec ratio for ultrawide monitors.
; Enable if you have wide monitor and you feel like zoom is too close.
[/Script/Engine.LocalPlayer]
; AspectRatioAxisConstraint=AspectRatio_MaintainYFOV
