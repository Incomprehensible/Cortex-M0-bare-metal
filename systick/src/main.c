#include "stm32f030x6.h"
#include "main.h"
#include "gpio.h"
#include "irq.h"
#include "systick.h"

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
    gpio_clock_on();
    gpio_configue();

// To generate timer every 500 ms:
// 8 Million cycles per sec -> 4 million cycles per half a second
// 8*10**6/1000 -> cycles per ms
    systick_init(8000000/1000);
    systick_enable();


    for (int i=0; i<10; ++i) {
        led_toggle();
        dummy_wait();
    }

    led_write(0);
    while (1)
    {
        led_toggle();
        delay_ms(5000);
        // else if (toggle)
        // {
        //     led_toggle();
        //     toggle = 0;
        // }
        // dummy_wait();
    }

    return 0;
}