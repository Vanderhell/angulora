#include "pc_stepper_angle_from_steps_q.h"
#include <limits.h>
#include <stddef.h>

bool pc_stepper_angle_from_steps_q(int32_t steps, uint32_t steps_per_rev,
                                   int32_t *angle_q16_16_degrees)
{
    const int64_t numerator = (int64_t)steps * INT64_C(360) * INT64_C(65536);
    const bool negative = numerator < 0;
    const uint64_t magnitude = (uint64_t)(negative ? -numerator : numerator);
    uint64_t quotient, remainder;
    int64_t result;

    if (angle_q16_16_degrees == NULL || steps_per_rev == 0u) return false;
    quotient = magnitude / steps_per_rev;
    remainder = magnitude % steps_per_rev;
    if (remainder > steps_per_rev / 2u ||
        ((steps_per_rev & 1u) == 0u && remainder == steps_per_rev / 2u &&
         (quotient & 1u) != 0u)) {
        ++quotient;
    }
    result = negative ? -(int64_t)quotient : (int64_t)quotient;
    if (result < INT32_MIN || result > INT32_MAX) return false;
    *angle_q16_16_degrees = (int32_t)result;
    return true;
}