#include "../include/pc_axis_angle_small_q.h"
/* Held-out cross-family validation source; dependency is outside this product. */
#include <stddef.h>
#include <stdint.h>
#define ONE INT32_C(1073741824)
#define CHECK(c) do{if(!(c))return __LINE__;}while(0)
int main(void){int32_t axis[3]={0,0,ONE},q[4]={71,72,73,74};uint32_t seed=1393;
 CHECK(!pc_axis_angle_small_q(NULL,0,q));CHECK(!pc_axis_angle_small_q(axis,0,NULL));
 CHECK(pc_axis_angle_small_q(axis,0,q)&&q[0]==ONE&&q[1]==0&&q[2]==0&&q[3]==0);
 CHECK(pc_axis_angle_small_q(axis,655,q));CHECK(q[0]>INT32_C(1073720000)&&q[3]>0);
 {int32_t n[4];CHECK(pc_axis_angle_small_q(axis,-655,n));CHECK(n[0]==q[0]&&n[1]==0&&n[2]==0&&n[3]==-q[3]);}
 CHECK(!pc_axis_angle_small_q(axis,656,q));CHECK(!pc_axis_angle_small_q(axis,-656,q));
 axis[0]=axis[1]=axis[2]=0;q[0]=71;
 CHECK(!pc_axis_angle_small_q(axis,0,q)&&q[0]==71);
 axis[2]=ONE/2;
 CHECK(!pc_axis_angle_small_q(axis,10,q)&&q[0]==71);
 axis[2]=0;
 axis[0]=ONE+1;CHECK(!pc_axis_angle_small_q(axis,10,q)&&q[0]==71);
 axis[0]=0;axis[2]=ONE;
 for(unsigned n=0;n<10000;n++){int32_t raw[4]={0,0,0,0},unit[4];for(unsigned i=1;i<4;i++){seed=seed*UINT32_C(1664525)+UINT32_C(1013904223);raw[i]=(int32_t)((int64_t)(seed%UINT32_C(2147483649))-ONE);}if(!(raw[1]|raw[2]|raw[3]))raw[3]=1;CHECK(pc_quat_normalize_q(raw,unit));for(unsigned i=0;i<3;i++)axis[i]=unit[i+1];seed=seed*UINT32_C(1664525)+UINT32_C(1013904223);int32_t a=(int32_t)(seed%1311u)-655;CHECK(pc_axis_angle_small_q(axis,a,q));}
 return 0;}