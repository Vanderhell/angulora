#ifndef PRIMITIVECORE_RM_BEARING2_Q_H
#define PRIMITIVECORE_RM_BEARING2_Q_H
#include <stdbool.h>
#include <stdint.h>

/* CORDIC estimate of bearing from an int32 origin to target as unsigned Q0.32
 * turns CCW from +X. Equal points return zero. Coordinates need not share a
 * specific Q scale. Differential tests observed <= 16 phase-code error; this
 * empirical limit is not a formal global bound. */
bool rm_bearing2_q(int32_t origin_x, int32_t origin_y,
                   int32_t target_x, int32_t target_y,
                   uint32_t *bearing_turns_q0_32);
#endif

/* Canonical facade; the legacy implementation and contract are preserved. */
#ifndef pc_bearing2_q
#define pc_bearing2_q rm_bearing2_q
#endif
#ifndef pc_bearing2_q
#define pc_bearing2_q rm_bearing2_q
#endif
#ifndef PC_PRIMITIVECORE_RM_BEARING2_Q_H
#define PC_PRIMITIVECORE_RM_BEARING2_Q_H PRIMITIVECORE_RM_BEARING2_Q_H
#endif

