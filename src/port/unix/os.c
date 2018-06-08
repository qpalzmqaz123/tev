#include "tev.h"
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#if TEV_CONF_ENABLE_OS == 1
typedef struct {
    pthread_mutex_t ev_lock;
    pthread_cond_t ev_cond;
} pthread_event_t;

void
tev__mutex_init(void **pdata)
{
    TEV_ASSERT_NOT_NULL(*pdata =
                        (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)));

    pthread_mutex_init((pthread_mutex_t *)*pdata, NULL);
}

void
tev__mutex_lock(void *data)
{
    pthread_mutex_lock((pthread_mutex_t *)data);
}

void
tev__mutex_unlock(void *data)
{
    pthread_mutex_unlock((pthread_mutex_t *)data);
}

void
tev__mutex_deinit(void *data)
{
    free(data);
}

void
tev__event_init(void **pdata)
{
    pthread_event_t *event;

    TEV_ASSERT_NOT_NULL(event =
                        (pthread_event_t *)malloc(sizeof(pthread_event_t)));

    pthread_mutex_init(&event->ev_lock, NULL);
    pthread_cond_init(&event->ev_cond, NULL);

    *pdata = event;
}

int
tev__event_wait(void *data, uint64_t timeout)
{
    pthread_event_t *event = data;
    struct timeval now;
    struct timeval wait;
    struct timeval to_wait;
    struct timespec to_wait_r;
    int res = 0;

    pthread_mutex_lock(&event->ev_lock);

    if (timeout == -1LL) {
        pthread_cond_wait(&event->ev_cond, &event->ev_lock);
    }
    else {
        gettimeofday(&now, NULL);
        wait.tv_sec = timeout / 1000;
        wait.tv_usec = (timeout % 1000) * 1000;
        timeradd(&now, &wait, &to_wait);
        to_wait_r.tv_sec = to_wait.tv_sec;
        to_wait_r.tv_nsec = to_wait.tv_usec * 1000;
        res = pthread_cond_timedwait(&event->ev_cond, &event->ev_lock,
                                     &to_wait_r);
    }

    pthread_mutex_unlock(&event->ev_lock);

    return res;
}

void
tev__event_set(void *data)
{
    pthread_event_t *event = data;

    pthread_mutex_lock(&event->ev_lock);
    pthread_cond_signal(&event->ev_cond);
    pthread_mutex_unlock(&event->ev_lock);
}

void
tev__event_deinit(void *data)
{
    free(data);
}

#endif
