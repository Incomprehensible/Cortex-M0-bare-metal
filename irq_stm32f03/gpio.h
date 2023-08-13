#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_clock_on();
void gpio_configue();
void gpio_write(uint32_t);
void gpio_toggle();
uint32_t gpio_read();
uint32_t get_gpioa_status();
uint32_t get_gpioc_status();


#endif