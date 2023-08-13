#include "main.h"

void gpio_clock_on()
{
    RCC->AHBENR |= (RCC_AHBENR_GPIOAEN) ; // enable GPIO (Port A) clock & power
}

void gpio_configue()
{
    GPIOA->MODER &= ~(0b11 << (2*BUTTON)); // input mode

    GPIOA->MODER &= ~(0b11 << (2*LED));
    GPIOA->MODER |= 0b01 << (2*LED); // output mode

    GPIOA->OTYPER &= ~(0b1 << LED); // push-pull mode

    GPIOA->OSPEEDR |= 0b11 << (2*LED); // high-speed mode

    GPIOA->PUPDR &= ~(0b11 << (2*LED)); // no pull-up or pull-down
    GPIOA->PUPDR &= ~(0b11 << (2*BUTTON));
}

void gpio_write(uint32_t level)
{
    if (level == 0)
        GPIOA->ODR |= 0b1 << LED;
    else {
        GPIOA->ODR &= ~(0b1 << LED);
    }
}

void gpio_toggle()
{
    GPIOA->ODR ^= 0b1 << LED;
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
