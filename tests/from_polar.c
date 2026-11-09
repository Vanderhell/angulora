#include "../include/pc_cpx_from_polar_q.h"
#include "roundmath/pc_cartesian_to_polar_q.h"
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t next_random(uint32_t *state)
{
    uint32_t x = *state;
    x ^= x << 13u; x ^= x >> 17u; x ^= x << 5u;
    *state = x;
    return x;
}

static int16_t next_i16(uint32_t *state)
{
    const uint32_t raw = next_random(state) & UINT32_C(0xffff);
    const int32_t signed_value = raw <= (uint32_t)INT16_MAX
        ? (int32_t)raw : (int32_t)raw - INT32_C(65536);
    return (int16_t)signed_value;
}

static uint64_t difference(int32_t actual, long double ideal)
{
    const long long rounded = llrintl(ideal);
    const long long delta = (long long)actual - rounded;
    return (uint64_t)(delta < 0 ? -delta : delta);
}

static int check(int32_t radius, uint32_t phase, uint64_t *maximum)
{
    pc_cpx_q out = {123, 456}, repeat;
    const long double angle = (long double)phase * (2.0L * acosl(-1.0L) / 4294967296.0L);
    uint64_t er, ei;
    if (!pc_cpx_from_polar_q(radius, phase, &out) ||
        !pc_cpx_from_polar_q(radius, phase, &repeat)) return 1;
    if (out.real != repeat.real || out.imag != repeat.imag) return 2;
    er = difference(out.real, (long double)radius * cosl(angle));
    ei = difference(out.imag, (long double)radius * sinl(angle));
    if (er > *maximum) *maximum = er;
    if (ei > *maximum) *maximum = ei;
    return er > 200u || ei > 200u ? 3 : 0;
}

int main(void)
{
    static const int32_t radii[] = {0, 1, 32767, 32768, 46341,
                                    INT32_MAX - 1, INT32_MAX};
    static const uint32_t phases[] = {0u, 1u, UINT32_C(0x1fffffff),
        UINT32_C(0x20000000), UINT32_C(0x3fffffff), UINT32_C(0x40000000),
        UINT32_C(0x7fffffff), UINT32_C(0x80000000), UINT32_C(0xc0000000), UINT32_MAX};
    uint32_t state = UINT32_C(0x3295c0de), n;
    uint64_t maximum = 0u;
    unsigned r, p;
    for (r = 0u; r < sizeof radii / sizeof radii[0]; ++r)
        for (p = 0u; p < sizeof phases / sizeof phases[0]; ++p) {
            const int status = check(radii[r], phases[p], &maximum);
            if (status) return 10 + status;
        }
    for (n = 0u; n < UINT32_C(1000000); ++n) {
        const int32_t radius = (int32_t)(next_random(&state) & UINT32_C(0x7fffffff));
        const int status = check(radius, next_random(&state), &maximum);
        if (status) return 20 + status;
    }
    {
        int x, y;
        for (x = -128; x < 128; ++x)
            for (y = -128; y < 128; ++y) {
                int32_t radius;
                uint32_t phase;
                pc_cpx_q reconstructed;
                if (!pc_cartesian_to_polar_q((int16_t)x, (int16_t)y, &radius, &phase) ||
                    !pc_cpx_from_polar_q(radius, phase, &reconstructed)) return 32;
                if (difference(reconstructed.real, (long double)x) > 2u ||
                    difference(reconstructed.imag, (long double)y) > 2u) return 33;
            }
        {
            static const int16_t edge[] = {INT16_MIN, INT16_MIN + 1, -1, 0, 1,
                                           INT16_MAX - 1, INT16_MAX};
            unsigned a, b;
            for (a = 0u; a < 7u; ++a)
                for (b = 0u; b < 7u; ++b) {
                    int32_t radius;
                    uint32_t phase;
                    pc_cpx_q reconstructed;
                    if (!pc_cartesian_to_polar_q(edge[a], edge[b], &radius, &phase) ||
                        !pc_cpx_from_polar_q(radius, phase, &reconstructed)) return 34;
                    if (difference(reconstructed.real, (long double)edge[a]) > 2u ||
                        difference(reconstructed.imag, (long double)edge[b]) > 2u) return 35;
                }
        }
        {
            uint32_t n, wide_radii = 0u;
            for (n = 0u; n < UINT32_C(100000); ++n) {
                const int16_t x16 = next_i16(&state), y16 = next_i16(&state);
                int32_t radius;
                uint32_t phase;
                pc_cpx_q reconstructed;
                if (!pc_cartesian_to_polar_q(x16, y16, &radius, &phase) ||
                    !pc_cpx_from_polar_q(radius, phase, &reconstructed)) return 36;
                if (radius > INT16_MAX) ++wide_radii;
                if (difference(reconstructed.real, (long double)x16) > 2u ||
                    difference(reconstructed.imag, (long double)y16) > 2u) return 37;
            }
            if (wide_radii == 0u) return 38;
        }
    }
    {
        pc_cpx_q out = {123, 456};
        if (pc_cpx_from_polar_q(-1, 0u, &out) || out.real != 123 || out.imag != 456) return 30;
        if (pc_cpx_from_polar_q(1, 0u, NULL)) return 31;
    }
    printf("1,000,070 radius/phase pairs; max rounded component error=%llu raw codes\n",
           (unsigned long long)maximum);
    return 0;
}
