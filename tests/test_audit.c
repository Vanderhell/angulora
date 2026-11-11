#include "pc_bearing2_q.h"
#include "pc_cartesian_to_polar_q.h"
#include "pc_cpx_q.h"
#include "pc_cpx_to_polar_q.h"
#include "pc_mod_phase_unwrap_i32.h"
#include "pc_phase_to_tick_u32.h"
#include "pc_q_hypot2.h"
#include "pc_stepper_angle_from_steps_q.h"
#include "pc_stepper_microstep_phase_u32.h"

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define CHECK(c) do { if (!(c)) { \
    fprintf(stderr, "audit check failed at line %d: %s\n", __LINE__, #c); \
    return 1; \
} } while (0)

#define PI_L 3.141592653589793238462643383279502884L
#define TURN_SCALE 4294967296.0L

unsigned angulora_audit_current_api = 0u;

static uint64_t rng_state = UINT64_C(0x6a09e667f3bcc909);

static uint32_t random_u32(void)
{
    uint64_t z;
    rng_state += UINT64_C(0x9e3779b97f4a7c15);
    z = rng_state;
    z = (z ^ (z >> 30u)) * UINT64_C(0xbf58476d1ce4e5b9);
    z = (z ^ (z >> 27u)) * UINT64_C(0x94d049bb133111eb);
    z ^= z >> 31u;
    return (uint32_t)(z >> 16u);
}

static int32_t random_i32(void)
{
    const uint32_t u = random_u32();
    const int64_t value = u <= (uint32_t)INT32_MAX
        ? (int64_t)u : (int64_t)u - INT64_C(4294967296);
    return (int32_t)value;
}

static int16_t random_i16(void)
{
    const uint32_t u = random_u32() & UINT32_C(0xffff);
    const int32_t value = u <= (uint32_t)INT16_MAX
        ? (int32_t)u : (int32_t)u - INT32_C(65536);
    return (int16_t)value;
}

static int64_t round_even(long double x)
{
    const int negative = x < 0.0L;
    const long double a = negative ? -x : x;
    const long double base = floorl(a);
    const long double fraction = a - base;
    int64_t q = (int64_t)base;
    if (fraction > 0.5L || (fraction == 0.5L && (q & 1) != 0)) ++q;
    return negative ? -q : q;
}

static uint32_t phase_reference(long double x, long double y)
{
    long double angle;
    int64_t code;
    if (x == 0.0L && y == 0.0L) return 0u;
    angle = (long double)atan2((double)y, (double)x);
    if (angle < 0.0L) angle += 2.0L * PI_L;
    code = round_even(angle * (TURN_SCALE / (2.0L * PI_L)));
    return (uint32_t)(uint64_t)code;
}

static uint32_t circular_error(uint32_t actual, uint32_t expected)
{
    const uint32_t d = actual - expected;
    return d > UINT32_C(0x80000000) ? 0u - d : d;
}

typedef struct {
    uint32_t maximum;
    long double sum;
    long double sum_squares;
    long double wx;
    long double wy;
    uint32_t count;
} phase_stats;

static void record_phase(phase_stats *s, uint32_t actual, uint32_t expected,
                         long double x, long double y)
{
    const uint32_t error = circular_error(actual, expected);
    const long double e = error;
    if (error > s->maximum) {
        s->maximum = error;
        s->wx = x;
        s->wy = y;
    }
    s->sum += e;
    s->sum_squares += e * e;
    ++s->count;
}

static void print_phase_stats(const char *name, const phase_stats *s)
{
    const long double mean = s->sum / s->count;
    const long double rms = sqrtl(s->sum_squares / s->count);
    const long double degrees = (long double)s->maximum * 360.0L / TURN_SCALE;
    printf("%s: n=%lu max=%lu codes (%.12e deg), mean=%.9e codes, rms=%.9e codes, worst=(%.0f,%.0f)\n",
           name, (unsigned long)s->count, (unsigned long)s->maximum,
           (double)degrees, (double)mean, (double)rms, (double)s->wx, (double)s->wy);
}

static int check_bearing(phase_stats *stats, int32_t ox, int32_t oy,
                         int32_t tx, int32_t ty)
{
    const int64_t dx = (int64_t)tx - ox;
    const int64_t dy = (int64_t)ty - oy;
    uint32_t out = UINT32_C(0xdeadbeef);
    uint32_t expected;
    if (!pc_bearing2_q(ox, oy, tx, ty, &out)) return 1;
    expected = phase_reference((long double)dx, (long double)dy);
    if (dx == 0 && dy == 0) return out == 0u ? 0 : 2;
    record_phase(stats, out, expected, (long double)dx, (long double)dy);
    return circular_error(out, expected) <= 16u ? 0 : 3;
}

static int check_cartesian(phase_stats *stats, int16_t x, int16_t y)
{
    int32_t radius = -77;
    uint32_t phase = UINT32_C(0xdeadbeef);
    const int64_t expected_radius = round_even(sqrtl((long double)x * x + (long double)y * y));
    const uint32_t expected_phase = phase_reference((long double)x, (long double)y);
    if (!pc_cartesian_to_polar_q(x, y, &radius, &phase)) return 1;
    if (radius != expected_radius) return 2;
    if (x == 0 && y == 0) return phase == 0u ? 0 : 3;
    record_phase(stats, phase, expected_phase, x, y);
    return circular_error(phase, expected_phase) <= 8u ? 0 : 4;
}

static int check_complex(phase_stats *stats, int32_t x, int32_t y)
{
    const pc_cpx_q value = {x, y};
    uint32_t radius = UINT32_C(0x12345678), phase = UINT32_C(0x87654321);
    const int64_t expected_radius = round_even(sqrtl((long double)x * x + (long double)y * y));
    const uint32_t expected_phase = phase_reference(x, y);
    if (!pc_cpx_to_polar_q(value, &radius, &phase)) return 1;
    if (radius != (uint32_t)expected_radius) return 2;
    if (x == 0 && y == 0) return phase == 0u ? 0 : 3;
    record_phase(stats, phase, expected_phase, x, y);
    return circular_error(phase, expected_phase) <= 16u ? 0 : 4;
}

static uint64_t multiply_reference(uint32_t a, uint32_t b)
{
    uint64_t product = 0u;
    uint64_t addend = a;
    while (b != 0u) {
        if ((b & 1u) != 0u) product += addend;
        addend <<= 1u;
        b >>= 1u;
    }
    return product;
}

static void divide_reference(uint64_t numerator, uint32_t denominator,
                             uint64_t *quotient, uint64_t *remainder)
{
    uint64_t q = 0u, r = 0u;
    unsigned bit = 64u;
    while (bit != 0u) {
        --bit;
        r = (r << 1u) | ((numerator >> bit) & UINT64_C(1));
        q <<= 1u;
        if (r >= denominator) {
            r -= denominator;
            q |= UINT64_C(1);
        }
    }
    *quotient = q;
    *remainder = r;
}

static uint32_t phase_tick_reference(uint32_t phase, uint32_t ticks)
{
    return (uint32_t)(multiply_reference(phase, ticks) >> 32u);
}

static uint32_t microstep_reference(uint32_t index, uint32_t count)
{
    uint64_t numerator = index % count;
    uint64_t q, r;
    numerator <<= 32u;
    divide_reference(numerator, count, &q, &r);
    if (r > count / 2u || ((count & 1u) == 0u && r == count / 2u && (q & 1u))) ++q;
    return (uint32_t)q;
}

static int test_bearing(void)
{
    static const int32_t edge[] = {INT32_MIN, INT32_MIN + 1, -1, 0, 1, INT32_MAX - 1, INT32_MAX};
    phase_stats stats = {0u, 0.0L, 0.0L, 0.0L, 0.0L, 0u};
    unsigned i, j;
    uint32_t n;
    for (i = 0u; i < 7u; ++i) for (j = 0u; j < 7u; ++j)
        CHECK(check_bearing(&stats, 0, 0, edge[i], edge[j]) == 0);
    CHECK(check_bearing(&stats, INT32_MIN, INT32_MIN, INT32_MAX, INT32_MAX) == 0);
    CHECK(check_bearing(&stats, INT32_MAX, INT32_MIN, INT32_MIN, INT32_MAX) == 0);
    for (n = 0u; n < 100000u; ++n) {
        const int32_t ox = random_i32();
        const int32_t oy = random_i32();
        int32_t tx = random_i32();
        int32_t ty = random_i32();
        if (tx == ox && ty == oy) tx = tx == INT32_MAX ? tx - 1 : tx + 1;
        CHECK(check_bearing(&stats, ox, oy, tx, ty) == 0);
    }
    CHECK(!pc_bearing2_q(0, 0, 1, 1, NULL));
    print_phase_stats("pc_bearing2_q", &stats);
    return 0;
}

static int test_cartesian(void)
{
    static const int16_t edge[] = {INT16_MIN, INT16_MIN + 1, -1, 0, 1, INT16_MAX - 1, INT16_MAX};
    phase_stats stats = {0u, 0.0L, 0.0L, 0.0L, 0.0L, 0u};
    unsigned i, j;
    uint32_t n;
    for (i = 0u; i < 7u; ++i) for (j = 0u; j < 7u; ++j)
        CHECK(check_cartesian(&stats, edge[i], edge[j]) == 0);
    for (n = 0u; n < 100000u; ++n) {
        const int16_t x = random_i16();
        const int16_t y = random_i16();
        CHECK(check_cartesian(&stats, x, y) == 0);
    }
    {
        int32_t radius = 77;
        uint32_t phase = 88u;
        CHECK(!pc_cartesian_to_polar_q(1, 1, NULL, &phase) && phase == 88u);
        CHECK(!pc_cartesian_to_polar_q(1, 1, &radius, NULL) && radius == 77);
        CHECK(!pc_cartesian_to_polar_q(1, 1, &radius, (uint32_t *)&radius) && radius == 77);
    }
    print_phase_stats("pc_cartesian_to_polar_q", &stats);
    return 0;
}

static int test_complex_type(void)
{
    const pc_cpx_q typed = {INT32_MIN, INT32_MAX};
    CHECK(typed.real == INT32_MIN && typed.imag == INT32_MAX);
    return 0;
}

static int test_complex(void)
{
    static const int32_t edge[] = {INT32_MIN, INT32_MIN + 1, -1, 0, 1, INT32_MAX - 1, INT32_MAX};
    phase_stats stats = {0u, 0.0L, 0.0L, 0.0L, 0.0L, 0u};
    unsigned i, j;
    uint32_t n;
    const pc_cpx_q typed = {INT32_MIN, INT32_MAX};
    for (i = 0u; i < 7u; ++i) for (j = 0u; j < 7u; ++j)
        CHECK(check_complex(&stats, edge[i], edge[j]) == 0);
    for (n = 0u; n < 100000u; ++n) {
        const int32_t x = random_i32();
        const int32_t y = random_i32();
        CHECK(check_complex(&stats, x, y) == 0);
    }
    {
        uint32_t radius = 123u, phase = 456u;
        CHECK(!pc_cpx_to_polar_q(typed, NULL, &phase) && phase == 456u);
        CHECK(!pc_cpx_to_polar_q(typed, &radius, NULL) && radius == 123u);
        CHECK(!pc_cpx_to_polar_q(typed, &radius, &radius) && radius == 123u);
    }
    print_phase_stats("pc_cpx_to_polar_q", &stats);
    return 0;
}

static int test_unwrap(void)
{
    uint32_t n;
    int32_t out = 123;
    CHECK(pc_mod_phase_unwrap_i32(0u, 0u, &out) && out == 0);
    CHECK(pc_mod_phase_unwrap_i32(UINT32_MAX, 0u, &out) && out == 1);
    CHECK(pc_mod_phase_unwrap_i32(0u, UINT32_MAX, &out) && out == -1);
    CHECK(pc_mod_phase_unwrap_i32(0u, UINT32_C(0x7fffffff), &out) && out == INT32_MAX);
    CHECK(pc_mod_phase_unwrap_i32(0u, UINT32_C(0x80000001), &out) && out == -INT32_MAX);
    out = 123;
    CHECK(!pc_mod_phase_unwrap_i32(0u, UINT32_C(0x80000000), &out) && out == 123);
    CHECK(!pc_mod_phase_unwrap_i32(0u, 1u, NULL));
    for (n = 0u; n < 100000u; ++n) {
        const uint32_t a = random_u32();
        const uint32_t b = random_u32();
        const uint32_t forward = b - a;
        if (forward == UINT32_C(0x80000000)) {
            out = 91;
            CHECK(!pc_mod_phase_unwrap_i32(a, b, &out) && out == 91);
        } else {
            const int32_t expected = forward < UINT32_C(0x80000000)
                ? (int32_t)forward : -(int32_t)(0u - forward);
            CHECK(pc_mod_phase_unwrap_i32(a, b, &out) && out == expected);
        }
    }
    return 0;
}

static int test_phase_tick(void)
{
    uint32_t n, out = 19u;
    static const uint32_t phases[] = {0u, 1u, UINT32_C(0x3fffffff), UINT32_C(0x40000000),
        UINT32_C(0x7fffffff), UINT32_C(0x80000000), UINT32_C(0xc0000000), UINT32_MAX};
    static const uint32_t ticks[] = {1u, 2u, 3u, 7u, 8u, 65535u, UINT32_MAX};
    unsigned i, j;
    CHECK(!pc_phase_to_tick_u32(0u, 0u, &out) && out == 19u);
    CHECK(!pc_phase_to_tick_u32(0u, 1u, NULL));
    for (i = 0u; i < sizeof phases / sizeof phases[0]; ++i)
        for (j = 0u; j < sizeof ticks / sizeof ticks[0]; ++j) {
            CHECK(pc_phase_to_tick_u32(phases[i], ticks[j], &out));
            CHECK(out == phase_tick_reference(phases[i], ticks[j]));
            CHECK(out < ticks[j]);
        }
    for (n = 0u; n < 100000u; ++n) {
        const uint32_t phase = random_u32();
        uint32_t count = random_u32();
        if (count == 0u) count = 1u;
        CHECK(pc_phase_to_tick_u32(phase, count, &out));
        CHECK(out == phase_tick_reference(phase, count) && out < count);
    }
    return 0;
}

static int test_hypot(void)
{
    static const int32_t edge[] = {INT32_MIN, INT32_MIN + 1, -1, 0, 1, INT32_MAX - 1, INT32_MAX};
    unsigned i, j;
    uint32_t n;
    int32_t out = 77;
    for (i = 0u; i < 7u; ++i) for (j = 0u; j < 7u; ++j) {
        const long double norm = sqrtl((long double)edge[i] * edge[i] + (long double)edge[j] * edge[j]);
        const int64_t expected = round_even(norm);
        const bool success = pc_q_hypot2(edge[i], edge[j], &out);
        if (expected > INT32_MAX) CHECK(!success);
        else CHECK(success && out == expected);
    }
    out = 77;
    CHECK(!pc_q_hypot2(1, 1, NULL));
    CHECK(!pc_q_hypot2(INT32_MIN, INT32_MIN, &out) && out == 77);
    for (n = 0u; n < 100000u; ++n) {
        const int32_t x = random_i32(), y = random_i32();
        const int64_t expected = round_even(sqrtl((long double)x * x + (long double)y * y));
        const bool success = pc_q_hypot2(x, y, &out);
        if (expected > INT32_MAX) CHECK(!success && out == 77);
        else CHECK(success && out == expected);
        out = 77;
    }
    return 0;
}

static int test_stepper_angle(void)
{
    uint32_t n;
    int32_t out = 55;
    CHECK(!pc_stepper_angle_from_steps_q(1, 0u, &out) && out == 55);
    CHECK(!pc_stepper_angle_from_steps_q(1, 1u, NULL));
    CHECK(pc_stepper_angle_from_steps_q(1, UINT32_C(47185920), &out) && out == 0);
    CHECK(pc_stepper_angle_from_steps_q(1, UINT32_C(15728640), &out) && out == 2);
    CHECK(pc_stepper_angle_from_steps_q(-1, UINT32_C(15728640), &out) && out == -2);
    for (n = 0u; n < 100000u; ++n) {
        const int32_t steps = random_i32();
        uint32_t count = random_u32();
        const int64_t numerator = (int64_t)steps * INT64_C(23592960);
        uint64_t q = 0u, r = 0u;
        int64_t expected;
        if (count != 0u) {
            divide_reference(numerator < 0 ? (uint64_t)(-numerator) : (uint64_t)numerator,
                             count, &q, &r);
            if (r > count / 2u ||
                ((count & 1u) == 0u && r == count / 2u && (q & 1u) != 0u)) ++q;
        }
        expected = numerator < 0 ? -(int64_t)q : (int64_t)q;
        if (count == 0u) {
            out = 55;
            CHECK(!pc_stepper_angle_from_steps_q(steps, 0u, &out) && out == 55);
        } else {
            const bool ok = pc_stepper_angle_from_steps_q(steps, count, &out);
            if (expected < INT32_MIN || expected > INT32_MAX) CHECK(!ok && out == 55);
            else CHECK(ok && out == expected);
            out = 55;
        }
    }
    {
        static const int32_t steps[] = {INT32_MIN, INT32_MIN + 1, -1, 0, 1, INT32_MAX - 1, INT32_MAX};
        static const uint32_t counts[] = {1u, 2u, 3u, 360u, 65536u, UINT32_MAX};
        unsigned i, j;
        for (i = 0u; i < sizeof steps / sizeof steps[0]; ++i)
            for (j = 0u; j < sizeof counts / sizeof counts[0]; ++j) {
                const int64_t numerator = (int64_t)steps[i] * INT64_C(23592960);
                uint64_t q, r;
                int64_t expected;
                divide_reference(numerator < 0 ? (uint64_t)(-numerator) : (uint64_t)numerator,
                                 counts[j], &q, &r);
                if (r > counts[j] / 2u ||
                    ((counts[j] & 1u) == 0u && r == counts[j] / 2u && (q & 1u) != 0u)) ++q;
                expected = numerator < 0 ? -(int64_t)q : (int64_t)q;
                const bool ok = pc_stepper_angle_from_steps_q(steps[i], counts[j], &out);
                if (expected < INT32_MIN || expected > INT32_MAX) CHECK(!ok);
                else CHECK(ok && out == expected);
            }
    }
    return 0;
}

static int test_microstep(void)
{
    static const uint32_t counts[] = {1u, 2u, 3u, 4u, 7u, 65535u, 65536u, UINT32_MAX};
    uint32_t n, out = 71u;
    unsigned i;
    CHECK(!pc_stepper_microstep_phase_u32(0u, 0u, &out) && out == 71u);
    CHECK(!pc_stepper_microstep_phase_u32(0u, 1u, NULL));
    for (i = 0u; i < sizeof counts / sizeof counts[0]; ++i) {
        unsigned k;
        for (k = 0u; k < 3u; ++k) {
            const uint32_t index = k == 0u ? 0u : (k == 1u ? counts[i] / 2u : counts[i] - 1u);
            CHECK(pc_stepper_microstep_phase_u32(index, counts[i], &out));
            CHECK(out == microstep_reference(index, counts[i]));
        }
    }
    for (n = 0u; n < 100000u; ++n) {
        uint32_t count = random_u32();
        const uint32_t index = random_u32();
        if (count == 0u) count = 1u;
        CHECK(pc_stepper_microstep_phase_u32(index, count, &out));
        CHECK(out == microstep_reference(index, count));
        if (index <= UINT32_MAX - count) {
            CHECK(pc_stepper_microstep_phase_u32(index + count, count, &out));
            CHECK(out == microstep_reference(index, count));
        }
    }
    return 0;
}

int main(void)
{
#define RUN_API(id, fn) do { angulora_audit_current_api = (id); if (fn() != 0) return 1; } while (0)
    RUN_API(1u, test_bearing);
    RUN_API(2u, test_cartesian);
    RUN_API(3u, test_complex_type);
    RUN_API(4u, test_complex);
    RUN_API(5u, test_unwrap);
    RUN_API(6u, test_phase_tick);
    RUN_API(7u, test_hypot);
    RUN_API(8u, test_stepper_angle);
    RUN_API(9u, test_microstep);
#undef RUN_API
    puts("PASS: 8 callable APIs and pc_cpx_q public type; >=100000 seeded cases per applicable API");
    return 0;
}
