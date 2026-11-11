# Performance and memory measurements

## ESP32-S3 environment

Current run: 2026-11-11, COM37, ESP32-S3 revision 0.2, MAC `3c:0f:02:d9:77:30`, 240 MHz CPU, 40 MHz crystal, ESP-IDF 5.5.1, Xtensa GCC 14.2.0. Validation and production C sources used `-O2`; firmware used USB Serial/JTAG and a custom partition table matching the read-only inspected device table. The app image was 202,016 bytes; SHA-256 `33ADD37EB6FCE05FA8746F170232B11220164385CE7F18ADE97C65FB262AE82F`. `esptool verify_flash` confirmed it at app0 offset `0x10000`. Only app0 was written during the run. Afterward, the previously read 1,310,720-byte app0 image was restored and verified byte-for-byte; NVS, bootloader, partition table, OTA metadata, app1 and data partitions were not written.

## Cycle method and results

Each scenario ran 21 measured batches of 1,000 calls. A volatile sink consumed each result; calls were through volatile function-pointer tables without LTO. A paired no-op batch measured the loop, indirect call and sink overhead; the reported per-call cycles are the median/minimum/maximum of the actual batch minus its paired no-op batch, divided by 1,000. Median no-op batch was 17,002 cycles (17.002 cycles/call). CPU clock remained at 240 MHz. Times convert as `cycles / 240 = microseconds`.

Inputs used for the two scenarios:

- Representative: bearing `(10,-20)->(12345,67890)`; Cartesian `(12345,-12345)`; complex `(1234567,-12345)`; phase delta `0x01234567`; phase tick `(0x87654321,1000003)`; hypot `(1234567,7654321)`; motor angle `(123 steps,200 steps/rev)`; microstep `(0x87654321,1000003)`.
- Boundary: bearing `(INT32_MIN,INT32_MIN)->(INT32_MAX,INT32_MAX)`; Cartesian `(INT16_MIN,INT16_MAX)`; complex `(INT32_MIN,INT32_MAX)`; phase delta `0->UINT32_MAX`; phase tick `(UINT32_MAX,UINT32_MAX)`; hypot `(INT32_MIN,0)` (failure path); motor angle `(INT32_MAX,1)` (unrepresentable/failure path); microstep `(UINT32_MAX-1,UINT32_MAX)`.

| Callable API | Representative median cycles (min..max) | Median us | Boundary median cycles (min..max) | Median us |
|---|---:|---:|---:|---:|
| `pc_bearing2_q` | 2712.49 (2711.48..2714.58) | 11.3020 | 2530.21 (2529.49..2531.94) | 10.5425 |
| `pc_cartesian_to_polar_q` | 3623.97 (3622.52..3625.01) | 15.0999 | 3786.93 (3785.52..3788.00) | 15.7789 |
| `pc_cpx_to_polar_q` | 3843.00 (3841.52..3844.04) | 16.0125 | 3819.99 (3818.52..3821.00) | 15.9166 |
| `pc_mod_phase_unwrap_i32` | 40.56 (40.00..42.44) | 0.1690 | 34.01 (33.00..34.43) | 0.1417 |
| `pc_phase_to_tick_u32` | 38.29 (38.00..39.01) | 0.1595 | 33.00 (32.98..34.01) | 0.1375 |
| `pc_q_hypot2` | 1131.45 (1131.05..1131.92) | 4.7144 | 1086.05 (1085.04..1087.47) | 4.5252 |
| `pc_stepper_angle_from_steps_q` | 248.01 (247.00..249.51) | 1.0334 | 336.01 (335.00..337.38) | 1.4000 |
| `pc_stepper_microstep_phase_u32` | 244.29 (244.00..246.51) | 1.0179 | 232.01 (231.00..233.38) | 0.9667 |

`pc_cpx_q` is a data type, not a callable operation, so it has no cycle measurement.

CORDIC error statistics below were also printed by the ESP32-S3 test run and matched the host run. The timing values are the current measured run; older values in prior audit notes are superseded by this table.

## CORDIC numerical error

Host GCC 16.1.0 deterministic test measurements compare Q0.32 output against an independent `atan2` reference. The campaign includes 100,000 seeded full-width inputs for bearing, 100,000 seeded Q15 inputs for Cartesian conversion, 100,000 seeded full-width complex values, and targeted axes, quadrants and integer boundaries. Error is circular absolute phase distance in Q0.32 codes.

| CORDIC API | Samples | Max observed error | Mean absolute error | RMS error | Worst input (x,y) | Equivalent max degrees |
|---|---:|---:|---:|---:|---|---:|
| `pc_bearing2_q` | 100,050 | 6 codes | 1.225657171 codes | 1.583741103 codes | `(860418290,-873776970)` delta | 5.029141903e-7 |
| `pc_cartesian_to_polar_q` | 100,048 | 6 codes | 1.239784903 codes | 1.597340065 codes | `(18063,-28779)` | 5.029141903e-7 |
| `pc_cpx_to_polar_q` | 100,048 | 7 codes | 1.248280825 codes | 1.605335787 codes | `(-1741304865,1092310586)` | 5.867332220e-7 |

These are observed maxima, not proven worst-case error bounds. The test’s host math reference uses the C math library `atan2` and nearest-even conversion; outputs are compared circularly so wraparound at one turn is handled correctly.

## Isolated memory and stack observations

- Linker object size for the eight production `src/*.c` objects at the current `-O2` build: **2,158 bytes** total (`1,662` `.flash.text` + `496` `.flash.rodata`, including referenced CORDIC constants). The linker map shows **0 bytes static RAM** across those objects. This is attributed to the library source objects, not ESP-IDF or the test harness.
- Whole validation firmware binary: **202,016 bytes**. This includes ESP-IDF and the test/benchmark harness and is not a library footprint.
- Main test task stack was configured to 12,288 bytes. The FreeRTOS watermark observed 10,644 bytes minimum free, corresponding to 1,644 bytes observed use during this run. A single watermark is not a maximum stack bound.
- The image header encoded 4 MB although the physical flash reports 16 MB; ESP-IDF warned about the discrepancy at boot. The tested app remained within the verified app0 partition. This is a validation-firmware header limitation, not a library measurement.
- No claim is made for other compiler options, link-time optimization, library subsets, or ESP32-S3 revisions.
