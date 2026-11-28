#ifndef ANGULORA_TRIGONOMETRY_Q_HYPOT2_H
#define ANGULORA_TRIGONOMETRY_Q_HYPOT2_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Nearest-even Euclidean norm of same-scale signed Q raw values. False on
 * null output or int32 result overflow; failure preserves *out. */
bool angulora_q_hypot2(int32_t x, int32_t y, int32_t *out);


#ifdef __cplusplus
}
#endif

#endif /* ANGULORA_TRIGONOMETRY_Q_HYPOT2_H */
