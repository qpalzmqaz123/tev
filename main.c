#include <stdio.h>
#include "tev.h"

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

int
main(void)
{
    tev_loop_t *loop = tev_loop_get_default();

    tev_timer_t timer;

    tev_timer_init(loop, &timer);
    tev_timer_start(&timer, callback, 1000, 500);

    tev_run(loop);

    return 0;
}
