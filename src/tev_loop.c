#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tev.h"
#include "tev_port.h"

static tev_loop_t *default_loop = NULL;

tev_loop_t *
tev_loop_create(tev_heap_fn_t *p)
{
    tev_loop_t *loop = NULL;

    TEV_ASSERT_NOT_NULL(p);

    loop = (tev_loop_t *)p->malloc(sizeof(tev_loop_t));
    TEV_ASSERT_NOT_NULL(loop);

    /* init */
    memcpy(&loop->heap_fn, p, sizeof(tev_heap_fn_t));
    QUEUE_INIT(loop->handle_queue);
    QUEUE_INIT(loop->timer_queue);
    QUEUE_INIT(loop->idle_queue);
    QUEUE_INIT(loop->active_async_queue);
    loop->is_cancel = 0;

#if TEV_CONF_ENABLE_OS == 1
    tev__mutex_init();
    tev__event_init();
#endif

    return loop;
}

tev_loop_t *
tev_default_loop(void)
{
    if (NULL == default_loop) {
        tev_heap_fn_t fn = {
            .malloc = malloc,
            .calloc = calloc,
            .realloc = realloc,
            .free = free
        };

        default_loop = tev_loop_create(&fn);
    }

    return default_loop;
}

void
tev_loop_delete(tev_loop_t *loop) {
    loop->heap_fn.free(loop);
}
