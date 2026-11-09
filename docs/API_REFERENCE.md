# Angulora API reference

Complete public-header inventory generated from the current source tree. Headers remain authoritative.

## include/angulora/trigonometry/pc_bearing2_q.h

Public API: pc_bearing2_q

Contract: CORDIC estimate of bearing from an int32 origin to target as unsigned Q0.32 turns CCW from +X. Equal points return zero. Coordinates need not share a specific Q scale. Differential tests observed <= 16 phase-code error; this empirical limit is not a formal global bound.

Header path: include/angulora/trigonometry/pc_bearing2_q.h

## include/angulora/trigonometry/pc_cartesian_to_polar_q.h

Public API: pc_cartesian_to_polar_q

Contract: Convert signed-Q15 Cartesian coordinates to nearest-even Q15 radius and counter-clockwise Q0.32 turns. The zero vector has phase zero. The phase uses bounded integer CORDIC approximation; observed phase error is <= 8 codes in the recorded differential campaign (not an exhaustive global bound).

Header path: include/angulora/trigonometry/pc_cartesian_to_polar_q.h

## include/angulora/trigonometry/pc_cpx_q.h

Public API: pc_cpx_q

Contract: The public header contains the authoritative signature and input/output contract.

Header path: include/angulora/trigonometry/pc_cpx_q.h

## include/angulora/trigonometry/pc_cpx_to_polar_q.h

Public API: pc_cpx_to_polar_q

Contract: Return nearest-integer uint32 magnitude and approximate Q0.32 CCW phase. Output pointers must be distinct; failure preserves both outputs.

Header path: include/angulora/trigonometry/pc_cpx_to_polar_q.h

## include/angulora/trigonometry/pc_mod_phase_unwrap_i32.h

Public API: pc_mod_phase_unwrap_i32

Contract: Shortest signed delta on the uint32_t phase ring; antipodes are ambiguous.

Header path: include/angulora/trigonometry/pc_mod_phase_unwrap_i32.h

## include/angulora/trigonometry/pc_phase_to_tick_u32.h

Public API: pc_phase_to_tick_u32

Contract: Map unsigned Q0.32 phase [0,1) to a tick in [0,ticks_per_cycle).

Header path: include/angulora/trigonometry/pc_phase_to_tick_u32.h

## include/angulora/trigonometry/pc_q_hypot2.h

Public API: pc_q_hypot2

Contract: Nearest-even Euclidean norm of same-scale signed Q raw values. False on null output or int32 result overflow; failure preserves *out.

Header path: include/angulora/trigonometry/pc_q_hypot2.h

## include/angulora/trigonometry/pc_stepper_angle_from_steps_q.h

Public API: pc_stepper_angle_from_steps_q

Contract: Convert signed integer steps to signed Q16.16 degrees using caller calibration. Zero steps/revolution or unrepresentable angle fails.

Header path: include/angulora/trigonometry/pc_stepper_angle_from_steps_q.h

## include/angulora/trigonometry/pc_stepper_microstep_phase_u32.h

Public API: pc_stepper_microstep_phase_u32

Contract: Map an unsigned microstep index to periodic unsigned Q0.32 revolution phase, nearest-even. steps_per_revolution must be nonzero.

Header path: include/angulora/trigonometry/pc_stepper_microstep_phase_u32.h
