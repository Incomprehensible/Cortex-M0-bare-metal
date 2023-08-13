#include "main.h"

void gpio_clock_on()
{
    RCC->AHBENR |= (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN) ; // enable GPIO (Ports A,C) clock & power
}

void gpio_configue()
{
    GPIOA->MODER &= ~(0b11 << (2*BUTTON)); // input mode

    GPIOC->MODER &= 0b00 << (2*LED_G);
    GPIOC->MODER |= 0b01 << (2*LED_G); // output mode

    GPIOC->OTYPER &= ~(0b1 << LED_G); // push-pull mode

    GPIOC->OSPEEDR |= 0b11 << (2*LED_G); // high-speed mode

    GPIOC->PUPDR &= ~(0b11 << (2*LED_G)); // no pull-up or pull-down
}

void gpio_write(uint32_t level)
{
    if (level == 1)
        GPIOC->ODR |= 0b1 << LED_G;
    else {
        GPIOC->ODR &= ~(0b1 << LED_G);
    }
}

void gpio_toggle()
{
    GPIOC->ODR ^= 0b1 << LED_G;
}

uint32_t gpio_read()
{
    return GPIOA->IDR & (0b1 << BUTTON);
}

uint32_t get_gpioa_status()
{
    return GPIOA->IDR;
}

uint32_t get_gpioc_status()
{
    return GPIOC->IDR;
}
