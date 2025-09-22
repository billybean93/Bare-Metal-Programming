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
void drawBgAndNames();
void display_video(int x, int y, int width, int height, const uint32_t **frames, int num_frames, int delay_count);
void clearScreen(unsigned int attr);
void swapBuffer(int bufferIndex);
void changeMapColor() ;
void drawIndexedSpriteRot(int x, int y, int w, int h,
                          const unsigned char* data,
                          const unsigned int* palette,
                          int scale,
                          int angle_idx);
void drawARGBSpriteRot(int x, int y, int w, int h,
                       const unsigned int* data, int scale, int angle_idx);
void drawString(int x, int y, char *str, unsigned int attr, int zoom);
void drawChar(unsigned char ch, int x, int y, unsigned int attr, int zoom);
void intToStr(int num, char* str);
