#ifndef __TEV_PORT_H__
#define __TEV_PORT_H__

uint64_t
tev__get_time(void);

#if TEV_CONF_ENABLE_OS == 1
void
tev__mutex_init(void **pdata);

void
tev__mutex_lock(void *data);

void
tev__mutex_unlock(void *data);

void
tev__event_init(void **pdata);

int
tev__event_wait(void *data, uint64_t timeout);

void
tev__event_set(void *data);

void
tev__event_set_from_isr();
#endif

#endif
