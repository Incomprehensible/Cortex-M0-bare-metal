#ifndef GRAPHICS_H
#define GRAPHICS_H

// #include <stdint.h>

// uint8_t everywhere to save small space

typedef struct 
{
    uint16_t size_x;
    uint16_t size_y;
    const uint8_t *payload;
} bitmap_t;

typedef struct 
{
    uint8_t character;
    uint16_t width;
    uint16_t x_offset;
} glyph_t;

typedef struct
{
    const bitmap_t* bitmap;
    const uint8_t glyph_count;
    const glyph_t* glyphs;

} font_t;

#endif