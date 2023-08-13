#include "usart.h"
#include "main.h"

static uint8_t init = 0;

volatile void spin(uint32_t ticks)
{
    int dummy = 0;
    for (int i=0; i<ticks; ++i)
    {
        dummy++;
    }
}

void usart_init(uint32_t fclk, uint32_t baud)
{
    // USART1 peripheral
    // PA9 and PA10 pins for TX and RX
    // Set gpio mode as alternative (10)
    GPIOA->MODER &= ~(0b11 << (UART1_TX * 2) | 0b11 << (UART1_RX * 2));
    GPIOA->MODER |= (0b10 << (UART1_TX * 2) | 0b10 << (UART1_RX * 2));
    GPIOA->OTYPER &= ~(0b1 << UART1_TX); // push-pull mode
    // Alternative function AF1: 0001
    // GPIOA alternate function high register (GPIOx_AFRH)
    GPIOA->AFR[1] &= ~(0b1111 << ((UART1_RX & 0b111)*4));
    GPIOA->AFR[1] |= (0b1 << ((UART1_RX & 0b111)*4));
    GPIOA->AFR[1] &= ~(0b1111 << ((UART1_TX & 0b111)*4));
    GPIOA->AFR[1] |= (0b1 << ((UART1_TX & 0b111)*4));
    // Oversampling by 16
    uint32_t div = fclk / baud;
    USART1->BRR = div;
    USART1->CR1 = 0;
    init = 1;
}

void usart_tx_enable()
{
    if (!init)
        return;
    USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
}

void usart_clock_on()
{
    //RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // enable gpio pins
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
}

void usart_transmit(const uint8_t* data, uint32_t len)
{
    while (len--)
    {
        USART1->TDR = *data++;
        // This bit is set by hardware if the transmission of a frame containing data is complete and if
        // TXE is set. It is cleared by software, writing 1 to the TCCF in the USART_ICR register 
        // or by a write to the USART_TDR
        while (!(USART1->ISR & USART_ISR_TC)) spin(1);
    }
    // Clear the TC flag in the USART_ISR register
    USART1->ICR |= USART_ICR_TCCF;
}

void usart_receive(uint8_t* data, uint32_t len)
{
    while (len--)
    {
        // The RXNE bit is set to indicate that the content of the shift register is transferred to the RDR. 
        // In other words, data has been received and can be read.
        while (!(USART1->ISR & USART_ISR_RXNE)) spin(1);
        // Clearing the RXNE bit is performed by a software read to the USART_RDR register. 
        // The RXNE flag can also be cleared by writing 1 to the RXFRQ in the USART_RQR register.
        *data++ = (uint8_t) USART1->RDR;
    }
    // Clear the RXNE flag in the USART_RQR register
    //USART1->RQR |= USART_RQR_RXFRQ;
}

