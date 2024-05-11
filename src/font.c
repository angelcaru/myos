#define DECLARE_FONT
#include "font.h"

void plot(Canvas canvas, size_t x, size_t y, uint32_t color) {
    if (x < canvas.width && y < canvas.height) {
        CANVAS_AT(canvas, x, y) = color;
    }
}

void draw_rect(Canvas canvas, size_t x0, size_t y0, size_t w, size_t h, uint32_t color) {
    for (size_t dx = 0; dx < w; dx++) {
        size_t x = x0 + dx;
        for (size_t dy = 0; dy < h; dy++) {
            size_t y = y0 + dy;
            plot(canvas, x, y, color);
        }
    }
}

size_t draw_text(Canvas canvas, size_t x0, size_t y, size_t scale, const char *text, size_t len) {
    size_t x = x0;

    for (size_t i = 0; i < len; i++) {
        char ch = text[i];
        Glyph glyph = font[(size_t)ch];

        for (size_t row = 0; row < GLYPH_HEIGHT; row++) {
            for (size_t col = 0; col < GLYPH_WIDTH; col++) {
                uint32_t color = glyph.img[row][col] ? 0xFFFFFFFF : 0x00000000;
                draw_rect(canvas, x + col * scale, y + row * scale, scale, scale, color);
            }
        }
        x += scale * GLYPH_WIDTH;
    }
    return x;
}

