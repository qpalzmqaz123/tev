#include "tev.h"

int
tev_idle_init(tev_loop_t *loop, tev_idle_t *handle)
{
    tev__handle_init(loop, (tev_handle_t *)handle);

    handle->handle_type = TEV_HANDLE_TYPE_IDLE;
    handle->cb = NULL;

    return 0;
}

int
tev_idle_start(tev_idle_t *handle, tev_idle_cb cb)
{
    handle->cb = cb;

    QUEUE_INSERT_TAIL(handle->loop->idle_queue, handle->idle_queue);

    return 0;
}

int
tev_idle_stop(tev_idle_t *handle)
{
    QUEUE_REMOVE(handle->idle_queue);

    return 0;
}
