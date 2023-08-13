#include "stm32f030x6.h"
#include "main.h"
#include "gpio.h"
#include "systick.h"
#include "usart.h"

volatile uint8_t toggle = 0;

extern volatile uint32_t s_ticks;

volatile void dummy_wait()
{
    int dummy = 0;
    for (int i=0; i<100000; ++i)
    {
        dummy++;
    }
}

void delay_ms(uint32_t ms)
{
    uint32_t ms_to_wait = s_ticks + ms; 
    while (s_ticks < ms_to_wait) ;
}

int main()
{
    const uint32_t sysclock = 8000000;
    gpio_clock_on();
    gpio_configue();

    usart_clock_on();
    usart_init(sysclock, 9600);
    usart_tx_enable();

// To generate timer every 500 ms:
// 8 Million cycles per sec -> 4 million cycles per half a second
// 8*10**6/1000 -> cycles per ms
    systick_init(sysclock/1000);
    systick_enable();


    for (int i=0; i<10; ++i) {
        led_toggle();
        dummy_wait();
    }

    led_write(0);

    const uint8_t str[] = "Hello Nad";
    while (1)
    {
        led_write(1);
        usart_transmit(str, sizeof(str));
        led_write(0);
        delay_ms(1000); // 1s delay
    }

    return 0;
}