#include "tev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

uint64_t
tev__get_time(void)
{
    return (uint64_t)xTaskGetTickCount();
}
