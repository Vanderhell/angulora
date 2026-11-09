# angulora

Angulora is a standalone C99 angle and phase library for deterministic modular phase handling, tick conversion, rotations and bounded trigonometric transforms.

## Scope

This first extraction contains 25 family headers, 8 implementation units and 11 migrated test sources. It is portable C99, uses no mandatory heap or operating-system service, and keeps caller-owned state explicit where applicable.

## Build and test

```sh
cmake -S . -B build -DANGULORA_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure -C Release
```

## Validation status

Host validation is run during extraction. ESP32-S3 validation and isolated footprint measurements are pending unless explicitly recorded; no hardware result is claimed.

## Name and origin

Angulora combines angulus (angle) with ora (bounded measured form), reflecting predictable angular transforms.
It originates from PrimitiveCore's trigonometry/angles_and_transforms family.

## License

Apache License 2.0, copyright Vanderhell. See LICENSE and NOTICE.
