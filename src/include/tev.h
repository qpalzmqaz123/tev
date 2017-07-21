#ifndef __TEV_H__
#define __TEV_H__

#include "queue.h"
#include <stdint.h>


#define TEV_ASSERT_NOT_NULL(x) do { \
    if (NULL == (x)) { \
        abort(); \
    } \
} while (0)


typedef enum {
    TEV_HANDLE_TYPE_TIMER = 0,
    TEV_HANDLE_TYPE_IDLE = 0
} tev_handle_type_t;


#define TEV_HANDLE_FIELDS \
    void *data; \
    tev_loop_t *loop; \
    QUEUE handle_queue[2]; \
    tev_handle_type_t handle_type; \
    tev_handle_cb process; \
    int is_cancel;

#define TEV_HANDLE_TIMER_FIELDS \
    QUEUE timer_queue[2]; \
    uint64_t time; \
    uint64_t repeat; \
    tev_timer_cb cb;

#define TEV_HANDLE_IDLE_FILEDS \
    QUEUE idle_queue[2]; \
    tev_idle_cb cb;


typedef struct tev_handle_s tev_handle_t;
typedef struct tev_timer_s tev_timer_t;
typedef struct tev_io_s tev_io_t;
typedef struct tev_idle_s tev_idle_t;


typedef void (*tev_handle_cb)(tev_handle_t *);
typedef void (*tev_timer_cb)(tev_timer_t *);
typedef void (*tev_io_cb)(tev_io_t *);
typedef void (*tev_idle_cb)(tev_idle_t *);


typedef struct {
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} tev_heap_fn_t;

typedef struct {
    QUEUE handle_queue[2];
    QUEUE timer_queue[2];
    QUEUE idle_queue[2];
    int is_cancel;
    tev_heap_fn_t heap_fn;
    uint64_t time;
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


/* private APIS */
int
tev__handle_init(tev_loop_t *loop, tev_handle_t *handle);


/* APIS */
tev_loop_t *
tev_loop_create(tev_heap_fn_t *p);

tev_loop_t *
tev_default_loop(void);

int
tev_run(tev_loop_t *loop);

int
tev_timer_init(tev_loop_t *loop, tev_timer_t *handle);

int
tev_timer_start(tev_timer_t *handle,
                tev_timer_cb cb,
                uint64_t time,
                uint64_t repeat);

int
tev_timer_stop(tev_timer_t *handle);

#endif
