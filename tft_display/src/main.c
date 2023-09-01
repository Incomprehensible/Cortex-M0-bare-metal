#include "stm32f030x6.h"
#include "main.h"
#include "led.h"
#include "systick.h"
// #include "usart.h"
#include "ili9163.h"
#include "handjet.c"

volatile uint8_t toggle = 0;

extern volatile uint32_t s_ticks;

volatile void __attribute__((optimize("O0"))) dummy_wait()
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

void simple_test_loop(uint8_t rows, uint8_t cols)
{
    uint16_t red = 0xF800;
    uint16_t blue = 0x001F;
    uint8_t flag = 0;
    uint16_t color = 0;
    while (true)
    {
        color = (flag)? blue : TEST_COLOR;
        flag = !flag;
        for (int i = 0; i < rows * cols; ++i)
        {
            spi_write_2bytes(0, TFT_DATA);
        }
        delay_ms(10);
    }
}

void simple_animation(uint8_t rows, uint8_t cols)
{
    for (uint8_t square_dim=10; square_dim < rows; square_dim+=2)
    {
        for (int i=0; i<(rows-square_dim-2)/2; ++i)
        {
            for (int j=0; j<cols; ++j)
                spi_write_2bytes(0x0, TFT_DATA);
        }
        for (int j=0; j<(cols-square_dim)/2; ++j)
            spi_write_2bytes(0x0, TFT_DATA);
        for (int j=0; j<square_dim; ++j)
            spi_write_2bytes(TEST_COLOR, TFT_DATA);
        for (int j=0; j<(cols-square_dim)/2; ++j)
            spi_write_2bytes(0x0, TFT_DATA);
        for (int i=0; i<square_dim; ++i)
        {
            for (int j=0; j<cols; ++j) {
                if (j == (cols-square_dim)/2 || j == (cols-square_dim)/2+square_dim)
                    spi_write_2bytes(TEST_COLOR, TFT_DATA);
                else
                    spi_write_2bytes(0x0, TFT_DATA);
            }
        }
        for (int j=0; j<(cols-square_dim)/2; ++j)
            spi_write_2bytes(0x0, TFT_DATA);
        for (int j=0; j<square_dim; ++j)
            spi_write_2bytes(TEST_COLOR, TFT_DATA);
        for (int j=0; j<(cols-square_dim)/2; ++j)
            spi_write_2bytes(0x0, TFT_DATA);
        for (int i=0; i<(rows-square_dim-2)/2; ++i)
        {
            for (int j=0; j<cols; ++j)
                spi_write_2bytes(0x0, TFT_DATA);
        }
    }
}

void line_test(uint8_t rows, uint8_t cols)
{
    for (int i=0; i<rows; ++i)
    {
        for (int j=0; j<cols; ++j)
        {
            if (j >= 50 && j <= 56)
                spi_write_2bytes(0xFFFF, TFT_DATA);
            else
                spi_write_2bytes(0x0, TFT_DATA);
        }
    }
}

void write_char(uint8_t rows, uint8_t cols, const font_t *font, uint8_t s)
{
    const bitmap_t* bitmap = font->bitmap;
    const glyph_t* glyph = &font->glyphs[s];
    const uint8_t *payload = bitmap->payload + (bitmap->size_x/8)*glyph->x_offset;

    for (int i=0; i<glyph->width; ++i)
    {
        for (int j=0; j<bitmap->size_x/8; ++j)
        {
            uint8_t byte = *payload;
            for (int b=0; b<8; ++b)
            {
                if (byte & (1 << (8-b)))
                    spi_write_2bytes(0xFFFF, TFT_DATA);
                else
                    spi_write_2bytes(0x0, TFT_DATA);
            }
            payload++;
        }
        for (int i=0; i<(cols)-(bitmap->size_x); ++i)
        {
            spi_write_2bytes(0xFFFF, TFT_DATA);
        }
    }
    for (int i=0; i<(rows*cols)-(glyph->width*cols); ++i)
    {
        spi_write_2bytes(0xFFFF, TFT_DATA);
    }
}

void write_char_somewhere(uint8_t rows, uint8_t cols, const font_t *font, uint8_t s)
{
    const bitmap_t* bitmap = font->bitmap;
    const glyph_t* glyph = &font->glyphs[s];
    const uint8_t *payload = bitmap->payload + (bitmap->size_x/8)*glyph->x_offset;

    for (int i=0; i<((rows*cols)-(glyph->width*cols))/2; ++i)
    {
        spi_write_2bytes(0xFFFF, TFT_DATA);
    }
    for (int i=0; i<glyph->width; ++i)
    {
        for (int j=0; j<bitmap->size_x/8; ++j)
        {
            uint8_t byte = *payload;
            for (int b=7; b>=0; --b)
            {
                if (byte & (1 << (b)))
                    spi_write_2bytes(0xFFFF, TFT_DATA);
                else
                    spi_write_2bytes(0x0, TFT_DATA);
            }
            payload++;
        }
        for (int i=0; i<(cols)-(bitmap->size_x); ++i)
        {
            spi_write_2bytes(0xFFFF, TFT_DATA);
        }
    }
    for (int i=0; i<((rows*cols)-(glyph->width*cols))/2; ++i)
    {
        spi_write_2bytes(0xFFFF, TFT_DATA);
    }
}

void write_all_chars(uint8_t rows, uint8_t cols, const font_t *font)
{
    const bitmap_t* bitmap = font->bitmap;
    const glyph_t* glyph;
    const uint8_t *payload;
    uint16_t y_offset = 0;

    for (int g=1; g<font->glyph_count-15; ++g)
    {
        glyph = &font->glyphs[g];
        payload = bitmap->payload + (bitmap->size_x/8)*glyph->x_offset;

        for (int i=0; i<glyph->width; ++i)
        {
            for (int j=0; j<bitmap->size_x/8; ++j)
            {
                uint8_t byte = *payload;
                for (int b=7; b>=0; --b)
                {
                    if (byte & (1 << b))
                        spi_write_2bytes(~0xFFFF, TFT_DATA);
                    else
                        spi_write_2bytes(~0x0, TFT_DATA);
                }
                payload++;
            }
            for (int j=0; j<cols-bitmap->size_x; ++j)
            {
                spi_write_2bytes(~0xFFFF, TFT_DATA);
            }
        }
        y_offset += glyph->width;
    }
    for (int i=0; i<((rows*cols)-(y_offset*cols)); ++i)
    {
        spi_write_2bytes(~0xFFFF, TFT_DATA);
    }
}

int main()
{
    const uint32_t sysclock = 8000000;
    gpioa_clock_on();
    led_gpio_configue();

    // TODO: bump sysclock up to 48MHz

    // usart_clock_on();
    // usart_init(sysclock, 9600);
    // usart_tx_enable();

// To generate timer every 500 ms:
// 8 Million cycles per sec -> 4 million cycles per half a second
// 8*10**6/1000 -> cycles per ms
    systick_init(sysclock/1000);
    systick_enable();

    spi_clock_on();
    test_tft_gpio_configure();
    tft_gpio_init();
    tft_reset();
    test_spi_configure();
    spi_enable();
    // tft_reset();
    // GPIOB->ODR &= ~(1 << SPI1_CS);
    tft_start();
    // ili9341_hspi_init();
    tft_set_dims(127, 127);
    tft_en_mem_transfer();

    for (int i=0; i<10; ++i) {
        led_toggle();
        dummy_wait();
    }

    led_write(0);

    const uint8_t rows = 128;
    const uint8_t cols = 128;

    while (true) {
        // for (int i=0; i<2000; ++i)
        // {
        //     uint8_t byte = FONT_generalemono_a_65.bitmap->payload[i];
        //     for (int b=0; b<8; ++b)
        //     {
        //         if (byte & (1 << b))
        //             spi_write_2bytes(0xFFFF, TFT_DATA);
        //         else
        //             spi_write_2bytes(0x0, TFT_DATA);
        //     }
        // }
        // for (int i=0; i<rows*cols-2000*8; ++i)
        //     spi_write_2bytes(0xFFFF, TFT_DATA);
        // write_char(rows, cols, &FONT_generalemono_a_65, 1);
        // write_char_somewhere(rows, cols, &FONT_handjet_regular_14, 14);
        write_all_chars(rows, cols, &FONT_handjet_regular_14);
    }

    return 0;
}