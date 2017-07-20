#include "tev.h"
#include "tev_port.h"

static void
tev__update_time(tev_loop_t *loop)
{
    loop->time = tev__get_time();
}

void
tev_run(tev_loop_t *loop)
{
    while (0 == loop->is_stop) {
        tev__update_time(loop);
    }
}
