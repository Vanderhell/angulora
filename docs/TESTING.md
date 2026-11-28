# Testing and validation

## Public coverage

Angulora exposes eight callable C99 functions and one public `angulora_cpx_q` value type. The `test_audit` program directly calls all eight functions, constructs the type, and checks the independent arithmetic and phase references. It uses explicit return-code checks rather than `assert`, so Release builds retain them.

The separate `test_to_polar` test checks Cartesian and complex polar conversions against an independent exact integer square-root rounding oracle and an `atan2l` phase reference over boundary-enriched inputs and one million deterministic full-width samples. The radius oracle uses binary search over the integer square root and compares the exact remainder with the half-integer threshold; it avoids floating-point tie misclassification on platforms where `long double` has only double precision. It also checks conversion consistency and invalid-output behavior. Both test programs use deterministic fixed seeds. Sample maxima are empirical, not proofs over every input.

CTest registers 12 entries: two behavior suites, nine independent C99 public-header translation units, and one aggregate C++ header/link smoke. The C++ test confirms header compatibility and linkage; library implementation remains C99. Test-only math calls link libm on platforms that require it; the production target has no libm dependency.

## Host toolchain results

The strict local matrix is recorded here after execution of the productized source revision.

| Compiler | Configuration | Result |
|---|---|---|
| GCC 16.1.0 (WinLibs UCRT) | Debug | PASS, 12/12 |
| GCC 16.1.0 (WinLibs UCRT) | Release | PASS, 12/12 |
| GCC 13.3.0 (Ubuntu 24.04 / WSL) | Debug | PASS, 12/12 |
| GCC 13.3.0 (Ubuntu 24.04 / WSL) | Release | PASS, 12/12 |
| Clang 18.1.3 (Ubuntu 24.04 / WSL) | Debug | PASS, 12/12 |
| Clang 18.1.3 (Ubuntu 24.04 / WSL) | Release | PASS, 12/12 |
| ClangCL 19.1.5 (Visual Studio toolset) | Debug | PASS, 12/12 |
| ClangCL 19.1.5 (Visual Studio toolset) | Release | PASS, 12/12 |
| MSVC 19.42.34444 | Debug | PASS, 12/12 |
| MSVC 19.42.34444 | Release | PASS, 12/12 |
| GCC 13.3.0 AddressSanitizer (Ubuntu 24.04 / WSL) | Debug | PASS, 12/12 |
| GCC 13.3.0 UndefinedBehaviorSanitizer (Ubuntu 24.04 / WSL) | Debug | PASS, 12/12 |

All configurations used C99 with extensions disabled and strict warnings (`-Wall -Wextra -Wpedantic -Werror` or `/W4 /WX`). Both behavior tests use explicit failure paths. GitHub Actions run [46087454935](https://github.com/Vanderhell/angulora/actions/runs/46087454935) for source commit `96b35195db9ba9f35069bccc93d16991aaa93ba9` completed successfully: all six host configurations and both sanitizer jobs passed. A documentation-only follow-up is being validated separately.

## Reproduce

```sh
cmake -S . -B build -DANGULORA_BUILD_TESTS=ON -DANGULORA_STRICT_WARNINGS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

The suite can also be run with GCC, Clang, and MSVC Debug/Release configurations. Tests and examples are disabled by default when consumed as a dependency.

## Integration checks

The installed CMake package was consumed through `find_package(Angulora 0.2 CONFIG REQUIRED)`. Separate Windows C99 consumers also built and ran through `add_subdirectory()` and direct compilation of a selected production source file. All three consumers checked a known phase-to-tick result. Public C headers compile individually as strict C99 and together from one C++ translation unit in the CTest suite.

## ESP32-S3

Historical pre-namespace evidence: an ESP32-S3 revision 0.2 run printed `ANGULORA_ALL9_PASS` after checking eight callable APIs plus the `angulora_cpx_q` value type. It used ESP-IDF 5.5.1, Xtensa GCC 14.2.0, `-O2`, and 240 MHz. The same audit test source exercised the seeded numerical campaign. The previous run changed only its application partition after inspecting the layout and restored the original application image; persistent data and other partitions were not modified.

Current namespaced revision: **HW NOT VERIFIED**. No authorized ESP32-S3 was identified during the current session, so no current firmware build, flash, or execution is claimed. Current cycle and memory results are not inferred from the historical run. The dedicated ESP-IDF validation project is in `tests/esp32/` and can be built with ESP-IDF 5.5.1 and its target toolchain using `idf.py build`.

## Limits

- Deterministic sampling does not exhaust the complete signed 32-bit input spaces.
- CORDIC error maxima in [Performance](PERFORMANCE.md) are measured sample maxima, not global worst-case proofs.
- Native Windows Clang without the Visual Studio runtime was not used; ClangCL and hosted Linux Clang are the supported matrix checks.
- Historical hardware timings and object sizes apply to the documented run configuration, not every build option or application.
- A stack watermark is not a maximum-stack bound.
