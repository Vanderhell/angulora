#include <assert.h>
#include <stdint.h>
#include "../include/pc_time_since_timestamp_u64.h"
int main(void)
{
    uint64_t age=77;
    assert(pc_time_since_timestamp_u64(5,5,&age) && age==0);
    assert(pc_time_since_timestamp_u64(10,3,&age) && age==7);
    assert(pc_time_since_timestamp_u64(UINT64_MAX,0,&age) && age==UINT64_MAX);
    age=77;
    assert(!pc_time_since_timestamp_u64(3,4,&age) && age==77);
    assert(!pc_time_since_timestamp_u64(1,0,0));
    return 0;
}
