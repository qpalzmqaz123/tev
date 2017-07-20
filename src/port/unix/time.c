#include "tev.h"
#include "sys/time.h"

uint64_t
tev__get_time(void)
{
    uint64_t time;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    time = (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;

    return time;
}
