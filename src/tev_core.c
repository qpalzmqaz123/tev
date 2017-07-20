#include "tev.h"
#include "tev_port.h"

static void
tev__update_time(tev_loop_t *loop)
{
    loop->time = tev__get_time();
}

int
tev_run(tev_loop_t *loop)
{
    while (0 == loop->is_cancel) {
        tev__update_time(loop);
    }

    return 0;
}

int
tev__handle_init(tev_loop_t *loop, tev_handle_t *handle)
{
    handle->data = NULL;
    handle->loop = loop;

    return 0;
}
