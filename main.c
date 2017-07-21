#include <stdio.h>
#include "tev.h"

#include <unistd.h>

static void
callback(tev_timer_t *handle)
{
    static int i = 0;
    i++;

    if (i > 5) {
        tev_timer_stop(handle);
    }
    printf("callback\n");
}

static void
idle_callback(tev_idle_t *handle)
{
    static int i = 0;
    i++;

    if (i > 20) {
        tev_idle_stop(handle);
    }

    printf("idle\n");
    usleep(100000);
}

int
main(void)
{
    tev_loop_t *loop = tev_default_loop();

    tev_timer_t timer;
    tev_idle_t idle;

    tev_timer_init(loop, &timer);
    tev_timer_start(&timer, callback, 1000, 500);

    tev_idle_init(loop, &idle);
    tev_idle_start(&idle, idle_callback);

    tev_run(loop);

    return 0;
}
