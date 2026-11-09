#include "pc_cpx_to_polar_q.h"
#include "pc_bearing2_q.h"
#include <stddef.h>

static uint64_t integer_sqrt_floor(uint64_t value)
{
    uint64_t low = 0u, high = UINT64_C(3037000500);
    while (low + 1u < high) {
        const uint64_t middle = low + (high - low) / 2u;
        if (middle * middle <= value) low = middle;
        else high = middle;
    }
    return low;
}

static uint32_t magnitude_nearest_even(int32_t real, int32_t imag)
{
    const int64_t r = real, i = imag;
    const uint64_t squared = (uint64_t)(r * r) + (uint64_t)(i * i);
    uint64_t root = integer_sqrt_floor(squared);
    const uint64_t delta = squared - root * root;
    if (4u * delta > 4u * root + 1u ||
        (4u * delta == 4u * root + 1u && (root & 1u) != 0u)) ++root;
    return (uint32_t)root;
}

bool pc_cpx_to_polar_q(pc_cpx_q value, uint32_t *radius,
                       uint32_t *phase_turns_q0_32)
{
    uint32_t temporary_radius, temporary_phase;
    if (radius == NULL || phase_turns_q0_32 == NULL || radius == phase_turns_q0_32) return false;
    temporary_radius = magnitude_nearest_even(value.real, value.imag);
    if (!pc_bearing2_q(0, 0, value.real, value.imag, &temporary_phase)) return false;
    *radius = temporary_radius;
    *phase_turns_q0_32 = temporary_phase;
    return true;
}