#include <primitivecore/trigonometry/angle/inverse/pc_bearing2_q.h>
#include <stddef.h>

static const int64_t atan_turns_q32[31]={
    INT64_C(536870912),INT64_C(316933406),INT64_C(167458907),INT64_C(85004756),
    INT64_C(42667331),INT64_C(21354465),INT64_C(10679838),INT64_C(5340245),
    INT64_C(2670163),INT64_C(1335087),INT64_C(667544),INT64_C(333772),
    INT64_C(166886),INT64_C(83443),INT64_C(41722),INT64_C(20861),INT64_C(10430),
    INT64_C(5215),INT64_C(2608),INT64_C(1304),INT64_C(652),INT64_C(326),INT64_C(163),
    INT64_C(81),INT64_C(41),INT64_C(20),INT64_C(10),INT64_C(5),INT64_C(3),INT64_C(1),INT64_C(1)
};

static int64_t shift_zero(int64_t value,unsigned shift)
{
    const uint64_t mag=value<0?(uint64_t)(-value):(uint64_t)value;
    const int64_t q=(int64_t)(mag>>shift);
    return value<0?-q:q;
}

bool rm_bearing2_q(int32_t origin_x,int32_t origin_y,
                   int32_t target_x,int32_t target_y,
                   uint32_t *bearing_turns_q0_32)
{
    int64_t x,y,z=0;
    int64_t dx,dy;
    unsigned i;
    if(bearing_turns_q0_32==NULL)return false;
    dx=(int64_t)target_x-origin_x;dy=(int64_t)target_y-origin_y;
    if(dx==0&&dy==0){*bearing_turns_q0_32=0u;return true;}
    x=dx*INT64_C(652032874);y=dy*INT64_C(652032874);
    if(x<0){z=y>=0?INT64_C(2147483648):-INT64_C(2147483648);x=-x;y=-y;}
    for(i=0u;i<31u;++i){
        const int64_t sx=shift_zero(x,i),sy=shift_zero(y,i);
        if(y>0){x+=sy;y-=sx;z+=atan_turns_q32[i];}
        else if(y<0){x-=sy;y+=sx;z-=atan_turns_q32[i];}
    }
    *bearing_turns_q0_32=(uint32_t)(uint64_t)z;
    return true;
}
