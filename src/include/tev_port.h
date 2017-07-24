#ifndef __TEV_PORT_H__
#define __TEV_PORT_H__

uint64_t
tev__get_time(void);

#if TEV_CONF_ENABLE_OS == 1
void
tev__mutex_init();

void
tev__mutex_lock();

void
tev__mutex_unlock();

void
tev__event_init();

int
tev__event_wait(uint64_t timeout);

void
tev__event_set();

void
tev__event_set_from_isr();
#endif

#endif
