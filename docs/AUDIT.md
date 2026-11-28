# Technical audit

## Scope and result

The product surface contains eight callable public functions and one public value type. All eight functions and the type were reviewed against their C headers and implementations. The current host audit suite directly calls all eight functions. No production arithmetic algorithm was changed in the namespace/productization work.

The normalized source-body comparison against the preserved pre-migration implementation passed 8/8. This verifies that the public symbol/header migration did not alter the implementation bodies. It is not a proof of correctness over all possible inputs.

| Classification | Finding | Resolution |
|---|---|---|
| BUG (public integration) | Public names and type used a shared historical namespace, and public headers were coupled to the historical path. | Migrated functions, type, macros, guards, tests, examples, and hardware validation sources to the `angulora_` namespace and `include/angulora/trigonometry/`. |
| BUG (build integration) | Production source list used `file(GLOB)`, tests were enabled by default, and there was no namespaced install/export package. | Replaced with explicit C99 source list, `Angulora::Angulora`, target-local flags, opt-in tests/examples, package config, install/export rules, and an external-consumer path. |
| BUG (coverage registration) | Independent Cartesian/complex polar differential test was not registered with CTest. | Added `test_to_polar` alongside `test_audit`; both use explicit error returns and deterministic references. |
| BUG (test oracle portability) | Floating-point `sqrtl` misclassified the norm for `(-460847552,1817114720)` because its exact norm is `1874642998.500000027...`; MSVC's `long double` rounding made the reference choose the lower integer while the exact nearest integer is the upper value. | Replaced radius oracles with independent binary-search integer square root and exact remainder comparison; added an explicit regression for the vector. No production algorithm changed. |
| BUG (strict Clang build) | Migrated C/C++ source units without final newlines fail Clang's `-Wnewline-eof` when warnings are errors. | Added final newlines to affected text sources and headers; GCC, Clang, MSVC, and ClangCL strict builds now pass. |
| OK | Fixed-point scaling, phase modulo arithmetic, signed boundaries, output preservation, and invalid pointer/calibration behavior match the documented contracts in the existing audit evidence. | Existing 100,000-case per-operation audit and direct edge cases remain enabled. |
| RISK | CORDIC phase outputs are approximate. | Error statistics are empirical sample results and are documented separately from proven bounds; no global error guarantee is claimed. |
| RISK | Hardware evidence predates the public namespace migration. | Historical result is retained; current revision remains `HW NOT VERIFIED`. |

## Direct API register

| Public entry | Direct test | Status |
|---|---|---|
| `angulora_bearing2_q` | `test_audit` | Covered |
| `angulora_cartesian_to_polar_q` | `test_audit`, `test_to_polar` | Covered |
| `angulora_cpx_q` value type | `test_audit`, public-header smoke | Covered |
| `angulora_cpx_to_polar_q` | `test_audit`, `test_to_polar` | Covered |
| `angulora_mod_phase_unwrap_i32` | `test_audit` | Covered |
| `angulora_phase_to_tick_u32` | `test_audit` | Covered |
| `angulora_q_hypot2` | `test_audit` | Covered |
| `angulora_stepper_angle_from_steps_q` | `test_audit` | Covered |
| `angulora_stepper_microstep_phase_u32` | `test_audit` | Covered |

Host compiler and CTest evidence is in [Testing](TESTING.md). Measured numerical error and historical hardware metrics are in [Performance](PERFORMANCE.md).
