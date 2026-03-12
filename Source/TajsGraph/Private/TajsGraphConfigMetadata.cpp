#include "TajsGraphModuleInternal.h"
#include "TajsGraphSmlSettings.h"
#include "Configuration/ConfigProperty.h"
#include "Configuration/Properties/ConfigPropertyBool.h"
#include "Configuration/Properties/ConfigPropertyFloat.h"
#include "Configuration/Properties/ConfigPropertyInteger.h"
#include "Configuration/Properties/ConfigPropertySection.h"
#include "Configuration/Properties/ConfigPropertyString.h"

namespace TajsGraphInternal
{
    namespace
    {
        static ETajsGraphSettingType ResolveDescriptorType(const FString& Section, const FString& Key, const UConfigProperty* Property)
        {
            if (Section == TEXT("PPV") && Key == TEXT("IndirectLightingColor"))
            {
                return ETajsGraphSettingType::Color;
            }
            if (Cast<UConfigPropertyBool>(Property))
            {
                return ETajsGraphSettingType::Bool;
            }
            if (Cast<UConfigPropertyInteger>(Property))
            {
                return ETajsGraphSettingType::Int;
            }
            if (Cast<UConfigPropertyFloat>(Property))
            {
                return ETajsGraphSettingType::Float;
            }
            return ETajsGraphSettingType::String;
        }

        static FString ResolveDefaultValue(const UConfigProperty* Property)
        {
            if (const UConfigPropertyBool* BoolProperty = Cast<UConfigPropertyBool>(Property))
            {
                return BoolProperty->Value ? TEXT("true") : TEXT("false");
            }
            if (const UConfigPropertyInteger* IntProperty = Cast<UConfigPropertyInteger>(Property))
            {
                return FString::FromInt(IntProperty->Value);
            }
            if (const UConfigPropertyFloat* FloatProperty = Cast<UConfigPropertyFloat>(Property))
            {
                return FString::SanitizeFloat(FloatProperty->Value);
            }
            if (const UConfigPropertyString* StringProperty = Cast<UConfigPropertyString>(Property))
            {
                return StringProperty->Value;
            }
            return FString();
        }

        static void InferBounds(const FString& Section, const FString& Key, float& OutMin, float& OutMax, float& OutStep)
        {
            OutMin = 0.0f;
            OutMax = 0.0f;
            OutStep = 0.0f;

            if (Section == TEXT("DebugHub"))
            {
                if (Key == TEXT("PerfSampleInterval") || Key == TEXT("RenderSampleInterval") || Key == TEXT("SceneProbeInterval") || Key == TEXT("LastStateWriteInterval"))
                {
                    OutMin = 0.0f;
                    OutMax = 30.0f;
                    OutStep = 0.05f;
                    return;
                }
                if (Key == TEXT("HitchThresholdMs"))
                {
                    OutMin = 1.0f;
                    OutMax = 1000.0f;
                    OutStep = 1.0f;
                    return;
                }
                if (Key == TEXT("HitchPerSignatureCooldownSec"))
                {
                    OutMin = 0.0f;
                    OutMax = 600.0f;
                    OutStep = 1.0f;
                    return;
                }
                if (Key == TEXT("HistorySecondsForSnapshot"))
                {
                    OutMin = 1.0f;
                    OutMax = 300.0f;
                    OutStep = 1.0f;
                    return;
                }
                if (Key == TEXT("MaxPerfSamples"))
                {
                    OutMin = 60.0f;
                    OutMax = 200000.0f;
                    OutStep = 60.0f;
                    return;
                }
                if (Key == TEXT("MaxRenderSamples"))
                {
                    OutMin = 60.0f;
                    OutMax = 50000.0f;
                    OutStep = 60.0f;
                    return;
                }
                if (Key == TEXT("MaxProbeSamples"))
                {
                    OutMin = 30.0f;
                    OutMax = 10000.0f;
                    OutStep = 30.0f;
                    return;
                }
                if (Key == TEXT("MaxEvents"))
                {
                    OutMin = 64.0f;
                    OutMax = 20000.0f;
                    OutStep = 32.0f;
                    return;
                }
                if (Key == TEXT("MaxCopiedHitchSnapshots"))
                {
                    OutMin = 0.0f;
                    OutMax = 50.0f;
                    OutStep = 1.0f;
                    return;
                }
            }

            if (Section == TEXT("Visualization") && Key == TEXT("ModeSwitchCooldownMs"))
            {
                OutMin = 0.0f;
                OutMax = 5000.0f;
                OutStep = 10.0f;
                return;
            }

            if (Section == TEXT("Rendering"))
            {
                if (Key == TEXT("MaximumActiveLights"))
                {
                    OutMin = 0.0f;
                    OutMax = 5000.0f;
                    OutStep = 1.0f;
                    return;
                }
                if (Key == TEXT("LightPoolRelevancy"))
                {
                    OutMin = 0.0f;
                    OutMax = 100.0f;
                    OutStep = 0.1f;
                    return;
                }
            }

            if (Section != TEXT("PPV"))
            {
                return;
            }

            if (Key == TEXT("LumenSceneLightingQuality") || Key == TEXT("LumenFinalGatherQuality") || Key == TEXT("LumenReflectionQuality"))
            {
                OutMin = 0.0f;
                OutMax = 8.0f;
                OutStep = 0.1f;
                return;
            }
            if (Key == TEXT("LumenSceneDetail"))
            {
                OutMin = 0.0f;
                OutMax = 32.0f;
                OutStep = 0.1f;
                return;
            }
            if (Key == TEXT("LumenSceneViewDistance") || Key == TEXT("LumenMaxTraceDistance"))
            {
                OutMin = 100.0f;
                OutMax = 1000000.0f;
                OutStep = 100.0f;
                return;
            }
            if (Key == TEXT("LumenSceneLightingUpdateSpeed") || Key == TEXT("LumenFinalGatherLightingUpdateSpeed"))
            {
                OutMin = 0.01f;
                OutMax = 8.0f;
                OutStep = 0.01f;
                return;
            }
            if (Key == TEXT("LumenDiffuseColorBoost"))
            {
                OutMin = 0.0f;
                OutMax = 8.0f;
                OutStep = 0.01f;
                return;
            }
            if (Key == TEXT("LumenSkylightLeaking"))
            {
                OutMin = 0.0f;
                OutMax = 1.0f;
                OutStep = 0.01f;
                return;
            }
            if (Key == TEXT("LumenFullSkylightLeakingDistance"))
            {
                OutMin = 0.0f;
                OutMax = 200000.0f;
                OutStep = 10.0f;
                return;
            }
            if (Key == TEXT("LumenSurfaceCacheResolution"))
            {
                OutMin = 0.25f;
                OutMax = 4.0f;
                OutStep = 0.01f;
                return;
            }
            if (Key == TEXT("IndirectLightingIntensity"))
            {
                OutMin = 0.0f;
                OutMax = 20.0f;
                OutStep = 0.01f;
                return;
            }
            if (Key == TEXT("LumenRayLightingMode") || Key == TEXT("LumenFrontLayerTranslucencyReflections"))
            {
                OutMin = 0.0f;
                OutMax = 1.0f;
                OutStep = 1.0f;
                return;
            }
            if (Key == TEXT("LumenMaxReflectionBounces"))
            {
                OutMin = 1.0f;
                OutMax = 64.0f;
                OutStep = 1.0f;
            }
        }

        static bool IsDangerous(const FString& Section, const FString& Key)
        {
            return Section == TEXT("Debug") || Section == TEXT("DebugHub") || Section == TEXT("Visualization") ||
                Key.Contains(TEXT("Async")) || Key == TEXT("UnoptimizedLumen") || Key == TEXT("EnableDLSSFG") || Key == TEXT("EnableReflex");
        }
    }

    int32 GetSettingSchemaVersion()
    {
        return 3;
    }

    const TArray<FTajsGraphSettingDescriptor>& GetKnownSettingDescriptors()
    {
        static const TArray<FTajsGraphSettingDescriptor> Descriptors = []()
        {
            TArray<FTajsGraphSettingDescriptor> Out;
            const UConfigPropertySection* RootSection = TajsGraphSmlSettings::GetRootSection(nullptr, true);
            if (!RootSection)
            {
                return Out;
            }

            for (const TPair<FString, UConfigProperty*>& SectionPair : RootSection->SectionProperties)
            {
                const UConfigPropertySection* SectionProperty = Cast<UConfigPropertySection>(SectionPair.Value);
                if (!SectionProperty)
                {
                    continue;
                }

                const FString& SectionName = SectionPair.Key;
                for (const TPair<FString, UConfigProperty*>& PropertyPair : SectionProperty->SectionProperties)
                {
                    const UConfigProperty* Property = PropertyPair.Value;
                    if (!Property || Cast<UConfigPropertySection>(Property))
                    {
                        continue;
                    }

                    FTajsGraphSettingDescriptor Descriptor;
                    Descriptor.Section = SectionName;
                    Descriptor.Key = PropertyPair.Key;
                    Descriptor.Id = SectionName + TEXT(".") + PropertyPair.Key;
                    Descriptor.Category = SectionName;
                    Descriptor.Subcategory = (SectionName == TEXT("PPV") && PropertyPair.Key.StartsWith(TEXT("Override"))) ? TEXT("Overrides") : TEXT("General");
                    Descriptor.Type = ResolveDescriptorType(SectionName, PropertyPair.Key, Property);
                    Descriptor.DefaultValue = ResolveDefaultValue(Property);
                    Descriptor.ApplyPolicy = Property->bRequiresWorldReload ? ETajsGraphApplyPolicy::RequiresReload : ETajsGraphApplyPolicy::Live;
                    Descriptor.bDangerous = IsDangerous(SectionName, PropertyPair.Key) || Property->bHidden;
                    Descriptor.Tooltip = Property->Tooltip.IsEmpty()
                        ? FString::Printf(TEXT("%s.%s"), *SectionName, *PropertyPair.Key)
                        : Property->Tooltip.ToString();
                    InferBounds(SectionName, PropertyPair.Key, Descriptor.MinValue, Descriptor.MaxValue, Descriptor.Step);
                    Out.Add(MoveTemp(Descriptor));
                }
            }

            Out.Sort([](const FTajsGraphSettingDescriptor& A, const FTajsGraphSettingDescriptor& B)
            {
                return A.Id < B.Id;
            });
            return Out;
        }();

        return Descriptors;
    }

}
