#include "stm32f030x6.h"
#include "main.h"

void gpio_clock_on()
{
    RCC->AHBENR |= (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOFEN) ; // enable GPIO (Ports A,B,F) clock & power
}

void gpio_configue()
{
    GPIOA->MODER &= ~(0b11 << (2*BUTTON)); // input mode

    GPIOA->MODER &= ~(0b11 << (2*LED));
    GPIOA->MODER |= 0b01 << (2*LED); // output mode

    GPIOB->MODER &= ~(0b11 << (2*ROTARY_A)); // input mode
    GPIOF->MODER &= ~(0b11 << (2*ROTARY_B));

    GPIOA->OTYPER &= ~(0b1 << LED); // push-pull mode

    GPIOA->OSPEEDR |= 0b11 << (2*LED); // high-speed mode

    GPIOA->PUPDR &= ~(0b11 << (2*LED)); // no pull-up or pull-down
    GPIOA->PUPDR &= ~(0b11 << (2*BUTTON));
    GPIOB->PUPDR &= ~(0b11 << (2*ROTARY_A));
    GPIOF->PUPDR &= ~(0b11 << (2*ROTARY_B));
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

uint32_t button_read()
{
    return GPIOA->IDR & (0b1 << BUTTON);
}

uint32_t rotary_a_read()
{
    return GPIOB->IDR & (0b1 << ROTARY_A);
}

uint32_t rotary_b_read()
{
    return GPIOF->IDR & (0b1 << ROTARY_B);
}

uint16_t rotary_read()
{
    return (((GPIOB->IDR & (0b1 << ROTARY_A)) << 8) | (GPIOF->IDR & (0b1 << ROTARY_B)));
}
