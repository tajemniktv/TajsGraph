// TestRunnerStub.cpp
// Example wiring showing how to invoke the test stubs. Disabled by default.

#include "CanonicalizationTests.h"
#include "RemapLookupTests.h"

int main_TestRunner_stub()
{
#if 0
    // WARNING: This function is intentionally disabled. To enable,
    // - Convert to UE automation test macros (recommended) OR
    // - Build as a standalone test binary and link with the module implementation.

    TajsGraph::Tests::RunCanonicalizationTests();
    TajsGraph::Tests::RunRemapLookupTests();
    return 0;
#else
    // TODO: hook into Unreal's automation framework (FAutomationTestBase) and remove
    // this manual runner. Leaving as a no-op to avoid accidental inclusion.
    return 0;
#endif
}
