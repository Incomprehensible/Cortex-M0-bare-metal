#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_clock_on();
void timer_clock_off();
void timer_configure(uint16_t);
void timer_enable();
void timer_stop();

#endif
