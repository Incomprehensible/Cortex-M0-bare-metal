#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_clock_on();
void gpio_configue();
void gpio_write(uint32_t);
void gpio_toggle();


#endif