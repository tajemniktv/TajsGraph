// CanonicalizationTests.h
// Lightweight scaffold for testing CanonicalizeRemapAssetPath
// TODO: Convert into Unreal Automation Tests (FAutomationTestBase) when integrating.

#pragma once

#include <string>
#include <vector>

namespace TajsGraph::Tests
{

    struct CanonicalizeCase
    {
        std::string InputPath;
        std::string ExpectedCanonical;
    };

    // Return a vector of sample test cases. Data also provided in TestData/canonicalize_remap_cases.json
    std::vector<CanonicalizeCase> GetCanonicalizeSampleCases();

    // Runner stub (non-built). Implement integration with UE test harness later.
    void RunCanonicalizationTests();

} // namespace TajsGraph::Tests
