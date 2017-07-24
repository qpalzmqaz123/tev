#include <stdio.h>
#include "tev.h"

#include <unistd.h>

static void
async_callback(tev_async_t *handle)
{

}

int
main(void)
{
    tev_loop_t *loop = tev_default_loop();

    tev_async_t async;

    tev_async_init(loop, &async, async_callback);

    tev_run(loop);

    return 0;
}
