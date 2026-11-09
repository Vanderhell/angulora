#include "../include/pc_angle_sector_center_q.h"
#include "roundmath/phase_to_tick_u32.h"

#include <stdint.h>

#define CHECK(c) do { if (!(c)) return __LINE__; } while (0)

static uint32_t nearest_even(uint64_t numerator, uint32_t denominator)
{
    uint64_t q = numerator / denominator;
    uint64_t r = numerator % denominator;
    if (2u * r > denominator || (2u * r == denominator && (q & 1u) != 0u)) ++q;
    return (uint32_t)q;
}

static uint32_t reference(uint32_t index, uint32_t count)
{
    return nearest_even((((uint64_t)index * 2u) + 1u) << 31, count);
}

static int check(uint32_t index, uint32_t count)
{
    uint32_t center = UINT32_C(0xdeadbeef);
    uint32_t sector;
    if (!pc_angle_sector_center_q(index, count, &center)) return __LINE__;
    if (center != reference(index, count)) return __LINE__;
    if (!pc_phase_to_tick_u32(center, count, &sector) || sector != index) return __LINE__;
    return 0;
}

int main(void)
{
    uint32_t out = UINT32_C(0xdeadbeef);
    CHECK(!pc_angle_sector_center_q(0u, 1u, 0));
    CHECK(!pc_angle_sector_center_q(0u, 0u, &out) && out == UINT32_C(0xdeadbeef));
    CHECK(!pc_angle_sector_center_q(1u, 1u, &out) && out == UINT32_C(0xdeadbeef));

    for (uint32_t count = 1; count <= 256u; ++count) {
        for (uint32_t index = 0; index < count; ++index) {
            CHECK(check(index, count) == 0);
        }
    }

    {
        static const uint32_t count[] = {257u, 65535u, 65536u,
            UINT32_C(2147483647), UINT32_C(2147483648), UINT32_MAX};
        for (unsigned i = 0; i < sizeof count / sizeof count[0]; ++i) {
            uint32_t k[] = {0u, 1u, count[i] / 2u,
                count[i] - 2u, count[i] - 1u};
            for (unsigned j = 0; j < sizeof k / sizeof k[0]; ++j) {
                CHECK(check(k[j], count[i]) == 0);
            }
        }
    }

    {
        uint32_t state = UINT32_C(0x1332a11);
        for (unsigned i = 0; i < 100000u; ++i) {
            uint32_t count, index;
            state = state * UINT32_C(1664525) + UINT32_C(1013904223);
            count = state == 0u ? 1u : state;
            state = state * UINT32_C(1664525) + UINT32_C(1013904223);
            index = state % count;
            CHECK(check(index, count) == 0);
        }
    }
    return 0;
}
