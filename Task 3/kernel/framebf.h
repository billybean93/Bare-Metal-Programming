// ----------------------------------- framebf.h -------------------------------------
void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawLCircle(int center_x, int center_y, int radius, unsigned int attr, int fill);
void drawBitmap(int x0, int y0, int width, int height, const uint32_t *bitmap) ;
// ----------------------------------- framebf.h -------------------------------------

