#ifndef IRQ
#define IRQ

#include <stdint.h>

void syscfg_clock_on();
void syscfg_config();
void syscfg_gpio_config(uint32_t, uint32_t);
void exti_config();
void nvic_config();

#endif