#include <stdint.h>
#include <stdio.h>
#include "pc_stepper_microstep_phase_u32.h"
int main(void){uint32_t i,n,e,o;while(scanf("%u%u%u",&i,&n,&e)==3){o=77;if(!pc_stepper_microstep_phase_u32(i,n,&o)||o!=e)return 1;}return 0;}
