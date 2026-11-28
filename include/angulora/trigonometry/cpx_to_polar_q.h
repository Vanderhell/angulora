#ifndef ANGULORA_TRIGONOMETRY_CPX_TO_POLAR_Q_H
#define ANGULORA_TRIGONOMETRY_CPX_TO_POLAR_Q_H
#include <stdbool.h>
#include <stdint.h>
#include <angulora/trigonometry/cpx_q.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Return nearest-integer uint32 magnitude and approximate Q0.32 CCW phase.
 * Output pointers must be distinct; failure preserves both outputs. */
bool angulora_cpx_to_polar_q(angulora_cpx_q value, uint32_t *radius,
                       uint32_t *phase_turns_q0_32);

#ifdef __cplusplus
}
#endif

#endif /* ANGULORA_TRIGONOMETRY_CPX_TO_POLAR_Q_H */
