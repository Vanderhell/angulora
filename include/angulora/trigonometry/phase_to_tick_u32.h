#ifndef ANGULORA_TRIGONOMETRY_PHASE_TO_TICK_U32_H
#define ANGULORA_TRIGONOMETRY_PHASE_TO_TICK_U32_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Map unsigned Q0.32 phase [0,1) to a tick in [0,ticks_per_cycle). */
bool angulora_phase_to_tick_u32(uint32_t phase_q0_32, uint32_t ticks_per_cycle,
                          uint32_t *tick);


#ifdef __cplusplus
}
#endif

#endif /* ANGULORA_TRIGONOMETRY_PHASE_TO_TICK_U32_H */
