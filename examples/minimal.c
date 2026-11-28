#include <stdint.h>
#include <stdio.h>
#include <angulora/trigonometry/cartesian_to_polar_q.h>

int main(void)
{
    int32_t radius_q15 = 0;
    uint32_t phase_turns_q0_32 = 0u;
    if (!angulora_cartesian_to_polar_q(16384, 16384,
                                      &radius_q15, &phase_turns_q0_32)) {
        return 1;
    }
    printf("radius_q15=%ld phase_q0_32=%lu\n",
           (long)radius_q15, (unsigned long)phase_turns_q0_32);
    return 0;
}
