#include "stm32f030x6.h"
#include "main.h"

void gpio_clock_on()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN ; // enable GPIO (Ports A,B,F) clock & power
}

void gpio_configue()
{
    GPIOA->MODER &= ~(0b11 << (2*LED));
    GPIOA->MODER |= 0b01 << (2*LED); // output mode

    GPIOA->OTYPER &= ~(0b1 << LED); // push-pull mode

    GPIOA->OSPEEDR |= 0b11 << (2*LED); // high-speed mode

    GPIOA->PUPDR &= ~(0b11 << (2*LED)); // no pull-up or pull-down
}

void led_write(uint32_t level)
{
    if (level == 0)
        GPIOA->ODR |= 0b1 << LED;
    else {
        GPIOA->ODR &= ~(0b1 << LED);
    }
}

void led_toggle()
{
    GPIOA->ODR ^= 0b1 << LED;
}