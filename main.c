#include <stdio.h>
#include "tev.h"

int main(void)
{
    tev_loop_t *loop = tev_loop_get_default();

    tev_run(loop);

    return 0;
}
