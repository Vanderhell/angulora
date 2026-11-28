#ifndef ANGULORA_TRIGONOMETRY_CARTESIAN_TO_POLAR_Q_H
#define ANGULORA_TRIGONOMETRY_CARTESIAN_TO_POLAR_Q_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Convert signed-Q15 Cartesian coordinates to nearest-even Q15 radius and
 * counter-clockwise Q0.32 turns. The zero vector has phase zero. The phase
 * uses bounded integer CORDIC approximation; observed phase error is <= 8 codes
 * in the recorded differential campaign (not an exhaustive global bound). */
bool angulora_cartesian_to_polar_q(int16_t x_q15, int16_t y_q15,
                             int32_t *radius_q15, uint32_t *phase_turns_q0_32);


#ifdef __cplusplus
}
#endif

#endif /* ANGULORA_TRIGONOMETRY_CARTESIAN_TO_POLAR_Q_H */
