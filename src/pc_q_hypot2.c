#include <primitivecore/fixed_point/arithmetic/q_arithmetic/pc_q_hypot2.h>
#include <stddef.h>

bool rm_q_hypot2(int32_t x, int32_t y, int32_t *out)
{
    uint64_t ax = x < 0 ? (uint64_t)(-(int64_t)x) : (uint64_t)x;
    uint64_t ay = y < 0 ? (uint64_t)(-(int64_t)y) : (uint64_t)y;
    uint64_t target = ax * ax + ay * ay;
    uint64_t lo = 0u, hi = UINT64_C(3037000501), q, delta;

    if (out == NULL) return false;
    while (lo + 1u < hi) {
        uint64_t mid = lo + (hi - lo) / 2u;
        if (mid * mid <= target) lo = mid;
        else hi = mid;
    }
    q = lo;
    delta = target - q * q;
    if (4u * delta > 4u * q + 1u ||
        (4u * delta == 4u * q + 1u && (q & 1u) != 0u)) {
        ++q;
    }
    if (q > INT32_MAX) return false;
    *out = (int32_t)q;
    return true;
}
