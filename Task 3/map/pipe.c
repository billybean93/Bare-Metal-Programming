#include "map.h"
#include "../kernel/framebf.h"
#include "map.h"
#include "../kernel/framebf.h"

void drawPipe(Pipe *p, uint32_t color) {
    // Top pipe (from top of screen down to gap_y)
    drawRectARGB32(
        p->x,                 // x1
        0,                    // y1 (top of screen)
        p->x + PIPE_WIDTH,    // x2
        p->gap_y,             // y2
        color,
        1                     // fill
    );

    // Bottom pipe (from gap_y + GAP_HEIGHT to bottom of screen)
    drawRectARGB32(
        p->x,                        // x1
        p->gap_y + GAP_HEIGHT,       // y1
        p->x + PIPE_WIDTH,           // x2
        BACKGROUND_IMAGE_HEIGHT - GROUND_HEIGHT,               // y2 (bottom of screen)
        color,
        1                            // fill
    );
}