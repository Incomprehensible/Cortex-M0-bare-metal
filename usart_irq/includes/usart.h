#ifndef USART_H
#define USART_H

#include <stdint.h>

#define UART1_TX 9 // PA9
#define UART1_RX 10 // PA10

void usart_init(uint32_t, uint32_t);
void usart_enable();
void usart_clock_on();
void usart_transmit(const uint8_t*, uint32_t);
void usart_receive(uint8_t*, uint32_t);

#endif