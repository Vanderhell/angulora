#ifndef PC_PC_CARTESIAN_TO_POLAR_Q_H
#define PC_PC_CARTESIAN_TO_POLAR_Q_H
#include <stdbool.h>
#include <stdint.h>

/* Convert signed-Q15 Cartesian coordinates to nearest-even Q15 radius and
 * counter-clockwise Q0.32 turns. The zero vector has phase zero. The phase
 * uses bounded integer CORDIC approximation; observed phase error is <= 8 codes
 * in the recorded differential campaign (not an exhaustive global bound). */
bool pc_cartesian_to_polar_q(int16_t x_q15, int16_t y_q15,
                             int32_t *radius_q15, uint32_t *phase_turns_q0_32);
#endif

/* Canonical facade; the legacy implementation and contract are preserved. */
#ifndef pc_cartesian_to_polar_q
#define pc_cartesian_to_polar_q pc_cartesian_to_polar_q
#endif
#ifndef pc_cartesian_to_polar_q
#define pc_cartesian_to_polar_q pc_cartesian_to_polar_q
#endif
#ifndef PC_PC_PC_CARTESIAN_TO_POLAR_Q_H
#define PC_PC_PC_CARTESIAN_TO_POLAR_Q_H PC_PC_CARTESIAN_TO_POLAR_Q_H
#endif
