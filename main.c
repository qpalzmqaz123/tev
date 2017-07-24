#include <stdio.h>
#include "tev.h"

#include <unistd.h>

tev_async_t async;
tev_timer_t timer;

static void
async_callback(tev_async_t *handle)
{
    printf("async\n");
}

static void
timer_callback(tev_timer_t *handle)
{
    printf("timer\n");
    tev_async_send(&async);
}

int
main(void)
{
    tev_loop_t *loop = tev_default_loop();

    tev_async_init(loop, &async, async_callback);

    tev_timer_init(loop, &timer);
    tev_timer_start(&timer, timer_callback, 1000, 1000);

    tev_run(loop);

    return 0;
}
