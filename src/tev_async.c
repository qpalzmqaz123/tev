#include "tev.h"
#include "tev_port.h"

int
tev_async_init(tev_loop_t *loop,
               tev_async_t *handle,
               tev_async_cb cb)
{
    tev__handle_init(loop, (tev_handle_t *)handle);

    handle->cb = cb;
    handle->handle_type = TEV_HANDLE_TYPE_ASYNC;
    
    return 0;
}

int
tev_async_send(tev_async_t *handle)
{
    tev__mutex_lock();

    QUEUE_INSERT_TAIL(handle->loop->active_async_queue, handle->queue);

    tev__mutex_unlock();

    tev__event_set();

    return 0;
}

void
tev_async_close(tev_async_t *handle)
{
    handle->is_cancel = 1;
}

int
tev_async_send_from_isr(tev_async_t *handle)
{
    QUEUE_INSERT_TAIL(handle->loop->active_async_queue, handle->queue);

    tev__event_set_from_isr();

    return 0;
}
