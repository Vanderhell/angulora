#include "../include/pc_cpx_phase_unwrap_q.h"

#include <stdint.h>
#include <limits.h>
#include <stddef.h>

#define CHECK(x) do { if (!(x)) return __LINE__; } while (0)

int main(void)
{
    pc_cpx_phase_unwrap_state_q state = {0u, 0, 0u};
    pc_cpx_phase_unwrap_state_q before;
    uint32_t i;

    CHECK(pc_cpx_phase_unwrap_q((pc_cpx_q){1000, 0}, &state));
    CHECK(state.initialized == 1u && state.unwrapped_phase_q0_32 == 0);
    CHECK(pc_cpx_phase_unwrap_q((pc_cpx_q){0, 1000}, &state));
    CHECK(state.unwrapped_phase_q0_32 >= INT64_C(0x40000000)-32 &&
          state.unwrapped_phase_q0_32 <= INT64_C(0x40000000)+32);
    CHECK(pc_cpx_phase_unwrap_q((pc_cpx_q){-1000, 0}, &state));
    CHECK(state.unwrapped_phase_q0_32 >= INT64_C(0x80000000)-64 &&
          state.unwrapped_phase_q0_32 <= INT64_C(0x80000000)+64);
    CHECK(pc_cpx_phase_unwrap_q((pc_cpx_q){0, -1000}, &state));
    CHECK(state.unwrapped_phase_q0_32 >= INT64_C(0xC0000000)-96 &&
          state.unwrapped_phase_q0_32 <= INT64_C(0xC0000000)+96);

    before = state;
    CHECK(!pc_cpx_phase_unwrap_q((pc_cpx_q){0, 0}, &state));
    CHECK(state.previous_phase_q0_32 == before.previous_phase_q0_32 &&
          state.unwrapped_phase_q0_32 == before.unwrapped_phase_q0_32 &&
          state.initialized == before.initialized);
    CHECK(!pc_cpx_phase_unwrap_q((pc_cpx_q){1, 0}, NULL));

    state.initialized = 2u;
    before = state;
    CHECK(!pc_cpx_phase_unwrap_q((pc_cpx_q){1000, 0}, &state));
    CHECK(state.initialized == before.initialized &&
          state.previous_phase_q0_32 == before.previous_phase_q0_32 &&
          state.unwrapped_phase_q0_32 == before.unwrapped_phase_q0_32);

    state.initialized = 1u;
    state.previous_phase_q0_32 = 0u;
    state.unwrapped_phase_q0_32 = 0;
    before = state;
    CHECK(!pc_cpx_phase_unwrap_q((pc_cpx_q){-1000, 0}, &state));
    CHECK(state.previous_phase_q0_32 == before.previous_phase_q0_32 &&
          state.unwrapped_phase_q0_32 == before.unwrapped_phase_q0_32);

    state.previous_phase_q0_32 = UINT32_C(0x40000000);
    state.unwrapped_phase_q0_32 = INT64_MAX;
    CHECK(!pc_cpx_phase_unwrap_q((pc_cpx_q){-1000, 0}, &state));
    CHECK(state.unwrapped_phase_q0_32 == INT64_MAX);
    state.previous_phase_q0_32 = 0u;
    state.unwrapped_phase_q0_32 = INT64_MIN;
    CHECK(!pc_cpx_phase_unwrap_q((pc_cpx_q){1000, -1}, &state));
    CHECK(state.unwrapped_phase_q0_32 == INT64_MIN);

    state = (pc_cpx_phase_unwrap_state_q){0u, 0, 0u};
    for (i = 0u; i < 100000u; ++i) {
        static const pc_cpx_q axes[4] = {
            {1000, 0}, {0, 1000}, {-1000, 0}, {0, -1000}
        };
        CHECK(pc_cpx_phase_unwrap_q(axes[i & 3u], &state));
    }
    CHECK(state.initialized == 1u);
    CHECK(state.unwrapped_phase_q0_32 >=
          INT64_C(99999) * INT64_C(0x40000000) - INT64_C(3200000));
    CHECK(state.unwrapped_phase_q0_32 <=
          INT64_C(99999) * INT64_C(0x40000000) + INT64_C(3200000));

    state = (pc_cpx_phase_unwrap_state_q){0u, 0, 0u};
    for (i = 0u; i < 100000u; ++i) {
        static const pc_cpx_q reverse_axes[4] = {
            {1000, 0}, {0, -1000}, {-1000, 0}, {0, 1000}
        };
        CHECK(pc_cpx_phase_unwrap_q(reverse_axes[i & 3u], &state));
    }
    CHECK(state.unwrapped_phase_q0_32 <=
          -INT64_C(99999) * INT64_C(0x40000000) + INT64_C(3200000));
    CHECK(state.unwrapped_phase_q0_32 >=
          -INT64_C(99999) * INT64_C(0x40000000) - INT64_C(3200000));
    return 0;
}
