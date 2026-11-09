#ifndef PC_PHASE_TO_TICK_U32_H
#define PC_PHASE_TO_TICK_U32_H
#include <stdbool.h>
#include <stdint.h>
/* Map unsigned Q0.32 phase [0,1) to a tick in [0,ticks_per_cycle). */
bool pc_phase_to_tick_u32(uint32_t phase_q0_32, uint32_t ticks_per_cycle,
                          uint32_t *tick);
#endif

/* Canonical facade; the legacy implementation and contract are preserved. */
#ifndef pc_phase_to_tick_u32
#define pc_phase_to_tick_u32 pc_phase_to_tick_u32
#endif
#ifndef pc_phase_to_tick_u32
#define pc_phase_to_tick_u32 pc_phase_to_tick_u32
#endif
#ifndef PC_PC_PHASE_TO_TICK_U32_H
#define PC_PC_PHASE_TO_TICK_U32_H PC_PHASE_TO_TICK_U32_H
#endif
