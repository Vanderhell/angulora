# Angulora family guide

Angulora is a portable C99 collection for integer and fixed-point angle, phase, polar/cartesian, and stepper calculations. It has eight callable APIs and the `angulora_cpx_q` public value type. The library has no mandatory heap, operating system or external runtime dependency.

## Primitive map

| Family | Public API | Main use | Checked behavior |
|---|---|---|---|
| Bearing | `angulora_bearing2_q` | Bearing between two signed integer coordinates | CORDIC approximation, zero vector, full `int32_t` coordinates |
| Cartesian/polar | `angulora_cartesian_to_polar_q`, `angulora_cpx_to_polar_q`, `angulora_cpx_q` | Magnitude and phase conversion | nearest-even radius, Q0.32 phase, pointer validation and output preservation |
| Phase | `angulora_mod_phase_unwrap_i32`, `angulora_phase_to_tick_u32` | Shortest modular phase delta and phase-to-tick mapping | Antipode rejection; floor mapping and zero-cycle rejection |
| Norm | `angulora_q_hypot2` | Euclidean norm of same-scale signed values | nearest-even integer result and overflow rejection |
| Stepper | `angulora_stepper_angle_from_steps_q`, `angulora_stepper_microstep_phase_u32` | Calibrated signed angle and periodic microstep phase | nearest-even conversion and zero-calibration rejection |

## Representation selection

- Use Q0.32 for a normalized turn on the modulo-one-turn ring. It cannot distinguish zero turns from one complete turn.
- Use `angulora_mod_phase_unwrap_i32` only when the shortest signed phase step is intended and each step is unambiguous; a half-turn delta is rejected.
- Use Q16.16 signed degrees for calibrated motor angle. The function rejects results outside `int32_t`.
- `angulora_cartesian_to_polar_q` accepts signed Q15 component raws, but returns radius as `int32_t` to hold diagonal magnitudes above `INT16_MAX`.
- CORDIC phase values are approximations. The measured error distribution and test limitations are in `docs/PERFORMANCE.md` and `docs/TESTING.md`; they are not formal global bounds.

## Integration

1. Include only the needed public header.
2. Compile the matching `src/*.c` unit or link the `Angulora::Angulora` CMake target.
3. Check boolean returns before using output values.
4. Respect failure-preservation and output-pointer alias rules in the API reference.
5. Run the CMake/CTest suite in the same compiler configuration as the application.

## Coverage and resource evidence

All eight callable functions have direct checks in `tests/test_audit.c`; the ninth public entry is the `angulora_cpx_q` struct type and is checked through direct construction and use. This is deterministic boundary and differential testing, not exhaustive enumeration. Hardware cycles and linker-map memory figures are documented only when measured in `docs/PERFORMANCE.md`.
