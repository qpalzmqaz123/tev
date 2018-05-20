#include "tev.h"
#include "FreeRTOS.h"
#include "task.h"

uint64_t
tev__get_time(void)
{
    return (uint64_t)xTaskGetTickCount() * 1000 / configTICK_RATE_HZ;
}
