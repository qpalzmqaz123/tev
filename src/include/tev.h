#ifndef __TEV_H__
#define __TEV_H__

#include "queue.h"
#include <stdint.h>

#define TEV_ASSERT_NOT_NULL(x) do { \
    if (NULL == (x)) { \
        abort(); \
    } \
} while (0)

#define TEV_HANDLE_FIELDS \
    void *data; \
    tev_loop_t *loop;

#define TEV_HANDLE_TIMER_FIELDS \
    uint64_t time; \
    uint64_t repeat; \
    int is_cancel; \
    void (*cb)(tev_handle_t *);

typedef struct {
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} tev_heap_fn_t;

typedef struct {
    QUEUE handle_queue[2];
    int is_cancel;
    tev_heap_fn_t heap_fn;
    uint64_t time;
} tev_loop_t;

typedef struct {
    TEV_HANDLE_FIELDS
} tev_handle_t;

typedef struct {
    TEV_HANDLE_FIELDS
    TEV_HANDLE_TIMER_FIELDS
} tev_timer_t;

typedef struct {
    TEV_HANDLE_FIELDS
} tev_io_t;

typedef void (*tev_handle_cb)(tev_handle_t *);

/* private APIS */
int
tev__handle_init(tev_loop_t *loop, tev_handle_t *handle);

/* APIS */
tev_loop_t *
tev_loop_create(tev_heap_fn_t *p);

tev_loop_t *
tev_loop_get_default();

int
tev_run(tev_loop_t *loop);
#endif
