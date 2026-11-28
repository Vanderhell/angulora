#ifndef ANGULORA_TRIGONOMETRY_STEPPER_ANGLE_FROM_STEPS_Q_H
#define ANGULORA_TRIGONOMETRY_STEPPER_ANGLE_FROM_STEPS_Q_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Convert signed integer steps to signed Q16.16 degrees using caller
 * calibration. Zero steps/revolution or unrepresentable angle fails. */
bool angulora_stepper_angle_from_steps_q(int32_t steps, uint32_t steps_per_revolution,
                                   int32_t *angle_q16_16_degrees);

#ifdef __cplusplus
}
#endif

#endif /* ANGULORA_TRIGONOMETRY_STEPPER_ANGLE_FROM_STEPS_Q_H */
