#include "tev.h"
#include "tev_port.h"

int
tev_timer_init(tev_loop_t *loop, tev_timer_t *handle)
{
    tev__handle_init(loop, (tev_handle_t *)handle);
    handle->cb = NULL;
    
    return 0;
}

int
tev_timer_start(tev_timer_t *handle,
                tev_timer_cb cb,
                uint64_t time,
                uint64_t repeat)
{
    handle->time = tev__get_time() + time;
    handle->repeat = repeat;
    handle->cb = cb;

    return 0;
}
