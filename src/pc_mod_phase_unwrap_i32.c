#include <primitivecore/trigonometry/angle/inverse/pc_mod_phase_unwrap_i32.h>
#include <stddef.h>

bool rm_mod_phase_unwrap_i32(uint32_t previous, uint32_t current,
                             int32_t *delta)
{
    uint32_t forward;
    if (delta == NULL) return false;
    forward = current - previous;
    if (forward == UINT32_C(0x80000000)) return false;
    if (forward < UINT32_C(0x80000000)) {
        *delta = (int32_t)forward;
    } else {
        uint32_t magnitude = 0u - forward;
        *delta = -(int32_t)magnitude;
    }
    return true;
}
