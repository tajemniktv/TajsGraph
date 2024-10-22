; Paste under Engine.ini after last line
; Settings for Better LoD + Optimizations and Stutter fixes

[SystemSettings]
; LOD
r.ViewDistanceScale=10
foliage.LODDistanceScale=2

; shadow
r.Shadow.DistanceScale=4
r.LightMaxDrawDistanceScale=2

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