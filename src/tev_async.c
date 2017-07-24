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
    return 0;
}
