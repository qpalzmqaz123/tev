#include "tev.h"

int
tev_idle_init(tev_loop_t *loop, tev_idle_t *handle)
{
    tev__handle_init(loop, (tev_handle_t *)handle);

    handle->handle_type = TEV_HANDLE_TYPE_IDLE;
    handle->cb = NULL;

    return 0;
}
