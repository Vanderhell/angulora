#include "pc_phase_to_tick_u32.h"
#include <stddef.h>

bool pc_phase_to_tick_u32(uint32_t phase_q0_32, uint32_t ticks_per_cycle,
                          uint32_t *tick)
{
    uint64_t product;
    if (tick == NULL || ticks_per_cycle == 0u) return false;
    product = (uint64_t)phase_q0_32 * (uint64_t)ticks_per_cycle;
    *tick = (uint32_t)(product >> 32);
    return true;
}