#ifndef FONT_H
#define FONT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "font-data.h" // Generated by meta/font.c. Declares Glyph struct and font (array of glyphs)

typedef struct {
    uint32_t *data;
    size_t width, height, stride;
} Canvas;

#define CANVAS_AT(canvas, x, y) (canvas).data[(y) * (canvas).stride + (x)]

void plot(Canvas canvas, size_t x, size_t y, uint32_t color);
void draw_rect(Canvas canvas, size_t x, size_t y, size_t w, size_t h, uint32_t color);
size_t draw_text(Canvas canvas, size_t x, size_t y, size_t scale, const char *text, size_t len);

#endif // FONT_H
