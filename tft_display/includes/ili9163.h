#ifndef ILI9163_H
#define ILI9163_H

#include "main.h"

// RGB
#define TEST_COLOR ((uint16_t)(((uint8_t)0b00000) << 11 | ((uint8_t)0b111111) << 5 | (uint8_t)0b0000))

typedef enum TFT_Packet_type {
    TFT_DATA,
    TFT_CMD,
} TFT_Packet;

void tft_gpio_configure();
void tft_gpio_init();
void tft_reset();
void tft_start();
void tft_en_mem_transfer();
void tft_set_dims(uint16_t, uint16_t);
void spi_clock_on();
void spi_reset(bool en);
void spi_configure();
void spi_enable();
void spi_disable();
void spi_write_byte(uint8_t, TFT_Packet);
void spi_write_2bytes(uint16_t, TFT_Packet);
void spi_slow_down();

void ili9341_hspi_init();
void test_tft_gpio_configure();
void test_spi_configure();

#endif