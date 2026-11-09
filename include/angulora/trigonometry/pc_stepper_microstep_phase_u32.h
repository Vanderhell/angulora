#ifndef PC_STEPPER_MICROSTEP_PHASE_U32_H
#define PC_STEPPER_MICROSTEP_PHASE_U32_H
#include <stdbool.h>
#include <stdint.h>
/* Map an unsigned microstep index to periodic unsigned Q0.32 revolution
 * phase, nearest-even. steps_per_revolution must be nonzero. */
bool pc_stepper_microstep_phase_u32(uint32_t microstep_index,
    uint32_t steps_per_revolution, uint32_t *phase_q0_32);
#endif
