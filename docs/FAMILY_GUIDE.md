# Angulora family guide

## What this family solves

**Trigonometry and angle transforms.** Deterministic fixed-point angle, phase, polar/cartesian and motor-angle calculations.

This guide describes the complete public family as an independent embedded-C product. Each primitive remains separately linkable; select only the headers and source units required by the application.

## When to use this family

Use Angulora when the application needs the operations listed below and requires deterministic behavior, explicit caller-owned memory, and no mandatory operating-system services.

## Family at a glance

```mermaid
flowchart LR
    A[Application input] --> B[Validate representation and bounds]
    B --> C[Select one public primitive]
    C --> D[Check status or output contract]
    D --> E[Use result with documented units]
```

## Primitive map

| Public header | Symbol | Purpose and contract | Validation source |
|---|---|---|---|
| `include/angulora/trigonometry/pc_bearing2_q.h` | `pc_bearing2_q` | CORDIC estimate of bearing from an int32 origin to target as unsigned Q0.32 turns CCW from +X. Equal points return zero. Coordinates need not share a specific Q scale. Differential tests observed <= 16 phase-code error; this empirical limit is not a formal global bound. | No enabled test source was found by symbol search. |
| `include/angulora/trigonometry/pc_cartesian_to_polar_q.h` | `pc_cartesian_to_polar_q` | Convert signed-Q15 Cartesian coordinates to nearest-even Q15 radius and counter-clockwise Q0.32 turns. The zero vector has phase zero. The phase uses bounded integer CORDIC approximation; observed phase error is <= 8 codes in the recorded differential campaign (not an exhaustive global bound). | tests/from_polar.c |
| `include/angulora/trigonometry/pc_cpx_q.h` | `pc_cpx_q` | The public header contains the authoritative signature and input/output contract. | No enabled test source was found by symbol search. |
| `include/angulora/trigonometry/pc_cpx_to_polar_q.h` | `pc_cpx_to_polar_q` | Return nearest-integer uint32 magnitude and approximate Q0.32 CCW phase. Output pointers must be distinct; failure preserves both outputs. | tests/to_polar.c |
| `include/angulora/trigonometry/pc_mod_phase_unwrap_i32.h` | `pc_mod_phase_unwrap_i32` | Shortest signed delta on the uint32_t phase ring; antipodes are ambiguous. | No enabled test source was found by symbol search. |
| `include/angulora/trigonometry/pc_phase_to_tick_u32.h` | `pc_phase_to_tick_u32` | Map unsigned Q0.32 phase [0,1) to a tick in [0,ticks_per_cycle). | tests/test_exhaustive.c |
| `include/angulora/trigonometry/pc_q_hypot2.h` | `pc_q_hypot2` | Nearest-even Euclidean norm of same-scale signed Q raw values. False on null output or int32 result overflow; failure preserves *out. | No enabled test source was found by symbol search. |
| `include/angulora/trigonometry/pc_stepper_angle_from_steps_q.h` | `pc_stepper_angle_from_steps_q` | Convert signed integer steps to signed Q16.16 degrees using caller calibration. Zero steps/revolution or unrepresentable angle fails. | No enabled test source was found by symbol search. |
| `include/angulora/trigonometry/pc_stepper_microstep_phase_u32.h` | `pc_stepper_microstep_phase_u32` | Map an unsigned microstep index to periodic unsigned Q0.32 revolution phase, nearest-even. steps_per_revolution must be nonzero. | tests/driver.c |

## Selection guidance

- Choose the primitive whose width, representation, rounding, saturation, wrapping, and error contract match the application.
- Do not substitute a similarly named function without checking the public header.
- Allocate state and buffers in the caller; respect documented aliasing and output-preservation rules.
- Treat entries marked without a validation source as requiring additional product-level tests before release.

## Integration checklist

1. Include only the required public header.
2. Compile the matching source translation unit or use the product CMake target.
3. Check every boolean/status return before consuming an output.
4. Add application-specific boundary tests for units, widths, invalid inputs, and overflow behavior.
5. Run the documented host tests before target integration.

## Complexity and footprint

The product is split into independent translation units. Exact flash, RAM, stack, and cycle costs depend on the selected functions, compiler, optimization, and MCU; unmeasured values remain explicitly marked PENDING in the testing documentation.

## Validation boundary

The API reference links each public entry to its implementation and available validation source. Hardware status is reported only when reproducible target evidence exists.
