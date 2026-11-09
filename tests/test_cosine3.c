#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/cosine_similarity3_q15.h"

static uint32_t state=UINT32_C(0x1121c05e);
static uint32_t next_u32(void){state^=state<<13;state^=state>>17;state^=state<<5;return state;}

int main(void)
{
    int32_t out=123;
    assert(!pc_cosine_similarity3_q15_experiment(0,0,0,1,0,0,&out)&&out==123);
    assert(!pc_cosine_similarity3_q15_experiment(1,0,0,0,0,0,&out)&&out==123);
    assert(!pc_cosine_similarity3_q15_experiment(1,0,0,1,0,0,NULL));
    assert(pc_cosine_similarity3_q15_experiment(1,0,0,2,0,0,&out)&&out==32768);
    assert(pc_cosine_similarity3_q15_experiment(1,0,0,-2,0,0,&out)&&out==-32768);
    assert(pc_cosine_similarity3_q15_experiment(1,0,0,0,1,0,&out)&&out==0);
    assert(pc_cosine_similarity3_q15_experiment(-4,-4,-4,-4,-4,-4,&out)&&out==32768);
    assert(pc_cosine_similarity3_q15_experiment(-4,-4,-4,4,4,4,&out)&&out==-32768);
    for(unsigned i=0;i<20000u;++i){
        int32_t a[3],b[3],ab,ba;
        for(unsigned k=0;k<3u;++k){a[k]=(int32_t)next_u32();b[k]=(int32_t)next_u32();}
        if(a[0]==0&&a[1]==0&&a[2]==0)a[0]=1;
        if(b[0]==0&&b[1]==0&&b[2]==0)b[1]=1;
        assert(pc_cosine_similarity3_q15_experiment(a[0],a[1],a[2],b[0],b[1],b[2],&ab));
        assert(pc_cosine_similarity3_q15_experiment(b[0],b[1],b[2],a[0],a[1],a[2],&ba));
        assert(ab==ba&&ab>=-32768&&ab<=32768);
    }
    puts("PASS cosine Q15 validity, axes, symmetry, and 20000 seeded full-width pairs");
    return 0;
}