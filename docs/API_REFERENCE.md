# API reference

The public count is eight callable functions plus the `pc_cpx_q` value type (nine public header symbols total). The headers in `include/angulora/trigonometry/` are authoritative. All functions are deterministic C99 routines with caller-owned output storage and no allocation.

| Public symbol | Inputs and output | Success, failure, rounding and limits | Direct test |
|---|---|---|---|
| `pc_bearing2_q` | Origin and target `int32_t` coordinates; output is counter-clockwise Q0.32 turns from +X. Coordinates may be any `int32_t`; same-scale coordinates are assumed. | Equal points return phase zero. Null output fails. CORDIC phase approximation; measured error campaign is in `docs/PERFORMANCE.md`, and is not a formal bound. | `test_audit.c`: axes, quadrants, equal points, extremes, 100,000 seeded origin/target pairs. |
| `pc_cartesian_to_polar_q` | Signed Q15 `int16_t` x/y; outputs `int32_t` nearest-even radius in raw Q15 units and Q0.32 phase. | Zero vector maps to radius/phase zero. Null or aliased output pointers fail without writing either result. CORDIC phase; exact cardinal phases. Radius is same-scale Euclidean norm and can exceed `INT16_MAX`. | `test_audit.c`: full axis/quadrant boundary set, 100,000 seeded pairs, radius oracle, null/alias checks. |
| `pc_cpx_q` | Public struct value `{ int32_t real; int32_t imag; }`; not a callable operation. | Represents signed Cartesian integer components. The type itself has no failure or rounding behavior. | `test_audit.c`: direct construction and field checks; also exercised as input to complex conversion. |
| `pc_cpx_to_polar_q` | `pc_cpx_q` input; outputs nearest-integer `uint32_t` magnitude and counter-clockwise Q0.32 phase. | Output pointers must be non-null and distinct; failure preserves both. Zero maps to `(0,0)`. Magnitude is rounded to nearest-even. CORDIC phase. | `test_audit.c`: boundary Cartesian pairs, 100,000 seeded full-width pairs, magnitude/atan2 oracle, null/alias checks. |
| `pc_mod_phase_unwrap_i32` | Previous/current unsigned Q0.32 phase codes; output shortest signed `int32_t` delta on the modulo-2^32 ring. | Null output fails. Exactly antipodal difference `0x80000000` is ambiguous and fails; all other differences map to the shortest signed delta. Modular arithmetic applies only to phase deltas. | `test_audit.c`: wrap, positive/negative limits, antipode, null, 100,000 seeded phase pairs. |
| `pc_phase_to_tick_u32` | Q0.32 phase and nonzero ticks-per-cycle; output tick. | Null output or zero ticks-per-cycle fails without changing output. Maps by floor to `[0,ticks_per_cycle)`; no nearest rounding. | `test_audit.c`: phase/tick endpoints, `UINT32_MAX`, invalid count, 100,000 seeded pairs with an independent shift-add product reference. |
| `pc_q_hypot2` | Same-scale signed `int32_t` x/y raw values; output nearest-even Euclidean norm as `int32_t`. | Null output or a rounded norm above `INT32_MAX` fails; output is preserved on failure. Uses integer square root. | `test_audit.c`: signed boundaries, representability failure, null, 100,000 seeded pairs against `sqrtl`. |
| `pc_stepper_angle_from_steps_q` | Signed `int32_t` steps and nonzero `uint32_t` steps/revolution; output signed Q16.16 degrees. | Null output, zero calibration, or rounded value outside `int32_t` fails without changing output. Exact rational scaling, nearest-even rounding, symmetric for negative steps. | `test_audit.c`: positive/negative ties, zero calibration, extremes, 100,000 seeded cases against independent bitwise long division. |
| `pc_stepper_microstep_phase_u32` | Unsigned microstep index and nonzero steps/revolution; output periodic Q0.32 turn phase. | Null output or zero count fails preserving output. Index is reduced modulo steps/revolution, then phase is rounded nearest-even. Rounding is on the modulo phase representation. | `test_audit.c`: periodicity, boundary counts, invalid count, 100,000 seeded cases against independent bitwise long division. |

## Representation notes

- Q0.32 is a modulo-one-turn code: `0x00000000` is zero/full turn; `0x40000000` is one quarter-turn; `0x80000000` is a half-turn.
- Q15 Cartesian raw values are signed `int16_t`; their Euclidean radius is returned in `int32_t` because the radius may be greater than one component.
- Q16.16 motor angles are signed degrees scaled by 65,536.
- Fixed-point conversion rounding is nearest-even where stated. `pc_phase_to_tick_u32` is floor mapping.
- `pc_mod_phase_unwrap_i32` reports only a shortest signed phase difference. It does not infer a number of complete turns.

## Verification limits

CORDIC phase error statistics are measured on a deterministic, boundary-enriched campaign and are not proof of a global worst-case bound. Full input spaces are not exhaustively enumerated. Consult `docs/TESTING.md` and `docs/PERFORMANCE.md` for compiler, sanitizer, hardware and timing evidence.
