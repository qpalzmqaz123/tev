#include "tev.h"
#include <pthread.h>
#include <sys/time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#if TEV_CONF_ENABLE_OS == 1

static xSemaphoreHandle xQueueMutex;
static xSemaphoreHandle xSemaTxDone;

void
tev__mutex_init()
{
    /* Create Mutex lock */
    xQueueMutex = xSemaphoreCreateMutex();
    configASSERT (xQueueMutex != NULL);
}

void
tev__mutex_lock()
{
    xSemaphoreTake( xQueueMutex, portMAX_DELAY );
}

void
tev__mutex_unlock()
{
    xSemaphoreGive( xQueueMutex );
}

void
tev__event_init()
{
    /* Queue Semaphore */ 
    vSemaphoreCreateBinary( xSemaTxDone );
    configASSERT ( xSemaTxDone != NULL );
}

int
tev__event_wait(uint64_t timeout)
{
	int res = 0;

    xSemaphoreTake(xSemaTxDone, timeout);

    return res;
}

void
tev__event_set()
{
    xSemaphoreGive( xSemaTxDone );
}

void
tev__event_set_from_isr()
{
    xSemaphoreGiveFromISR( xSemaTxDone, NULL );
}
#endif
