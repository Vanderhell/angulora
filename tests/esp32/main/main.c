#include <stdint.h>
#include <stdio.h>

#include "sdkconfig.h"
#include "esp_cpu.h"
#include "esp_private/esp_clk.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pc_bearing2_q.h"
#include "pc_cartesian_to_polar_q.h"
#include "pc_cpx_q.h"
#include "pc_cpx_to_polar_q.h"
#include "pc_mod_phase_unwrap_i32.h"
#include "pc_phase_to_tick_u32.h"
#include "pc_q_hypot2.h"
#include "pc_stepper_angle_from_steps_q.h"
#include "pc_stepper_microstep_phase_u32.h"

#define SAMPLE_COUNT 21u
#define CALLS_PER_BATCH 1000u

extern int angulora_audit_test_main(void);
extern unsigned angulora_audit_current_api;

static volatile uint64_t result_sink;
static volatile uint32_t phase_input = UINT32_C(0x87654321);
static volatile uint32_t cycle_input = UINT32_C(1000003);
static volatile int32_t signed_input = INT32_C(1234567);
static volatile int16_t cartesian_input = INT16_C(12345);
static volatile uint32_t calibration_input = 200u;

typedef uint64_t (*benchmark_fn)(void);
typedef struct {
    const char *name;
    benchmark_fn function;
} benchmark_case;

static uint64_t bearing_representative(void)
{
    uint32_t output = 0u;
    (void)pc_bearing2_q(10, -20, 12345, 67890, &output);
    return output;
}

static uint64_t cartesian_representative(void)
{
    int32_t radius = 0;
    uint32_t phase = 0u;
    (void)pc_cartesian_to_polar_q(cartesian_input, (int16_t)-12345,
                                  &radius, &phase);
    return (uint32_t)radius ^ phase;
}

static uint64_t complex_representative(void)
{
    uint32_t radius = 0u, phase = 0u;
    const pc_cpx_q value = {signed_input, (int32_t)-12345};
    (void)pc_cpx_to_polar_q(value, &radius, &phase);
    return radius ^ phase;
}

static uint64_t unwrap_representative(void)
{
    int32_t delta = 0;
    (void)pc_mod_phase_unwrap_i32(phase_input,
        phase_input + UINT32_C(0x01234567), &delta);
    return (uint32_t)delta;
}

static uint64_t tick_representative(void)
{
    uint32_t tick = 0u;
    (void)pc_phase_to_tick_u32(phase_input, cycle_input, &tick);
    return tick;
}

static uint64_t hypot_representative(void)
{
    int32_t norm = 0;
    (void)pc_q_hypot2(signed_input, INT32_C(7654321), &norm);
    return (uint32_t)norm;
}

static uint64_t stepper_representative(void)
{
    int32_t angle = 0;
    (void)pc_stepper_angle_from_steps_q(123, calibration_input, &angle);
    return (uint32_t)angle;
}

static uint64_t microstep_representative(void)
{
    uint32_t phase = 0u;
    (void)pc_stepper_microstep_phase_u32(phase_input, cycle_input, &phase);
    return phase;
}

static uint64_t bearing_boundary(void)
{
    uint32_t output = 0u;
    (void)pc_bearing2_q(INT32_MIN, INT32_MIN, INT32_MAX, INT32_MAX, &output);
    return output;
}

static uint64_t cartesian_boundary(void)
{
    int32_t radius = 0;
    uint32_t phase = 0u;
    (void)pc_cartesian_to_polar_q(INT16_MIN, INT16_MAX, &radius, &phase);
    return (uint32_t)radius ^ phase;
}

static uint64_t complex_boundary(void)
{
    uint32_t radius = 0u, phase = 0u;
    const pc_cpx_q value = {INT32_MIN, INT32_MAX};
    (void)pc_cpx_to_polar_q(value, &radius, &phase);
    return radius ^ phase;
}

static uint64_t unwrap_boundary(void)
{
    int32_t delta = 0;
    (void)pc_mod_phase_unwrap_i32(0u, UINT32_MAX, &delta);
    return (uint32_t)delta;
}

static uint64_t tick_boundary(void)
{
    uint32_t tick = 0u;
    (void)pc_phase_to_tick_u32(UINT32_MAX, UINT32_MAX, &tick);
    return tick;
}

static uint64_t hypot_boundary(void)
{
    int32_t norm = 0;
    (void)pc_q_hypot2(INT32_MIN, 0, &norm);
    return (uint32_t)norm;
}

static uint64_t stepper_boundary(void)
{
    int32_t angle = 0;
    (void)pc_stepper_angle_from_steps_q(INT32_MAX, 1u, &angle);
    return (uint32_t)angle;
}

static uint64_t microstep_boundary(void)
{
    uint32_t phase = 0u;
    (void)pc_stepper_microstep_phase_u32(UINT32_MAX - 1u, UINT32_MAX,
                                         &phase);
    return phase;
}

static uint64_t no_operation(void)
{
    return phase_input;
}

static benchmark_fn const volatile representative_functions[8] = {
    bearing_representative, cartesian_representative,
    complex_representative, unwrap_representative, tick_representative,
    hypot_representative, stepper_representative, microstep_representative
};

static benchmark_fn const volatile boundary_functions[8] = {
    bearing_boundary, cartesian_boundary, complex_boundary, unwrap_boundary,
    tick_boundary, hypot_boundary, stepper_boundary, microstep_boundary
};

static const benchmark_case cases[8] = {
    {"pc_bearing2_q", bearing_representative},
    {"pc_cartesian_to_polar_q", cartesian_representative},
    {"pc_cpx_to_polar_q", complex_representative},
    {"pc_mod_phase_unwrap_i32", unwrap_representative},
    {"pc_phase_to_tick_u32", tick_representative},
    {"pc_q_hypot2", hypot_representative},
    {"pc_stepper_angle_from_steps_q", stepper_representative},
    {"pc_stepper_microstep_phase_u32", microstep_representative}
};

static uint32_t run_batch(benchmark_fn function)
{
    uint32_t i;
    const uint32_t start = esp_cpu_get_cycle_count();
    for (i = 0u; i < CALLS_PER_BATCH; ++i) result_sink ^= function();
    return esp_cpu_get_cycle_count() - start;
}

static void sort_samples(int32_t values[SAMPLE_COUNT])
{
    unsigned i;
    for (i = 1u; i < SAMPLE_COUNT; ++i) {
        const int32_t value = values[i];
        unsigned j = i;
        while (j > 0u && values[j - 1u] > value) {
            values[j] = values[j - 1u];
            --j;
        }
        values[j] = value;
    }
}

static void measure(unsigned index, const char *scenario, benchmark_fn function)
{
    int32_t measured[SAMPLE_COUNT], baseline[SAMPLE_COUNT];
    unsigned i;
    for (i = 0u; i < SAMPLE_COUNT; ++i) baseline[i] = (int32_t)run_batch(no_operation);
    sort_samples(baseline);
    for (i = 0u; i < SAMPLE_COUNT; ++i) {
        uint32_t actual, empty;
        if ((i & 1u) != 0u) {
            empty = run_batch(no_operation);
            actual = run_batch(function);
        } else {
            actual = run_batch(function);
            empty = run_batch(no_operation);
        }
        measured[i] = (int32_t)((int64_t)actual - empty);
    }
    sort_samples(measured);
    printf("ANGULORA_CYCLE|api=%u|name=%s|input=%s|median_cycles_x100=%ld|"
           "min_cycles_x100=%ld|max_cycles_x100=%ld|noop_batch_median=%ld\n",
           index, cases[index - 1u].name, scenario,
           (long)((int64_t)measured[SAMPLE_COUNT / 2u] * 100 / CALLS_PER_BATCH),
           (long)((int64_t)measured[0] * 100 / CALLS_PER_BATCH),
           (long)((int64_t)measured[SAMPLE_COUNT - 1u] * 100 / CALLS_PER_BATCH),
           (long)baseline[SAMPLE_COUNT / 2u]);
}

static void benchmark(void)
{
    unsigned i;
    printf("ANGULORA_BENCH_META|cpu_mhz=%lu|samples=%u|repetitions=%u|"
           "method=paired_noop_subtraction|opt=O2|compiler=xtensa-esp-elf-gcc\n",
           (unsigned long)(esp_clk_cpu_freq() / 1000000u), SAMPLE_COUNT,
           CALLS_PER_BATCH);
    for (i = 0u; i < 8u; ++i) {
        measure(i + 1u, "representative", representative_functions[i]);
        measure(i + 1u, "boundary", boundary_functions[i]);
    }
    printf("ANGULORA_BENCH_SINK|%llu\n", (unsigned long long)result_sink);
}

void app_main(void)
{
    const uint32_t cpu_mhz = esp_clk_cpu_freq() / 1000000u;
    printf("ANGULORA_DEVICE|target=ESP32-S3|revision=0.2|"
           "MAC=3c:0f:02:d9:77:30|cpu_mhz=%lu|IDF=5.5.1|opt=O2\n",
           (unsigned long)cpu_mhz);
    if (angulora_audit_test_main() != 0) {
        printf("ANGULORA_ALL9_FAIL|first_failed_api=%u\n",
               angulora_audit_current_api);
        return;
    }
    printf("ANGULORA_ALL9_PASS|passed=9/9|callable=8|type=pc_cpx_q|"
           "first_failed_api=0\n");
    benchmark();
    {
        const uint32_t free_bytes = uxTaskGetStackHighWaterMark(NULL) *
                                    (uint32_t)sizeof(StackType_t);
        printf("ANGULORA_STACK|configured_bytes=%d|min_free_bytes=%lu|"
               "observed_use_bytes=%lu\n", CONFIG_ESP_MAIN_TASK_STACK_SIZE,
               (unsigned long)free_bytes,
               (unsigned long)(CONFIG_ESP_MAIN_TASK_STACK_SIZE - free_bytes));
    }
    for (;;) vTaskDelay(pdMS_TO_TICKS(1000));
}
