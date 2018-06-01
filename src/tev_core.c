#include "tev.h"
#include "tev_port.h"

static void
tev__update_time(tev_loop_t *loop)
{
    loop->time = tev__get_time();
}

static int
tev__wait_event(tev_loop_t *loop, uint64_t timeout)
{
    tev__event_wait(loop->event_handle, timeout);
    return 0;
}

static void
tev__process_event(tev_loop_t *loop)
{
    QUEUE *q;
    tev_handle_t *handle;

    tev__mutex_lock(loop->mutex_handle);

    if (QUEUE_EMPTY(loop->active_queue)) {
        goto EXIT;
    }

    QUEUE_FOREACH(q, loop->active_queue) {
        handle = QUEUE_DATA(q, tev_handle_t, active_queue);

        if (!handle->is_cancel) {
            switch (handle->handle_type) {
            case TEV_HANDLE_TYPE_TIMER:
                if (NULL != ((tev_timer_t *)handle)->cb) {
                    ((tev_timer_t *)handle)->cb((tev_timer_t *)handle);
                }
                break;
            case TEV_HANDLE_TYPE_ASYNC:
                if (NULL != ((tev_async_t *)handle)->cb) {
                    ((tev_async_t *)handle)->cb((tev_async_t *)handle);
                }
                break;
            default:
                break;
            }
        }
    }

    QUEUE_INIT(loop->active_queue);

EXIT:
    tev__mutex_unlock(loop->mutex_handle);
}

static uint64_t
tev__process_timer(tev_loop_t *loop)
{
    uint64_t timeout = 0;
    struct heap_node *min;
    tev_timer_t *handle;

    if (0 == loop->timer_heap.nelts) {
        return (uint64_t)-1;
    }

    min = heap_min(&loop->timer_heap);
    handle = (tev_timer_t *)((size_t)min - offsetof(tev_timer_t, heap_node));

    /* calulate timeout */
    timeout = handle->loop->time > handle->time ? 0 : handle->time - handle->loop->time;

    if (0 == timeout) {
        tev_timer_stop(handle);
        tev_timer_start(handle, handle->cb, handle->repeat, handle->repeat);

        QUEUE_INSERT_TAIL(loop->active_queue, handle->active_queue);
    }

    return timeout;
}

static void
tev__process_idle(tev_loop_t *loop)
{
    QUEUE *q;
    tev_idle_t *handle;

    if (QUEUE_EMPTY(loop->idle_queue)) {
        return;
    }

    QUEUE_FOREACH(q, loop->idle_queue) {
        handle = QUEUE_DATA(q, tev_idle_t, idle_queue);

        if (NULL != handle->cb) {
            handle->cb(handle);
        }
    }
}

int
tev_run(tev_loop_t *loop)
{
    uint64_t timeout = 0;

    while (0 == loop->is_cancel) {
        if (0 == loop->timer_heap.nelts &&
            QUEUE_EMPTY(loop->handle_queue) &&
            QUEUE_EMPTY(loop->idle_queue)) {
            break;
        }

        tev__update_time(loop);

        tev__process_idle(loop);
        timeout = tev__process_timer(loop);
        if (QUEUE_EMPTY(loop->active_queue)) {
            tev__wait_event(loop, timeout);
        }
        tev__process_event(loop);
    }

    return 0;
}

int
tev__handle_init(tev_loop_t *loop, tev_handle_t *handle)
{
    handle->data = NULL;
    handle->loop = loop;
    handle->is_cancel = 0;

    return 0;
}

void
tev_close(tev_handle_t *handle, tev_close_cb close_cb)
{
    switch (handle->handle_type) {
        case TEV_HANDLE_TYPE_ASYNC:
            tev_async_close((tev_async_t *)handle);
            break;
        default:
            break;
    }
}
