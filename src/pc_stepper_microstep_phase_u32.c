#include "pc_stepper_microstep_phase_u32.h"
#include <stddef.h>

bool pc_stepper_microstep_phase_u32(uint32_t index, uint32_t steps_per_rev,
                                    uint32_t *phase_q0_32)
{
    uint64_t product, quotient, remainder;
    if (phase_q0_32 == NULL || steps_per_rev == 0u) return false;
    product = (uint64_t)(index % steps_per_rev) * (UINT64_C(1) << 32);
    quotient = product / steps_per_rev;
    remainder = product % steps_per_rev;
    if (remainder > steps_per_rev / 2u ||
        ((steps_per_rev & 1u) == 0u && remainder == steps_per_rev / 2u &&
         (quotient & 1u) != 0u)) {
        ++quotient;
    }
    *phase_q0_32 = (uint32_t)quotient;
    return true;
}