#include "pc_cpx_to_polar_q.h"
#include "pc_cartesian_to_polar_q.h"
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_random(uint32_t *state)
{
    uint32_t x = *state;
    x ^= x << 13u; x ^= x >> 17u; x ^= x << 5u;
    *state = x;
    return x;
}

static int32_t next_i32(uint32_t *state)
{
    const uint32_t raw = next_random(state);
    const int64_t value = raw <= (uint32_t)INT32_MAX ? (int64_t)raw
        : (int64_t)raw - INT64_C(4294967296);
    return (int32_t)value;
}

static uint32_t phase_error(uint32_t a, uint32_t b)
{
    const uint32_t delta = a - b;
    return delta > UINT32_C(0x80000000) ? UINT32_C(0) - delta : delta;
}

static uint32_t reference_phase(int32_t real, int32_t imag)
{
    long double turns = atan2l((long double)imag, (long double)real) /
                        (2.0L * acosl(-1.0L));
    long double scaled;
    if (turns < 0.0L) turns += 1.0L;
    scaled = turns * 4294967296.0L;
    return (uint32_t)(uint64_t)llrintl(scaled);
}

static uint32_t reference_radius(int32_t real, int32_t imag)
{
    const long double norm = hypotl((long double)real, (long double)imag);
    return (uint32_t)llrintl(norm);
}

static int check(pc_cpx_q value, uint32_t *max_phase_error)
{
    uint32_t radius = 71u, phase = 93u;
    const uint32_t expected_radius = reference_radius(value.real, value.imag);
    const uint32_t expected_phase = value.real == 0 && value.imag == 0
        ? 0u : reference_phase(value.real, value.imag);
    uint32_t error;
    if (!pc_cpx_to_polar_q(value, &radius, &phase)) return 1;
    if (radius != expected_radius) return 2;
    error = phase_error(phase, expected_phase);
    if (error > *max_phase_error) *max_phase_error = error;
    return error > 32u ? 3 : 0;
}

int main(void)
{
    static const int32_t edge[] = {INT32_MIN, INT32_MIN + 1, -1, 0, 1,
                                   INT32_MAX - 1, INT32_MAX};
    uint32_t max_phase_error = 0u, state = UINT32_C(0x3296c0de), n;
    unsigned a, b;
    for (a = 0u; a < 7u; ++a)
        for (b = 0u; b < 7u; ++b) {
            const int status = check((pc_cpx_q){edge[a], edge[b]}, &max_phase_error);
            if (status) return 10 + status;
        }
    for (n = 0u; n < UINT32_C(1000000); ++n) {
        const pc_cpx_q value = {next_i32(&state), next_i32(&state)};
        const int status = check(value, &max_phase_error);
        if (status) return 20 + status;
    }
    for (n = 0u; n < UINT32_C(100000); ++n) {
        pc_cpx_q value = {next_i32(&state), next_i32(&state)};
        uint32_t radius, phase, conjugate_radius, conjugate_phase;
        if (value.imag == INT32_MIN) ++value.imag;
        if (!pc_cpx_to_polar_q(value, &radius, &phase) ||
            !pc_cpx_to_polar_q((pc_cpx_q){value.real, -value.imag},
                               &conjugate_radius, &conjugate_phase)) return 50;
        if (radius != conjugate_radius || phase_error(conjugate_phase, UINT32_C(0) - phase) > 32u)
            return 51;
    }
    {
        int x, y;
        for (x = -128; x < 128; ++x)
            for (y = -128; y < 128; ++y) {
                uint32_t radius;
                int32_t old_radius;
                uint32_t phase, old_phase;
                if (!pc_cpx_to_polar_q((pc_cpx_q){x, y}, &radius, &phase) ||
                    !pc_cartesian_to_polar_q((int16_t)x, (int16_t)y,
                                             &old_radius, &old_phase)) return 40;
                if (radius != (uint32_t)old_radius ||
                    phase_error(phase, old_phase) > 8u) return 41;
            }
    }
    {
        uint32_t radius = 71u, phase = 93u;
        const pc_cpx_q zero = {0, 0};
        if (!pc_cpx_to_polar_q(zero, &radius, &phase) || radius != 0u || phase != 0u) return 30;
        radius = 71u;
        if (pc_cpx_to_polar_q(zero, &radius, &radius) || radius != 71u) return 31;
        {
            const pc_cpx_q input = {INT32_MIN, INT32_MIN};
            if (!pc_cpx_to_polar_q(input, &radius, &phase) || radius != UINT32_C(3037000500)) return 32;
        }
    }
    printf("1,000,049 inputs; max circular phase error=%u Q0.32 codes\n", max_phase_error);
    return 0;
}