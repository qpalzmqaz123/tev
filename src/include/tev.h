#ifndef __TEV_H__
#define __TEV_H__

#include <stdint.h>
#include <stdbool.h>
#include "queue.h"
#include "heap-inl.h"
#include "tev_conf.h"


#define TEV_ASSERT_NOT_NULL(x) do { \
    if (NULL == (x)) { \
        abort(); \
    } \
} while (0)


typedef enum {
    TEV_HANDLE_TYPE_TIMER = 0,
    TEV_HANDLE_TYPE_IDLE,
    TEV_HANDLE_TYPE_ASYNC
} tev_handle_type_t;


#define TEV_HANDLE_FIELDS \
    void *data; \
    tev_loop_t *loop; \
    QUEUE handle_queue[2]; \
    QUEUE active_queue[2]; \
    QUEUE close_queue[2]; \
    tev_handle_type_t handle_type; \
    int is_cancel; \
    tev_close_cb close_cb;

#define TEV_HANDLE_TIMER_FIELDS \
    struct heap_node heap_node; \
    uint64_t time; \
    uint64_t repeat; \
    tev_timer_cb cb;

#define TEV_HANDLE_IDLE_FILEDS \
    QUEUE idle_queue[2]; \
    tev_idle_cb cb;

#define TEV_HANDLE_ASYNC_FILEDS \
    tev_async_cb cb;


typedef struct tev_handle_s tev_handle_t;
typedef struct tev_timer_s tev_timer_t;
typedef struct tev_io_s tev_io_t;
typedef struct tev_idle_s tev_idle_t;
typedef struct tev_async_s tev_async_t;


typedef void (*tev_handle_cb)(tev_handle_t *);
typedef void (*tev_timer_cb)(tev_timer_t *);
typedef void (*tev_io_cb)(tev_io_t *);
typedef void (*tev_idle_cb)(tev_idle_t *);
typedef void (*tev_async_cb)(tev_async_t *);
typedef void (*tev_close_cb)(tev_handle_t* handle);


typedef struct {
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} tev_heap_fn_t;

typedef struct {
    struct heap timer_heap;
    QUEUE handle_queue[2];
    QUEUE idle_queue[2];
    QUEUE active_queue[2];
    QUEUE close_queue[2];
    int is_cancel;
    tev_heap_fn_t heap_fn;
    uint64_t time;
    void *mutex_handle;
    void *event_handle;
} tev_loop_t;

struct tev_timer_s {
    TEV_HANDLE_FIELDS
    TEV_HANDLE_TIMER_FIELDS
};

struct tev_handle_s {
    TEV_HANDLE_FIELDS
};

struct tev_io_s {
    TEV_HANDLE_FIELDS
};

struct tev_idle_s {
    TEV_HANDLE_FIELDS
    TEV_HANDLE_IDLE_FILEDS
};

struct tev_async_s {
    TEV_HANDLE_FIELDS
    TEV_HANDLE_ASYNC_FILEDS
};


/* private APIS */
int
tev__handle_init(tev_loop_t *loop, tev_handle_t *handle);


/* APIS */
tev_loop_t *
tev_loop_create(tev_heap_fn_t *p);

tev_loop_t *
tev_default_loop(void);

void
tev_loop_delete(tev_loop_t *loop);

/* core */
int
tev_run(tev_loop_t *loop);

void
tev_cleanup(tev_loop_t *loop);

void
tev_close(tev_handle_t* handle, tev_close_cb close_cb);

void
tev_update_time(tev_loop_t *loop);

/* timer */
int
tev_timer_init(tev_loop_t *loop, tev_timer_t *handle);

int
tev_timer_start(tev_timer_t *handle,
                tev_timer_cb cb,
                uint64_t time,
                uint64_t repeat);

int
tev__timer_stop(tev_timer_t *handle);

/* idle */
int
tev_idle_init(tev_loop_t *loop, tev_idle_t *handle);

int
tev_idle_start(tev_idle_t *handle, tev_idle_cb cb);

int
tev__idle_stop(tev_idle_t *handle);

/* async */
int
tev_async_init(tev_loop_t *loop, tev_async_t *handle, tev_async_cb cb);

int
tev_async_send(tev_async_t *handle);

bool
tev_async_prepared(tev_async_t *handle);

void
tev__async_close(tev_async_t *handle);

#endif
