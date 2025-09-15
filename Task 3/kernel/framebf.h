// ----------------------------------- framebf.h -------------------------------------
typedef unsigned int uint32_t;
int drawGameStart(int *bufferIndex, char c, int fcount, int level);
void drawFinishedLv(int bufferIndex);
void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawLCircle(int center_x, int center_y, int radius, unsigned int attr, int fill);
void drawBitmap(int x0, int y0, int width, int height, const uint32_t *bitmap) ;
void drawBitmapFlipped(int x0, int y0, int width, int height, const uint32_t *bitmap);
void swapBuffer(int bufferIndex);
void changeMapColor() ;
void drawIndexedSpriteRot(int x, int y, int w, int h,
                          const unsigned char* data,
                          const unsigned int* palette,
                          int scale,
                          int angle_idx);
void drawARGBSpriteRot(int x, int y, int w, int h,
                       const unsigned int* data, int scale, int angle_idx);