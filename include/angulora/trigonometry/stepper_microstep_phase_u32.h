#ifndef ANGULORA_TRIGONOMETRY_STEPPER_MICROSTEP_PHASE_U32_H
#define ANGULORA_TRIGONOMETRY_STEPPER_MICROSTEP_PHASE_U32_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Map an unsigned microstep index to periodic unsigned Q0.32 revolution
 * phase, nearest-even. steps_per_revolution must be nonzero. */
bool angulora_stepper_microstep_phase_u32(uint32_t microstep_index,
    uint32_t steps_per_revolution, uint32_t *phase_q0_32);

#ifdef __cplusplus
}
#endif

#endif /* ANGULORA_TRIGONOMETRY_STEPPER_MICROSTEP_PHASE_U32_H */
