Tests for TajsGraph (scaffold)
================================

This folder contains lightweight, non-built test scaffolding for two areas:

- Canonicalization of asset paths (CanonicalizeRemapAssetPath)
- Remap lookup (TryGetRemapTarget)

Files
- `CanonicalizationTests.h/.cpp` - test cases and a runner stub for canonicalization
- `RemapLookupTests.h/.cpp` - test cases and a runner stub for remap lookup
- `TestData/canonicalize_remap_cases.json` - sample input/output cases in JSON
- `TestRunnerStub.cpp` - example of how to wire both test suites together (guarded, not built)

How to use

- These are intentionally not wired into the mod's build. They are stubs and examples you can
  adapt into Unreal's automation/test harness (e.g., FAutomationTestBase) later.
- To integrate: convert the runner functions into `IMPLEMENT_SIMPLE_AUTOMATION_TEST` or
  `IMPLEMENT_COMPLEX_AUTOMATION_TEST` cases. Add the `.cpp` files to the module's Build.cs
  Test target or link them into a test-only target.

Running tests in the Editor (command line)

- Make sure the test `.cpp` files are compiled into your module (add them to the module's
  Build.cs or a test target). Tests using Editor functionality typically require the Editor
  or PIE context and must be compiled with `WITH_AUTOMATION_TESTS` enabled.
- Run a single automation test from the editor using ExecCmds. Example (adjust paths):

  "C:\\Path\\To\\UE5Editor.exe" "C:\\Path\\To\\YourProject.uproject" -ExecCmds="Automation RunTests TajsGraph.Canonicalization.Basic" -unattended -nullrhi -nopause

- Or run all tests in the TajsGraph group:

  "C:\\Path\\To\\UE5Editor.exe" "C:\\Path\\To\\YourProject.uproject" -ExecCmds="Automation RunTests TajsGraph" -unattended -nullrhi -nopause

- Notes:
  - If a test requires Editor-only APIs, ensure it is gated by `WITH_EDITOR`/`EditorContext` flags.
  - For CI runs prefer `-unattended -nullrhi` and capture logs via `Saved/Logs`.
  - If the automation test is a scaffold (see warnings in test output), implement the
    TODOs in the test source to call the real module functions and add assertions.

TODOs
- Integrate tests into the Unreal test harness after verifying behavior in an editor/runtime build.
- Replace the stub data-parsing with your preferred JSON library or UE's `FJson` utilities.
- Add more cases and error-path tests.
