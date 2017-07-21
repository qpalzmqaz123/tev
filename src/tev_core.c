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

static int
tev__process_handle(tev_loop_t *loop)
{
    QUEUE *q;
    tev_handle_t *handle;

    QUEUE_FOREACH(q, loop->handle_queue) {
        handle = QUEUE_DATA(q, tev_handle_t, handle_queue);

        if (handle->process != NULL) {
            handle->process(handle);
        }
    }

    return 0;
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

        tev__wait_io(loop, 0);

        tev__update_time(loop);

        tev__process_handle(loop);
    }

    return 0;
}

int
tev__handle_init(tev_loop_t *loop, tev_handle_t *handle)
{
    handle->data = NULL;
    handle->loop = loop;
    handle->process = NULL;
    handle->is_cancel = 0;

    return 0;
}
