// RemapLookupTests.h
// Lightweight scaffold for testing TryGetRemapTarget

#pragma once

#include <string>
#include <vector>

namespace TajsGraph::Tests
{

    struct RemapLookupCase
    {
        std::string SourcePath;
        bool ExpectFound;
        std::string ExpectedTarget; // valid only when ExpectFound==true
    };

    std::vector<RemapLookupCase> GetRemapLookupSampleCases();
    void RunRemapLookupTests();

} // namespace TajsGraph::Tests
