#ifndef ANGULORA_TRIGONOMETRY_MOD_PHASE_UNWRAP_I32_H
#define ANGULORA_TRIGONOMETRY_MOD_PHASE_UNWRAP_I32_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Shortest signed delta on the uint32_t phase ring; antipodes are ambiguous. */
bool angulora_mod_phase_unwrap_i32(uint32_t previous, uint32_t current,
                             int32_t *delta);


#ifdef __cplusplus
}
#endif

#endif /* ANGULORA_TRIGONOMETRY_MOD_PHASE_UNWRAP_I32_H */
