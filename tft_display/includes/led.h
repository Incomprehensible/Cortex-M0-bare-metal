#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpioa_clock_on();
void led_gpio_configue();
void led_write(uint32_t);
void led_toggle();

#endif