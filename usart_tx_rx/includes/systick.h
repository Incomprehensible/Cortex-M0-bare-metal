#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void systick_init(uint32_t);
void systick_clock_on();
void systick_enable();
void systick_disable();

#endif