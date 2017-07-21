#include "tev.h"
#include "tev_port.h"

static void
tev__timer_process(tev_handle_t *h)
{
    tev_timer_t *handle = (tev_timer_t *)h;

    if (handle->time - handle->loop->time > 0) return;

    if (handle->cb) {
        handle->cb(handle);
    }

    handle->time = handle->loop->time + handle->repeat;
}

int
tev_timer_init(tev_loop_t *loop, tev_timer_t *handle)
{
    tev__handle_init(loop, (tev_handle_t *)handle);

    handle->cb = NULL;
    handle->handle_type = TEV_HANDLE_TYPE_TIMER;
    handle->process = tev__timer_process;
    
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

    QUEUE_INSERT_TAIL(handle->loop->handle_queue, handle->handle_queue);

    return 0;
}

int
tev_timer_stop(tev_timer_t *handle)
{
    handle->is_cancel = 1;

    QUEUE_REMOVE(handle->handle_queue);

    return 0;
}
