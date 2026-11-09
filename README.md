# angulora

Angulora is a standalone C99 angle and phase library for deterministic modular phase handling, tick conversion, rotations and bounded trigonometric transforms.

## Scope

The package contains 25 public headers and 8 implementation units. It is portable C99, uses no mandatory heap or operating-system service, and keeps caller-owned state explicit where applicable.

## Build and test

```sh
cmake -S . -B build -DANGULORA_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure -C Release
```

## Validation status

Host validation is run during extraction. ESP32-S3 validation and isolated footprint measurements are pending unless explicitly recorded; no hardware result is claimed.

## Architecture and integration

Each operation is a separate stateless C99 translation unit with fixed-width
inputs and caller-owned outputs. Add `include/` to the include path and link
the `angulora` target, or compile only the required `src/*.c` files.

## API and configuration

Public headers are under `include/angulora/trigonometry`. Contracts define
phase units, fixed-point representation, rounding and invalid-input behavior.
`ANGULORA_BUILD_TESTS` controls host tests; there are no mandatory runtime or
third-party dependencies.

## Resource and performance status

No embedded flash, RAM, stack or cycle measurement is currently available.
These values are `PENDING`, not estimates. The implementation uses no heap,
threads or hidden global state.

## Limitations and security

The current host gate has no enabled CTest test; see `docs/TESTING.md`.
Applications must validate external numeric inputs before conversion. The
library does not provide cryptographic protection.

## Version, license and origin

The package is versioned independently and follows
semantic-versioning intent until 1.0. It is Apache License 2.0, copyright
Vanderhell. This repository is self-contained.

## Name and origin

Angulora combines angulus (angle) with ora (bounded measured form), reflecting predictable angular transforms.
Its technical scope is angle and phase transformation.

## License

Apache License 2.0, copyright Vanderhell. See LICENSE and NOTICE.
