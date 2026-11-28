# Contributing to Angulora

Keep changes within the documented angle, phase, polar/cartesian, norm, and motor calculation scope. Preserve C99 compatibility, public numeric representations, rounding rules, failure behavior, bounded execution, and deterministic results. Do not add mandatory heap, operating-system, threading, or third-party dependencies.

For algorithm changes, include a reproducible defect case or independent reference test, plus boundary and invalid-input checks. Keep checks active in Release. Run `cmake -S . -B build -DANGULORA_BUILD_TESTS=ON`, `cmake --build build --parallel`, and `ctest --test-dir build --output-on-failure`; state which compilers and sanitizers were actually used.

Keep each production function in its corresponding translation unit. Public API semantics may change only when a correctness defect is reproduced and a regression test demonstrates the required fix.
