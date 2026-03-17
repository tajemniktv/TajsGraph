// CanonicalizationTests.cpp
// Stubbed tests for CanonicalizeRemapAssetPath

#include "CanonicalizationTests.h"
#include <iostream>

namespace TajsGraph::Tests
{

    std::vector<CanonicalizeCase> GetCanonicalizeSampleCases()
    {
        // Sample cases; keep in sync with TestData/canonicalize_remap_cases.json
        return {{"/Game/Props/../Textures/wood.uasset", "/Game/Textures/wood.uasset"},
                {"/Game/../Game/Maps/./LevelA.umap", "/Game/Maps/LevelA.umap"},
                {"Mods/MyMod/Content/Textures/../Meshes/cube.uasset", "Mods/MyMod/Content/Meshes/cube.uasset"}};
    }

    void RunCanonicalizationTests()
    {
#if 0
    // TODO: Replace with UE test macros and link into the automation test framework.
    using namespace std;
    auto cases = GetCanonicalizeSampleCases();
    for (const auto &c : cases) {
        // TODO: call into actual implementation: CanonicalizeRemapAssetPath(c.InputPath)
        string got = ""; // placeholder
        cout << "INPUT: " << c.InputPath << "\n";
        cout << "EXPECTED: " << c.ExpectedCanonical << "\n";
        cout << "GOT: " << got << "\n\n";
    }
#else
        // Runner is intentionally disabled to avoid linking into the mod binary. Enable
        // and adapt to UE tests after build verification.
#endif
    }

} // namespace TajsGraph::Tests
