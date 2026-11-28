#ifndef ANGULORA_TRIGONOMETRY_BEARING2_Q_H
#define ANGULORA_TRIGONOMETRY_BEARING2_Q_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/* CORDIC estimate of bearing from an int32 origin to target as unsigned Q0.32
 * turns CCW from +X. Equal points return zero. Coordinates need not share a
 * specific Q scale. Differential tests observed <= 16 phase-code error; this
 * empirical limit is not a formal global bound. */
bool angulora_bearing2_q(int32_t origin_x, int32_t origin_y,
                   int32_t target_x, int32_t target_y,
                   uint32_t *bearing_turns_q0_32);


#ifdef __cplusplus
}
#endif

#endif /* ANGULORA_TRIGONOMETRY_BEARING2_Q_H */
