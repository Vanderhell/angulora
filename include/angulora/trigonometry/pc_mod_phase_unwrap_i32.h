#ifndef PRIMITIVECORE_MOD_PHASE_UNWRAP_I32_H
#define PRIMITIVECORE_MOD_PHASE_UNWRAP_I32_H
#include <stdbool.h>
#include <stdint.h>
/* Shortest signed delta on the uint32_t phase ring; antipodes are ambiguous. */
bool rm_mod_phase_unwrap_i32(uint32_t previous, uint32_t current,
                             int32_t *delta);
#endif

/* Canonical facade; the legacy implementation and contract are preserved. */
#ifndef pc_mod_phase_unwrap_i32
#define pc_mod_phase_unwrap_i32 rm_mod_phase_unwrap_i32
#endif
#ifndef pc_mod_phase_unwrap_i32
#define pc_mod_phase_unwrap_i32 rm_mod_phase_unwrap_i32
#endif
#ifndef PC_PRIMITIVECORE_MOD_PHASE_UNWRAP_I32_H
#define PC_PRIMITIVECORE_MOD_PHASE_UNWRAP_I32_H PRIMITIVECORE_MOD_PHASE_UNWRAP_I32_H
#endif

