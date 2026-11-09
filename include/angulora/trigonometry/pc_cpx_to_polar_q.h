#ifndef PRIMITIVECORE_PC_CPX_TO_POLAR_Q_H
#define PRIMITIVECORE_PC_CPX_TO_POLAR_Q_H
#include <stdbool.h>
#include <stdint.h>
#include "pc_cpx_q.h"

/* Return nearest-integer uint32 magnitude and approximate Q0.32 CCW phase.
 * Output pointers must be distinct; failure preserves both outputs. */
bool pc_cpx_to_polar_q(pc_cpx_q value, uint32_t *radius,
                       uint32_t *phase_turns_q0_32);

#endif