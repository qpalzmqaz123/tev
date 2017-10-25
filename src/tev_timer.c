#include "tev.h"
#include "tev_port.h"

static int
tev__timer_compare(const struct heap_node *a, const struct heap_node *b)
{
    tev_timer_t *timer_a = (tev_timer_t *)((size_t)a - offsetof(tev_timer_t, heap_node));
    tev_timer_t *timer_b = (tev_timer_t *)((size_t)b - offsetof(tev_timer_t, heap_node));

    return timer_a->time < timer_b->time ? 1 : 0;
}

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

    heap_insert(&handle->loop->timer_heap, &handle->heap_node, tev__timer_compare);

    return 0;
}

int
tev_timer_stop(tev_timer_t *handle)
{
    heap_remove(&handle->loop->timer_heap, &handle->heap_node, tev__timer_compare);

    return 0;
}
