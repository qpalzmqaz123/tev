#include "tev.h"
#include "vdev.h"

#if TEV_CONF_ENABLE_OS == 1

static vdev_os_mutex_t mutex;
static vdev_os_event_t event;

static vdev_os_api_t *
tev__require_os()
{
    static vdev_os_api_t *os = NULL;

    return NULL == os ? (os = vdev_api_query("os")) : os;
}

void
tev__mutex_init()
{
    vdev_os_api_t *os = tev__require_os();

    os->mutex_create(&mutex);
}

void
tev__mutex_lock()
{
    vdev_os_api_t *os = tev__require_os();

    os->mutex_lock(mutex);
}

void
tev__mutex_unlock()
{
    vdev_os_api_t *os = tev__require_os();

    os->mutex_unlock(mutex);
}

void
tev__event_init()
{
    vdev_os_api_t *os = tev__require_os();

    os->event_create(&event);
}

int
tev__event_wait(uint64_t timeout)
{
    vdev_os_api_t *os = tev__require_os();

    return os->event_wait_timeout(event, timeout);
}

void
tev__event_set()
{
    vdev_os_api_t *os = tev__require_os();

    os->event_set(event);
}

uint64_t
tev__get_time(void)
{
    vdev_os_api_t *os = tev__require_os();

    return (uint64_t)os->get_time();
}
#endif
