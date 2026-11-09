#include "pc_cartesian_to_polar_q.h"
#include "pc_q_hypot2.h"
#include <stddef.h>

static const int64_t atan_turns_q32[31] = {
    INT64_C(536870912), INT64_C(316933406), INT64_C(167458907), INT64_C(85004756),
    INT64_C(42667331), INT64_C(21354465), INT64_C(10679838), INT64_C(5340245),
    INT64_C(2670163), INT64_C(1335087), INT64_C(667544), INT64_C(333772),
    INT64_C(166886), INT64_C(83443), INT64_C(41722), INT64_C(20861),
    INT64_C(10430), INT64_C(5215), INT64_C(2608), INT64_C(1304), INT64_C(652),
    INT64_C(326), INT64_C(163), INT64_C(81), INT64_C(41), INT64_C(20),
    INT64_C(10), INT64_C(5), INT64_C(3), INT64_C(1), INT64_C(1)
};

static int64_t shift_zero(int64_t v, unsigned n)
{
    const uint64_t magnitude = v < 0 ? (uint64_t)(-v) : (uint64_t)v;
    const int64_t quotient = (int64_t)(magnitude >> n);
    return v < 0 ? -quotient : quotient;
}

bool rm_cartesian_to_polar_q(int16_t x_q15, int16_t y_q15,
                             int32_t *radius_q15, uint32_t *phase_turns_q0_32)
{
    int32_t radius;
    int64_t x, y, z;
    unsigned i;
    if (radius_q15 == NULL || phase_turns_q0_32 == NULL ||
        (const void *)radius_q15 == (const void *)phase_turns_q0_32) return false;
    if (!rm_q_hypot2((int32_t)x_q15, (int32_t)y_q15, &radius)) return false;
    if (x_q15 == 0 && y_q15 == 0) {
        *radius_q15 = 0;
        *phase_turns_q0_32 = 0u;
        return true;
    }
    /* Cardinal axes have exact turn representations; do not route them
     * through the bounded CORDIC approximation. */
    if (x_q15 == 0) {
        *radius_q15 = radius;
        *phase_turns_q0_32 = y_q15 > 0 ? UINT32_C(0x40000000)
                                      : UINT32_C(0xc0000000);
        return true;
    }
    if (y_q15 == 0) {
        *radius_q15 = radius;
        *phase_turns_q0_32 = x_q15 > 0 ? 0u : UINT32_C(0x80000000);
        return true;
    }

    x = (int64_t)x_q15 * INT64_C(652032874);
    y = (int64_t)y_q15 * INT64_C(652032874);
    z = 0;
    if (x < 0) {
        z = y >= 0 ? INT64_C(2147483648) : -INT64_C(2147483648);
        x = -x;
        y = -y;
    }
    for (i = 0u; i < 31u; ++i) {
        const int64_t sx = shift_zero(x, i), sy = shift_zero(y, i);
        if (y > 0) { x += sy; y -= sx; z += atan_turns_q32[i]; }
        else if (y < 0) { x -= sy; y += sx; z -= atan_turns_q32[i]; }
    }
    *radius_q15 = radius;
    *phase_turns_q0_32 = (uint32_t)(uint64_t)z;
    return true;
}