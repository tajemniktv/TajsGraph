#include "Debug/TajsGraphDebugIO.h"

#include "Dom/JsonObject.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Policies/PrettyJsonPrintPolicy.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

namespace {

static TArray<TSharedPtr<FJsonValue>> ToJsonStringArray(const TArray<FString>& Items) {
    TArray<TSharedPtr<FJsonValue>> Out;
    Out.Reserve(Items.Num());
    for (const FString& Item : Items) {
        Out.Add(MakeShared<FJsonValueString>(Item));
    }
    return Out;
}

static TArray<TSharedPtr<FJsonValue>> ToJsonFloatMap(const TMap<FString, float>& Values) {
    TArray<TSharedPtr<FJsonValue>> Out;
    Out.Reserve(Values.Num());
    for (const TPair<FString, float>& Pair : Values) {
        TSharedPtr<FJsonObject> Item = MakeShared<FJsonObject>();
        Item->SetStringField(TEXT("key"), Pair.Key);
        Item->SetNumberField(TEXT("value"), Pair.Value);
        Out.Add(MakeShared<FJsonValueObject>(Item));
    }
    return Out;
}

static TArray<TSharedPtr<FJsonValue>> ToJsonStringMap(const TMap<FString, FString>& Values) {
    TArray<TSharedPtr<FJsonValue>> Out;
    Out.Reserve(Values.Num());
    for (const TPair<FString, FString>& Pair : Values) {
        TSharedPtr<FJsonObject> Item = MakeShared<FJsonObject>();
        Item->SetStringField(TEXT("key"), Pair.Key);
        Item->SetStringField(TEXT("value"), Pair.Value);
        Out.Add(MakeShared<FJsonValueObject>(Item));
    }
    return Out;
}

}

bool FTajsGraphDebugIO::WriteSnapshotJson(const FTajsGraphDebugSnapshot& Snapshot, const FString& FilePath) {
    return WriteJsonObject(MakeSnapshotJson(Snapshot), FilePath);
}

bool FTajsGraphDebugIO::WritePerfCsv(const TArray<FTajsGraphPerfSample>& Samples, const FString& FilePath) {
    TArray<FString> Lines;
    Lines.Reserve(Samples.Num() + 1);
    Lines.Add(TEXT("timestamp,frame_ms,fps,hitch,p95_ms,p99_ms,res_x,res_y,screen_pct,gpu_ms,cpu_ms"));
    for (const FTajsGraphPerfSample& Sample : Samples) {
        Lines.Add(FString::Printf(TEXT("%.6f,%.3f,%.2f,%d,%.3f,%.3f,%d,%d,%.2f,%.3f,%.3f"),
            Sample.Timestamp,
            Sample.FrameMs,
            Sample.FPS,
            Sample.bHitch ? 1 : 0,
            Sample.P95Ms,
            Sample.P99Ms,
            Sample.ResX,
            Sample.ResY,
            Sample.ScreenPct,
            Sample.GpuMs,
            Sample.CpuMs));
    }

    IFileManager::Get().MakeDirectory(*FPaths::GetPath(FilePath), true);
    return FFileHelper::SaveStringArrayToFile(Lines, *FilePath);
}

bool FTajsGraphDebugIO::WriteJsonObject(const TSharedPtr<FJsonObject>& JsonObject, const FString& FilePath) {
    if (!JsonObject.IsValid()) {
        return false;
    }

    FString Output;
    const TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&Output);
    if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer)) {
        return false;
    }

    IFileManager::Get().MakeDirectory(*FPaths::GetPath(FilePath), true);
    return FFileHelper::SaveStringToFile(Output, *FilePath);
}

TSharedPtr<FJsonObject> FTajsGraphDebugIO::MakeSnapshotJson(const FTajsGraphDebugSnapshot& Snapshot) {
    TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();
    Root->SetStringField(TEXT("snapshot_id"), Snapshot.SnapshotId);
    Root->SetStringField(TEXT("reason"), Snapshot.Reason);
    Root->SetNumberField(TEXT("timestamp"), Snapshot.Timestamp);
    Root->SetArrayField(TEXT("metadata"), ToJsonStringMap(Snapshot.Metadata));
    Root->SetObjectField(TEXT("current_render_state"), MakeRenderStateJson(Snapshot.CurrentRenderState));
    Root->SetObjectField(TEXT("current_scene_probe"), MakeSceneProbeJson(Snapshot.CurrentProbe));

    {
        TArray<TSharedPtr<FJsonValue>> PerfValues;
        PerfValues.Reserve(Snapshot.PerfHistory.Num());
        for (const FTajsGraphPerfSample& Sample : Snapshot.PerfHistory) {
            PerfValues.Add(MakeShared<FJsonValueObject>(MakePerfSampleJson(Sample)));
        }
        Root->SetArrayField(TEXT("perf_history"), PerfValues);
    }

    {
        TArray<TSharedPtr<FJsonValue>> RenderValues;
        RenderValues.Reserve(Snapshot.RenderHistory.Num());
        for (const FTajsGraphRenderStateSample& Sample : Snapshot.RenderHistory) {
            RenderValues.Add(MakeShared<FJsonValueObject>(MakeRenderStateJson(Sample)));
        }
        Root->SetArrayField(TEXT("render_history"), RenderValues);
    }

    {
        TArray<TSharedPtr<FJsonValue>> ProbeValues;
        ProbeValues.Reserve(Snapshot.ProbeHistory.Num());
        for (const FTajsGraphSceneProbeSample& Sample : Snapshot.ProbeHistory) {
            ProbeValues.Add(MakeShared<FJsonValueObject>(MakeSceneProbeJson(Sample)));
        }
        Root->SetArrayField(TEXT("probe_history"), ProbeValues);
    }

    {
        TArray<TSharedPtr<FJsonValue>> EventValues;
        EventValues.Reserve(Snapshot.RecentEvents.Num());
        for (const FTajsGraphDebugEvent& Event : Snapshot.RecentEvents) {
            EventValues.Add(MakeShared<FJsonValueObject>(MakeEventJson(Event)));
        }
        Root->SetArrayField(TEXT("events"), EventValues);
    }

    {
        TArray<TSharedPtr<FJsonValue>> DiffValues;
        DiffValues.Reserve(Snapshot.PostProcessDiff.Num());
        for (const FTajsGraphPPDiffEntry& Diff : Snapshot.PostProcessDiff) {
            TSharedPtr<FJsonObject> Item = MakeShared<FJsonObject>();
            Item->SetStringField(TEXT("field"), Diff.Field);
            Item->SetNumberField(TEXT("baseline"), Diff.Baseline);
            Item->SetNumberField(TEXT("current"), Diff.Current);
            Item->SetNumberField(TEXT("delta"), Diff.Delta);
            DiffValues.Add(MakeShared<FJsonValueObject>(Item));
        }
        Root->SetArrayField(TEXT("postprocess_diff"), DiffValues);
    }

    {
        TArray<TSharedPtr<FJsonValue>> ProvenanceValues;
        ProvenanceValues.Reserve(Snapshot.PostProcessProvenance.Num());
        for (const FTajsGraphPPProvenanceEntry& Entry : Snapshot.PostProcessProvenance) {
            TSharedPtr<FJsonObject> Item = MakeShared<FJsonObject>();
            Item->SetStringField(TEXT("field"), Entry.Field);
            Item->SetNumberField(TEXT("requested"), Entry.RequestedValue);
            Item->SetNumberField(TEXT("final"), Entry.FinalValue);
            Item->SetStringField(TEXT("last_writer_guess"), Entry.LastWriterGuess);
            Item->SetNumberField(TEXT("confidence"), Entry.Confidence);
            Item->SetBoolField(TEXT("inferred"), Entry.bInferred);
            ProvenanceValues.Add(MakeShared<FJsonValueObject>(Item));
        }
        Root->SetArrayField(TEXT("postprocess_provenance"), ProvenanceValues);
    }

    return Root;
}

TSharedPtr<FJsonObject> FTajsGraphDebugIO::MakePerfSampleJson(const FTajsGraphPerfSample& Sample) {
    TSharedPtr<FJsonObject> Json = MakeShared<FJsonObject>();
    Json->SetNumberField(TEXT("timestamp"), Sample.Timestamp);
    Json->SetNumberField(TEXT("frame_ms"), Sample.FrameMs);
    Json->SetNumberField(TEXT("fps"), Sample.FPS);
    Json->SetBoolField(TEXT("hitch"), Sample.bHitch);
    Json->SetNumberField(TEXT("p95_ms"), Sample.P95Ms);
    Json->SetNumberField(TEXT("p99_ms"), Sample.P99Ms);
    Json->SetNumberField(TEXT("res_x"), Sample.ResX);
    Json->SetNumberField(TEXT("res_y"), Sample.ResY);
    Json->SetNumberField(TEXT("screen_pct"), Sample.ScreenPct);
    Json->SetNumberField(TEXT("gpu_ms"), Sample.GpuMs);
    Json->SetNumberField(TEXT("cpu_ms"), Sample.CpuMs);
    return Json;
}

TSharedPtr<FJsonObject> FTajsGraphDebugIO::MakeRenderStateJson(const FTajsGraphRenderStateSample& Sample) {
    TSharedPtr<FJsonObject> Json = MakeShared<FJsonObject>();
    Json->SetNumberField(TEXT("timestamp"), Sample.Timestamp);
    Json->SetStringField(TEXT("rhi_name"), Sample.RHIName);
    Json->SetStringField(TEXT("feature_level"), Sample.FeatureLevel);
    Json->SetStringField(TEXT("shader_model"), Sample.ShaderModel);
    Json->SetStringField(TEXT("window_mode"), Sample.WindowMode);
    Json->SetNumberField(TEXT("view_rect_x"), Sample.ViewRectX);
    Json->SetNumberField(TEXT("view_rect_y"), Sample.ViewRectY);
    Json->SetNumberField(TEXT("view_rect_w"), Sample.ViewRectW);
    Json->SetNumberField(TEXT("view_rect_h"), Sample.ViewRectH);
    Json->SetNumberField(TEXT("res_x"), Sample.ResX);
    Json->SetNumberField(TEXT("res_y"), Sample.ResY);
    Json->SetNumberField(TEXT("screen_percentage"), Sample.ScreenPercentage);
    Json->SetBoolField(TEXT("upscaler_active"), Sample.bUpscalerActive);
    Json->SetBoolField(TEXT("lumen_active"), Sample.bLumenActive);
    Json->SetBoolField(TEXT("nanite_enabled"), Sample.bNaniteEnabled);
    Json->SetBoolField(TEXT("vsm_enabled"), Sample.bVSMEnabled);
    Json->SetNumberField(TEXT("showflags_hash"), Sample.ShowFlagsHash);
    Json->SetArrayField(TEXT("pp_values"), ToJsonFloatMap(Sample.FinalPPWatchedValues));
    Json->SetArrayField(TEXT("active_blendables"), ToJsonStringArray(Sample.ActiveBlendables));
    return Json;
}

TSharedPtr<FJsonObject> FTajsGraphDebugIO::MakeSceneProbeJson(const FTajsGraphSceneProbeSample& Sample) {
    TSharedPtr<FJsonObject> Json = MakeShared<FJsonObject>();
    Json->SetNumberField(TEXT("timestamp"), Sample.Timestamp);
    Json->SetNumberField(TEXT("nearby_lights_10m"), Sample.NearbyLights10m);
    Json->SetNumberField(TEXT("nearby_lights_25m"), Sample.NearbyLights25m);
    Json->SetNumberField(TEXT("nearby_lights_50m"), Sample.NearbyLights50m);
    Json->SetNumberField(TEXT("nearby_shadow_casters"), Sample.NearbyShadowCasters);
    Json->SetNumberField(TEXT("nearby_point_lights"), Sample.NearbyPointLights);
    Json->SetNumberField(TEXT("nearby_spot_lights"), Sample.NearbySpotLights);
    Json->SetNumberField(TEXT("nearby_rect_lights"), Sample.NearbyRectLights);
    Json->SetNumberField(TEXT("overlap_score"), Sample.OverlapScore);
    Json->SetNumberField(TEXT("primitive_count_approx"), Sample.PrimitiveCountApprox);
    Json->SetNumberField(TEXT("nanite_mesh_count_approx"), Sample.NaniteMeshCountApprox);
    Json->SetNumberField(TEXT("texture_pool_mb"), Sample.TexturePoolMB);
    Json->SetNumberField(TEXT("system_used_mb"), Sample.SystemUsedMB);
    Json->SetNumberField(TEXT("system_total_mb"), Sample.SystemTotalMB);

    TArray<TSharedPtr<FJsonValue>> Offenders;
    Offenders.Reserve(Sample.TopLights.Num());
    for (const FTajsGraphSceneProbeOffender& Offender : Sample.TopLights) {
        TSharedPtr<FJsonObject> Item = MakeShared<FJsonObject>();
        Item->SetStringField(TEXT("actor_name"), Offender.ActorName);
        Item->SetStringField(TEXT("actor_class"), Offender.ActorClass);
        Item->SetNumberField(TEXT("distance"), Offender.Distance);
        Item->SetNumberField(TEXT("radius"), Offender.Radius);
        Item->SetNumberField(TEXT("intensity"), Offender.Intensity);
        Item->SetBoolField(TEXT("casts_shadows"), Offender.bCastShadows);
        Item->SetNumberField(TEXT("score"), Offender.Score);
        Offenders.Add(MakeShared<FJsonValueObject>(Item));
    }
    Json->SetArrayField(TEXT("top_lights"), Offenders);

    return Json;
}

TSharedPtr<FJsonObject> FTajsGraphDebugIO::MakeEventJson(const FTajsGraphDebugEvent& Event) {
    TSharedPtr<FJsonObject> Json = MakeShared<FJsonObject>();
    Json->SetNumberField(TEXT("timestamp"), Event.Timestamp);
    Json->SetStringField(TEXT("category"), Event.Category);
    Json->SetStringField(TEXT("message"), Event.Message);
    Json->SetArrayField(TEXT("key_values"), ToJsonStringMap(Event.KeyValues));
    return Json;
}
