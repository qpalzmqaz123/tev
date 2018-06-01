#include <stdlib.h>
#include "tev.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tev_port.h"

#if TEV_CONF_ENABLE_OS == 1

void
tev__mutex_init(void **pdata)
{
    TEV_ASSERT_NOT_NULL(*pdata = xSemaphoreCreateMutex());
}

void
tev__mutex_lock(void *data)
{
    xSemaphoreTake((xSemaphoreHandle)data, portMAX_DELAY);
}

void
tev__mutex_unlock(void *data)
{
    xSemaphoreGive((xSemaphoreHandle)data);
}

void
tev__event_init(void **pdata)
{
    vSemaphoreCreateBinary(*pdata);
    TEV_ASSERT_NOT_NULL(*pdata);
}

int
tev__event_wait(void *data, uint64_t timeout)
{
    int res = 0;

    xSemaphoreTake(data, timeout * configTICK_RATE_HZ / 1000);

    return res;
}

void
tev__event_set(void *data)
{
    xSemaphoreGive(data);
}

#endif
