#include "tev.h"
#include "tev_port.h"

static void
tev__update_time(tev_loop_t *loop)
{
    loop->time = tev__get_time();
}

static int
tev__wait_io(tev_loop_t *loop, uint64_t timeout)
{
    return 0;
}

static void
tev__process_timer(tev_loop_t *loop)
{
    QUEUE *q;
    tev_timer_t *handle;

    QUEUE_FOREACH(q, loop->timer_queue) {
        handle = QUEUE_DATA(q, tev_timer_t, timer_queue);

        if (handle->time - handle->loop->time > 0) return;

        if (NULL != handle->cb) {
            handle->cb(handle);
        }

        handle->time = handle->loop->time + handle->repeat;
    }
}

int
tev_run(tev_loop_t *loop)
{
    while (0 == loop->is_cancel) {
        if (QUEUE_EMPTY(loop->handle_queue) &&
            QUEUE_EMPTY(loop->timer_queue) &&
            QUEUE_EMPTY(loop->idle_queue)) {
            break;
        }

        tev__update_time(loop);
        tev__process_timer(loop);

        tev__wait_io(loop, 0);

        tev__update_time(loop);
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
