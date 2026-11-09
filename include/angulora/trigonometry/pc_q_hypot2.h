#ifndef PC_Q_HYPOT2_H
#define PC_Q_HYPOT2_H
#include <stdbool.h>
#include <stdint.h>
/* Nearest-even Euclidean norm of same-scale signed Q raw values. False on
 * null output or int32 result overflow; failure preserves *out. */
bool pc_q_hypot2(int32_t x, int32_t y, int32_t *out);
#endif

/* Canonical facade; the legacy implementation and contract are preserved. */
#ifndef pc_q_hypot2
#define pc_q_hypot2 pc_q_hypot2
#endif
#ifndef pc_q_hypot2
#define pc_q_hypot2 pc_q_hypot2
#endif
#ifndef PC_PC_Q_HYPOT2_H
#define PC_PC_Q_HYPOT2_H PC_Q_HYPOT2_H
#endif
