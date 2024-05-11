#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

#include "font.h"
 
#include "limine-junk.h"

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
void _start(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }
 
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
 
    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // TODO: check for format (and maybe support multiple formats?)
    Canvas canvas = {
        .data = framebuffer->address,
        .width = framebuffer->width,
        .height = framebuffer->height,
        .stride = framebuffer->pitch / 4,
    };

#define LINE_CAP 1024
    static char line[LINE_CAP] = {0};
    static size_t line_sz = 0;

#define TIME_FACTOR 1000

    line[line_sz++] = '>';
    line[line_sz++] = ' ';

    int i = 0;
    while (1) {
        size_t scale = 2;
        size_t x = draw_text(canvas, 0, 0, scale, line, line_sz);
        uint32_t color = i < TIME_FACTOR ? 0xFFFFFFFF : 0x00000000;
        draw_rect(canvas, x, 0, GLYPH_WIDTH * scale, GLYPH_HEIGHT * scale, color);
        i++;

        if (i >= TIME_FACTOR) i = 0;
    }
}

