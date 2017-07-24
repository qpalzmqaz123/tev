#include <stdio.h>
#include "tev.h"
#include <pthread.h>
#include <unistd.h>

tev_async_t async;
tev_async_t async1;
tev_timer_t timer;

static void
async_callback(tev_async_t *handle)
{
    static uint32_t i = 0;
    printf("async: %d\n", i++);
}

static void
async_callback1(tev_async_t *handle)
{
    static uint32_t i = 0;
    printf("--- async: %d\n", i++);
}

static void
timer_callback(tev_timer_t *handle)
{
    printf("timer\n");
}

static void *
task1(void *p)
{
    while (1) {
        usleep(500000);
        tev_async_send(&async);
    }

    return NULL;
}

static void *
task2(void *p)
{
    while (1) {
        usleep(1000000);
        tev_async_send(&async1);
    }

    return NULL;
}

int
main(void)
{
    pthread_t thread1;
    pthread_t thread2;

    tev_loop_t *loop = tev_default_loop();

    tev_async_init(loop, &async, async_callback);
    tev_async_init(loop, &async1, async_callback1);

    tev_timer_init(loop, &timer);
    tev_timer_start(&timer, timer_callback, 1000, 1000);

    pthread_create(&thread1, NULL, task1, NULL);
    pthread_create(&thread2, NULL, task2, NULL);

    tev_run(loop);

    return 0;
}
