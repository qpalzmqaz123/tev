#include "tev.h"
#include <pthread.h>
#include <sys/time.h>

#if TEV_CONF_ENABLE_OS == 1
static pthread_mutex_t lock;
static pthread_mutex_t ev_lock;
static pthread_cond_t ev_cond;

void
tev__mutex_init()
{
    pthread_mutex_init(&lock, NULL);
}

void
tev__mutex_lock()
{
    pthread_mutex_lock(&lock);
}

void
tev__mutex_unlock()
{
    pthread_mutex_unlock(&lock);
}

void
tev__event_init()
{
    pthread_mutex_init(&ev_lock, NULL);
    pthread_cond_init(&ev_cond, NULL);
}

int
tev__event_wait(uint64_t timeout)
{
    struct timeval now;
    struct timeval wait;
    struct timeval to_wait;
    struct timespec to_wait_r;
	int res = 0;

    pthread_mutex_lock(&ev_lock);

    if (timeout == -1LL) {
        pthread_cond_wait(&ev_cond, &ev_lock);
    }
	else {
		gettimeofday(&now, NULL);
        wait.tv_sec = timeout / 1000;
        wait.tv_usec = (timeout % 1000) * 1000;
        timeradd(&now, &wait, &to_wait);
        to_wait_r.tv_sec = to_wait.tv_sec;
        to_wait_r.tv_nsec = to_wait.tv_usec * 1000;
		res = pthread_cond_timedwait(&ev_cond, &ev_lock, &to_wait_r);
	}

    pthread_mutex_unlock(&ev_lock);

    return res;
}

void
tev__event_set()
{
    pthread_mutex_lock(&ev_lock);
    pthread_cond_signal(&ev_cond);
    pthread_mutex_unlock(&ev_lock);
}

void
tev__event_set_from_isr()
{

}
#endif
