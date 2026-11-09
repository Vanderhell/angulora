# Testing

## Test summary

| Test category | Count | Platform | Result | Evidence |
|---|---:|---|---|---|
| Self-contained CTest tests | 0 | MSVC host | NOT TESTED | No test is registered by the current CMake gate. |
| Preserved test sources | 11 | Repository | PRESERVED | Sources remain available for future isolated coverage. |
| Hardware tests | 0 | ESP32-S3 | NOT TESTED | HW NOT VERIFIED. |

The available polar oracle is not enabled because its expected result is
outside the current product boundary. This is an explicit coverage gap, not a
passing result. Run `cmake -S . -B build -DANGULORA_BUILD_TESTS=ON`, build the
project, and inspect the CTest list before release.

Known gap: the product is not a release candidate until an independent angular
reference test is added and executed on the intended MCU.
