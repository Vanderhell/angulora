#include "../include/pc_cpx_phase_unwrap_q.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t next_u32(uint32_t *state)
{
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static uint32_t ideal_phase(int32_t real, int32_t imag)
{
    const long double turns = atan2l((long double)imag, (long double)real) /
                              (2.0L * acosl(-1.0L));
    const int64_t rounded = (int64_t)llroundl(turns * 4294967296.0L);
    return (uint32_t)rounded;
}

int main(void)
{
    pc_cpx_phase_unwrap_state_q state = {0};
    uint32_t random_state = UINT32_C(0x3310A55A);
    uint32_t phase = 0u;
    int64_t ideal_total = 0;
    uint32_t i;

    for (i = 0u; i < 100000u; ++i) {
        const int32_t step = (int32_t)(next_u32(&random_state) %
                                      UINT32_C(0x10000001)) - INT32_C(0x08000000);
        const long double angle = 2.0L * acosl(-1.0L) *
                                  ((long double)phase / 4294967296.0L);
        const int32_t real = (int32_t)llroundl(1000000000.0L * cosl(angle));
        const int32_t imag = (int32_t)llroundl(1000000000.0L * sinl(angle));
        const uint32_t next_phase = phase + (uint32_t)step;
        const uint32_t exact_code = ideal_phase(real, imag);

        if (i == 0u) ideal_total = (int64_t)exact_code;
        else {
            const uint32_t forward = exact_code -
                (uint32_t)(ideal_total & INT64_C(0xFFFFFFFF));
            const int32_t delta = forward < UINT32_C(0x80000000)
                ? (int32_t)forward
                : -(int32_t)(0u - forward);
            ideal_total += delta;
        }
        if (!pc_cpx_phase_unwrap_q((pc_cpx_q){real, imag}, &state)) return 1;
        if (llabs(state.unwrapped_phase_q0_32 - ideal_total) > 64) return 2;
        phase = next_phase;
    }
    return 0;
}