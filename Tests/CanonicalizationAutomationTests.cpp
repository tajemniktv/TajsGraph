// CanonicalizationAutomationTests.cpp
// Converted scaffold into an Unreal Automation Test stub.

#if WITH_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "CanonicalizationTests.h"
#include "CoreMinimal.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCanonicalizationAutomationTest, "TajsGraph.Canonicalization.Basic",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

bool FCanonicalizationAutomationTest::RunTest(const FString &Parameters)
{
    using namespace TajsGraph::Tests;

    // Use the existing scaffold data. These are std::string pairs converted to FString for UE tests.
    auto Cases = GetCanonicalizeSampleCases();

#if defined(TAJSGRAPH_CANONICALIZE_DECL)
    // If your module exposes a canonicalization function, define TAJSGRAPH_CANONICALIZE_DECL
    // (e.g., via Build.cs or a public header) so this test will call the real implementation.
    extern FString CanonicalizeRemapAssetPath(const FString &InPath);

    for (const auto &C : Cases)
    {
        const FString Input = FString(C.InputPath.c_str());
        const FString Expected = FString(C.ExpectedCanonical.c_str());
        const FString Got = CanonicalizeRemapAssetPath(Input);

        if (Got != Expected)
        {
            AddError(FString::Printf(TEXT("Canonicalization mismatch:\n  INPUT: %s\n  EXPECTED: %s\n  GOT: %s"), *Input,
                                     *Expected, *Got));
        }
        else
        {
            AddInfo(FString::Printf(TEXT("OK: %s -> %s"), *Input, *Got));
        }
    }
#else
    // Test scaffold: the real canonicalization implementation is not linked into the test yet.
    AddWarning(TEXT("CanonicalizeRemapAssetPath not available; this automation test is a scaffold.\n"
                    "Define TAJSGRAPH_CANONICALIZE_DECL and link the implementation to enable full checks."));

    // To allow enabling the test early (without failing CI), succeed while signalling it's a TODO.
#endif

    return true;
}

#endif // WITH_AUTOMATION_TESTS
