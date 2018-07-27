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

    QUEUE_INSERT_TAIL(handle->loop->handle_queue, handle->handle_queue);
    
    return 0;
}

int
tev_async_send(tev_async_t *handle)
{
    int res = 0;
    QUEUE *q;
    tev_handle_t *h;

    tev__mutex_lock(handle->loop->mutex_handle);

    /* check if handle is already in active_queue */
    QUEUE_FOREACH(q, handle->loop->active_queue) {
        h = QUEUE_DATA(q, tev_handle_t, active_queue);

        if (h == (tev_handle_t *)handle) {
            res = 1;
            goto exit;
        }
    }

    QUEUE_INSERT_TAIL(handle->loop->active_queue, handle->active_queue);

    tev__mutex_unlock(handle->loop->mutex_handle);

    tev__event_set(handle->loop->event_handle);

exit:
    tev__mutex_unlock(handle->loop->mutex_handle);

    return res;
}

void
tev_async_close(tev_async_t *handle)
{
    QUEUE_REMOVE(handle->handle_queue);
    handle->is_cancel = 1;
}
