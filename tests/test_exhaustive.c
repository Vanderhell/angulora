#include "pc_tick_to_phase_q.h"
#include "roundmath/phase_to_tick_u32.h"
#include <assert.h>
#include <stdint.h>
int main(void){uint32_t q,t;assert(pc_tick_to_phase_q(0,1,&q)&&q==0);assert(pc_tick_to_phase_q(7,8,&q)&&q==0xe0000000u);assert(pc_phase_to_tick_u32(q,8,&t)&&t==7);for(uint32_t n=1;n<2048;n++)for(uint32_t x=0;x<n;x++){assert(pc_tick_to_phase_q(x,n,&q));assert(pc_phase_to_tick_u32(q,n,&t)&&t==x);}q=77;assert(!pc_tick_to_phase_q(8,8,&q)&&q==77);assert(!pc_tick_to_phase_q(0,0,&q)&&q==77);assert(!pc_tick_to_phase_q(0,1,0));return 0;}

