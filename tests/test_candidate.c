#include "../include/pc_bezier_tangent3_q.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
static uint32_t s=875123;static uint32_t r(void){s^=s<<13;s^=s>>17;s^=s<<5;return s;}
static int64_t re(long double x){long double a=fabsl(x),q=floorl(a),z=a-q;int64_t v=(int64_t)q;if(z>0.5L||(z==0.5L&&(v&1)))v++;return x<0?-v:v;}
static void ck(int16_t a,int16_t b,int16_t c,int16_t d,uint32_t t){long double x=(long double)t/65536.0L,u=1-x;int64_t ref=re(3*(u*u*((int32_t)b-a)+2*u*x*((int32_t)c-b)+x*x*((int32_t)d-c)));int32_t out;assert(pc_bezier_tangent3_q(a,b,c,d,t,&out)&&out==ref&&out>=-196605&&out<=196605);}
int main(void){int32_t out=4;assert(!pc_bezier_tangent3_q(0,0,0,0,65537,&out)&&out==4);for(int a=-24;a<=24;a++)for(int b=-24;b<=24;b++)for(int c=-24;c<=24;c++)for(int d=-24;d<=24;d++)for(uint32_t t=0;t<=65536;t+=1024)ck((int16_t)a,(int16_t)b,(int16_t)c,(int16_t)d,t);for(int i=0;i<300000;i++)ck((int16_t)r(),(int16_t)r(),(int16_t)r(),(int16_t)r(),r()%65537u);ck(INT16_MIN,INT16_MAX,INT16_MAX,INT16_MAX,0);ck(INT16_MAX,INT16_MIN,INT16_MIN,INT16_MIN,0);ck(INT16_MIN,INT16_MIN,INT16_MIN,INT16_MAX,65536);ck(INT16_MAX,INT16_MAX,INT16_MAX,INT16_MIN,65536);puts("PASS reduced exhaustive +300000 seeded");return 0;}
