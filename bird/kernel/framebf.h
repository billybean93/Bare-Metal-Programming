// ----------------------------------- framebf.h -------------------------------------
void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawLine(int x1, int y1, int x2, int y2, unsigned int attr);
void drawLCircle(int center_x, int center_y, int radius, unsigned int attr, int fill);

unsigned int framebf_width();
unsigned int framebf_height();
void clearScreen(unsigned int attr);
void drawLCircle(int cx, int cy, int r, unsigned int attr, int fill);
void drawIndexedSprite(int x, int y, int w, int h,
                       const unsigned char* data,
                       const unsigned int* palette,
                       int scale);

// Vẽ sprite indexed (0 = transparent)
void drawIndexedSprite(int x, int y, int w, int h,
                       const unsigned char* data,
                       const unsigned int* palette,
                       int scale);

// Vẽ sprite indexed có XOAY: angle_idx ∈ {0..4} ứng với [-25,-10,0,30,70] độ
void drawIndexedSpriteRot(int x, int y, int w, int h,
                          const unsigned char* data,
                          const unsigned int* palette,
                          int scale,
                          int angle_idx);