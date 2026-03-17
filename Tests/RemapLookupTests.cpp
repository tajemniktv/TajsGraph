// RemapLookupTests.cpp
// Stubbed tests for TryGetRemapTarget

#include "RemapLookupTests.h"
#include <iostream>

namespace TajsGraph::Tests
{

    std::vector<RemapLookupCase> GetRemapLookupSampleCases()
    {
        return {{"/Game/Props/Chair.uasset", true, "/Game/Props/Chair_v2.uasset"},
                {"/Game/Props/Unknown.uasset", false, ""},
                {"Mods/MyMod/Content/Old/mesh.uasset", true, "Mods/MyMod/Content/New/mesh_fixed.uasset"}};
    }

    void RunRemapLookupTests()
    {
#if 0
    // TODO: Replace with UE test macros and call TryGetRemapTarget
    auto cases = GetRemapLookupSampleCases();
    for (const auto &c : cases) {
        std::string gotTarget;
        bool found = false; // placeholder for TryGetRemapTarget(c.SourcePath, gotTarget)
        std::cout << "SOURCE: " << c.SourcePath << "\n";
        std::cout << "EXPECTED FOUND: " << (c.ExpectFound ? "yes" : "no") << "\n";
        std::cout << "EXPECTED TARGET: " << c.ExpectedTarget << "\n";
        std::cout << "FOUND: " << (found ? "yes" : "no") << "\n";
        std::cout << "GOT TARGET: " << gotTarget << "\n\n";
    }
#else
        // Intentionally disabled. Integrate into UE tests after build verification.
#endif
    }

} // namespace TajsGraph::Tests

#if WITH_DEV_AUTOMATION_TESTS
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ConfigRuntime.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemapLookup_AddFindRemoveClear,
                                 "TajsGraph.ConfigRuntime.RemapLookup.AddFindRemoveClear",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FRemapLookup_AddFindRemoveClear::RunTest(const FString &Parameters)
{
    FConfigRuntime &Runtime = FConfigRuntime::Get();

    Runtime.ClearAssetRemaps();

    const FString From = TEXT("/Game/Test/MyMesh.MyMesh");
    const FString To = TEXT("/Game/Test/MyMesh_Renamed.MyMesh_Renamed");
    Runtime.AddAssetRemap(From, To);

    FString Out;
    TestTrue(TEXT("FindAssetRemap should locate added mapping"), Runtime.FindAssetRemap(From, Out));
    TestEqual(TEXT("Found remap target matches added value"), Out, To);

    TMap<FString, FString> Exported;
    Runtime.ExportAssetRemaps(Exported);
    TestTrue(TEXT("Exported remaps should contain key"),
             Exported.Contains(FConfigRuntime::CanonicalizeRemapAssetPath(From)));
    TestEqual(TEXT("Exported remap value matches"), Exported[FConfigRuntime::CanonicalizeRemapAssetPath(From)], To);

    const bool bRemoved = Runtime.RemoveAssetRemap(From);
    TestTrue(TEXT("RemoveAssetRemap should report true when removed"), bRemoved);

    FString Dummy;
    TestFalse(TEXT("FindAssetRemap should not find removed mapping"), Runtime.FindAssetRemap(From, Dummy));

    Runtime.AddAssetRemap(TEXT("/Game/A.A"), TEXT("/Game/B.B"));
    Runtime.AddAssetRemap(TEXT("/Game/C.C"), TEXT("/Game/D.D"));
    Runtime.ClearAssetRemaps();
    TMap<FString, FString> Exported2;
    Runtime.ExportAssetRemaps(Exported2);
    TestEqual(TEXT("Exported remaps empty after clear"), Exported2.Num(), 0);

    return true;
}
#endif
