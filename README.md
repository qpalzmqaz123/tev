# Tiny eventloop

Tev is a low resource consumption and portable asynchronous I/O

## Features

- RTOS, POSIX, Vdev
- Timer
- Asynchronous event

## Examples

### Hello world

```c
#include <stdio.h>
#include "tev.h"

static void
timer_callback(tev_timer_t *handle)
{
    printf("Hello world\n");

    tev_timer_stop(handle);
}

int
main(void)
{
    tev_timer_t timer;

    tev_loop_t *loop = tev_default_loop();

    printf("The 'hello world' will print out after 1 second\n");

    tev_timer_init(loop, &timer);
    tev_timer_start(&timer, timer_callback, 1000, 0);

    tev_run(loop);

    return 0;
}
```

### async

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "tev.h"

tev_async_t async;

static void
async_callback(tev_async_t *handle)
{
    if (4 == (size_t)handle->data) {
        tev_async_close(handle);
    }

    printf("async: %ld\n", (size_t)handle->data);
}

static void *
task(void *p)
{
    for (int i = 0; i < 5; i++) {
        usleep(1000000);

        async.data = (void *)(size_t)i;
        tev_async_send(&async);
    }

    return NULL;
}

int
main(void)
{
    pthread_t thread;

    tev_loop_t *loop = tev_default_loop();

    tev_async_init(loop, &async, async_callback);

    pthread_create(&thread, NULL, task, NULL);

    tev_run(loop);

    return 0;
}
```

## API

### Core

- int **tev_run**(tev_loop_t *loop)
- void **tev_close**(tev_handle_t *handle, tev_close_cb close_cb)

### Loop

- tev_loop_t * **tev_loop_create**(tev_heap_fn_t *p)
- void **tev_loop_delete**(tev_loop_t *loop)
- tev_loop_t * **tev_default_loop**(void)

### Timer

- int **tev_timer_init**(tev_loop_t *loop, tev_timer_t *handle)
- int **tev_timer_start**(tev_timer_t *handle, tev_timer_cb cb, uint64_t time, uint64_t repeat)
- int **tev_timer_stop**(tev_timer_t *handle)

### Idle

- int **tev_idle_init**(tev_loop_t *loop, tev_idle_t *handle)
- int **tev_idle_start**(tev_idle_t *handle, tev_idle_cb cb)
- int **tev_idle_stop**(tev_idle_t *handle)

### Async

- int **tev_async_init**(tev_loop_t *loop, tev_async_t *handle, tev_async_cb cb)
- int **tev_async_send**(tev_async_t *handle)
- void **tev_async_close**(tev_async_t *handle)

### Porting

- void **tev__mutex_init**(void **pdata)
- void **tev__mutex_lock**(void *data)
- void **tev__mutex_unlock**(void *data)
- void **tev__mutex_deinit**(void *data)
- void **tev__event_init**(void **pdata)
- int **tev__event_wait**(void *data, uint64_t timeout)
- void **tev__event_set**(void *data)
- void **tev__event_deinit**(void *data)
- uint64_t **tev__get_time**(void)
