# Angulora API reference

This reference explains every public header in the Angulora product. It is generated from the current headers and test sources; the header remains authoritative when a detail conflicts.

## Reading the entries
Each entry states the public symbol, implementation unit, contract, edge/failure behavior and a call extracted from the validating test source. The examples are evidence-oriented; adapt buffer ownership and numeric units to the application.

## include/angulora/trigonometry/pc_bearing2_q.h

**Public symbol:** pc_bearing2_q

**Implementation unit:** src/pc_bearing2_q.c

**Purpose and contract:** CORDIC estimate of bearing from an int32 origin to target as unsigned Q0.32 turns CCW from +X. Equal points return zero. Coordinates need not share a specific Q scale. Differential tests observed <= 16 phase-code error; this empirical limit is not a formal global bound.

**Validation source:** No enabled test source was found by symbol search.

**Test-derived call:**
```c
No test-derived call was found.;
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/angulora/trigonometry/pc_cartesian_to_polar_q.h

**Public symbol:** pc_cartesian_to_polar_q

**Implementation unit:** src/pc_cartesian_to_polar_q.c

**Purpose and contract:** Convert signed-Q15 Cartesian coordinates to nearest-even Q15 radius and counter-clockwise Q0.32 turns. The zero vector has phase zero. The phase uses bounded integer CORDIC approximation; observed phase error is <= 8 codes in the recorded differential campaign (not an exhaustive global bound).

**Validation source:** tests/from_polar.c

**Test-derived call:**
```c
pc_cartesian_to_polar_q((int16_t)x, (int16_t)y, &radius, &phase);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/angulora/trigonometry/pc_cpx_q.h

**Public symbol:** pc_cpx_q

**Implementation unit:** src/pc_cpx_q.c

**Purpose and contract:** The public header contains the authoritative signature and input/output contract.

**Validation source:** No enabled test source was found by symbol search.

**Test-derived call:**
```c
No test-derived call was found.;
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/angulora/trigonometry/pc_cpx_to_polar_q.h

**Public symbol:** pc_cpx_to_polar_q

**Implementation unit:** src/pc_cpx_to_polar_q.c

**Purpose and contract:** Return nearest-integer uint32 magnitude and approximate Q0.32 CCW phase. Output pointers must be distinct; failure preserves both outputs.

**Validation source:** tests/to_polar.c

**Test-derived call:**
```c
pc_cpx_to_polar_q(value, &radius, &phase\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/angulora/trigonometry/pc_mod_phase_unwrap_i32.h

**Public symbol:** pc_mod_phase_unwrap_i32

**Implementation unit:** src/pc_mod_phase_unwrap_i32.c

**Purpose and contract:** Shortest signed delta on the uint32_t phase ring; antipodes are ambiguous.

**Validation source:** No enabled test source was found by symbol search.

**Test-derived call:**
```c
No test-derived call was found.;
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/angulora/trigonometry/pc_phase_to_tick_u32.h

**Public symbol:** pc_phase_to_tick_u32

**Implementation unit:** src/pc_phase_to_tick_u32.c

**Purpose and contract:** Map unsigned Q0.32 phase [0,1) to a tick in [0,ticks_per_cycle).

**Validation source:** tests/test_exhaustive.c

**Test-derived call:**
```c
pc_phase_to_tick_u32(q,8,&t)&&t==7);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/angulora/trigonometry/pc_q_hypot2.h

**Public symbol:** pc_q_hypot2

**Implementation unit:** src/pc_q_hypot2.c

**Purpose and contract:** Nearest-even Euclidean norm of same-scale signed Q raw values. False on null output or int32 result overflow; failure preserves *out.

**Validation source:** No enabled test source was found by symbol search.

**Test-derived call:**
```c
No test-derived call was found.;
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/angulora/trigonometry/pc_stepper_angle_from_steps_q.h

**Public symbol:** pc_stepper_angle_from_steps_q

**Implementation unit:** src/pc_stepper_angle_from_steps_q.c

**Purpose and contract:** Convert signed integer steps to signed Q16.16 degrees using caller calibration. Zero steps/revolution or unrepresentable angle fails.

**Validation source:** No enabled test source was found by symbol search.

**Test-derived call:**
```c
No test-derived call was found.;
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/angulora/trigonometry/pc_stepper_microstep_phase_u32.h

**Public symbol:** pc_stepper_microstep_phase_u32

**Implementation unit:** src/pc_stepper_microstep_phase_u32.c

**Purpose and contract:** Map an unsigned microstep index to periodic unsigned Q0.32 revolution phase, nearest-even. steps_per_revolution must be nonzero.

**Validation source:** tests/driver.c

**Test-derived call:**
```c
pc_stepper_microstep_phase_u32(i,n,&o)||o!=e);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

