# Testing and validation

## Public surface and test method

`include/angulora/trigonometry/` contains eight callable C functions and the public `pc_cpx_q` struct type. The project count of nine public header entries includes that type; a struct is not a callable function. `tests/test_audit.c` directly exercises all eight functions and instantiates/checks the struct. CMake registers the test as `test_audit`, and the test uses explicit checks rather than `assert`, so Release/NDEBUG runs remain active.

The deterministic SplitMix64 sequence has a fixed seed. Each callable API receives at least 100,000 cases where applicable. `atan2`/`sqrt` references are independent of the production integer CORDIC and integer-square-root code. Exact phase/tick, signed phase difference, motor scaling and microstep calculations use mathematical integer references; division references are bit-at-a-time long division. Tests include axes, quadrants, equal and zero vectors, int16/int32 boundaries, invalid pointers/counts, output preservation, wrap boundaries, and nearest-even ties. This sampling does not exhaust the 32-bit input domains.

The CORDIC phase error campaign compares circular Q0.32 code distance against `atan2` converted to the documented phase representation. Current observed maxima and full sampled-run statistics are listed in `docs/PERFORMANCE.md`. They are empirical maxima, not proven global bounds.

## Host matrix

Local host results (C99; GCC/Clang `-Wall -Wextra -Wpedantic -Werror`; MSVC `/W4 /WX`):

| Compiler | Configuration | Result |
|---|---|---|
| GCC 16.1.0 (WinLibs UCRT) | Debug | PASS, 1/1 CTest |
| GCC 16.1.0 (WinLibs UCRT) | Release | PASS, 1/1 CTest |
| ClangCL 19.1.5 (Visual Studio toolset) | Debug | PASS, 1/1 CTest |
| ClangCL 19.1.5 (Visual Studio toolset) | Release | PASS, 1/1 CTest |
| MSVC 19.42.34444 | Debug | PASS, 1/1 CTest |
| MSVC 19.42.34444 | Release | PASS, 1/1 CTest |
| Native Windows Clang 22.1.8 | Debug/Release | NOT RUN: standalone link lacks `oldnames.lib` and `msvcrtd.lib`; Linux native Clang is covered by CI |
| Local MinGW GCC ASan / UBSan | - | NOT RUN: this GCC installation has no sanitizer runtime libraries; CI runs both |

The initial CTest invocation used the Visual Studio multi-config generator without `-C`; CTest reported the test as unavailable. This was a test invocation error, not a code failure; rerunning with `-C Debug` and using Ninja for the GCC single-config builds passed.

## CMake and CI

Reproduce a GCC release run:

```sh
cmake -G Ninja -S . -B build -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Release -DANGULORA_BUILD_TESTS=ON -DANGULORA_STRICT_WARNINGS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

GitHub Actions is configured for GCC and Clang Debug/Release, MSVC Debug/Release, ASan and UBSan. Run [43056702750](https://github.com/Vanderhell/angulora/actions/runs/43056702750) on commit `d7e9e66a48de8e939c673eafc0bc8566aa84a4ac` passed all 8 jobs. A documentation-only follow-up commit records this run; its final workflow result is reported after completion.

## ESP32-S3 hardware

**PASS, executed 2026-11-11:** `ANGULORA_ALL9_PASS|passed=9/9|callable=8|type=pc_cpx_q|first_failed_api=0` on ESP32-S3 COM37 (revision 0.2, MAC `3c:0f:02:d9:77:30`). All eight callable APIs ran the direct differential/boundary suite; the ninth public entry is the `pc_cpx_q` type, which is instantiated and checked directly. The firmware ran the same `tests/test_audit.c` source with at least 100,000 deterministic seeded cases per applicable numerical operation. It used ESP-IDF 5.5.1, Xtensa GCC 14.2.0, `-O2`, and 240 MHz.

Before flashing, the chip and 16 MB flash were identified and the partition table read back. It showed app0 at `0x10000`, size `0x140000`; the build used the same layout. The 202,016-byte app image (SHA-256 `33ADD37EB6FCE05FA8746F170232B11220164385CE7F18ADE97C65FB262AE82F`) was written only to app0 and verified. After the test, the previously read full app0 contents were restored and verified byte-for-byte; the board was returned to its pre-test app image. No bootloader, partition table, OTA metadata, NVS, app1 or data partition was written. ESP-IDF logged that the physical 16 MB flash exceeded the 4 MB size encoded in the image header; the warning did not prevent validation.

The checked-in validation project is `tests/esp32/`. With ESP-IDF 5.5.1 exported and its target toolchain installed, build from that directory with `idf.py build`. To execute on the board, write only `build/angulora_validation.bin` at the verified app0 offset; do not use a whole-flash command for this procedure. The source project uses the custom partition table matching the inspected device. Current cycle results, toolchain settings, isolated source-object footprint and stack watermark are in `docs/PERFORMANCE.md`.

## Test setup observations and limitations

During test development, a microstep periodicity expectation incorrectly allowed `uint32_t` addition to wrap before checking periodicity; it was fixed to test only non-wrapping sums. Random inputs were also changed to call the generator in sequenced statements, so every compiler receives the same deterministic vectors. Neither issue required a production change. One initial CTest command omitted `-C Debug` for a Visual Studio multi-config build; rerunning with the configuration selected passed.

No correctness test can prove every possible 32-bit input result by sampling. The phase error maxima are measured evidence only. Runtime timing varies with optimization, clock, flash/cache state and call-site context; the reported cycle counts are specific to the documented firmware and subtraction method. Stack watermark readings are not maximum-stack bounds.
