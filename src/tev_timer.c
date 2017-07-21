#include "tev.h"
#include "tev_port.h"

int
tev_timer_init(tev_loop_t *loop, tev_timer_t *handle)
{
    tev__handle_init(loop, (tev_handle_t *)handle);

    handle->cb = NULL;
    handle->handle_type = TEV_HANDLE_TYPE_TIMER;
    
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

    QUEUE_INSERT_TAIL(handle->loop->timer_queue, handle->timer_queue);

    return 0;
}

int
tev_timer_stop(tev_timer_t *handle)
{
    QUEUE_REMOVE(handle->timer_queue);

    return 0;
}
