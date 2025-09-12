// ----------------------------------- framebf.c -------------------------------------
#include "mbox.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "../map/map.h"
#include "../font/font.h"

//Use RGBA32 (32 bits for each pixel)
#define COLOR_DEPTH 32

//Pixel Order: BGR in memory order (little endian --> RGB in byte order)
#define PIXEL_ORDER 0

//Screen info
unsigned int width, height, pitch;

/* Frame buffer address
 * (declare as pointer of unsigned char to access each byte) */
unsigned char *fb;


/**
 * Set screen resolution to 1024x768
 */
void framebf_init()
{
    mBuf[0] = 35*4; // Length of message in bytes
    mBuf[1] = MBOX_REQUEST;

    mBuf[2] = MBOX_TAG_SETPHYWH; //Set physical width-height
    mBuf[3] = 8; // Value size in bytes
    mBuf[4] = 0; // REQUEST CODE = 0
    mBuf[5] = BACKGROUND_IMAGE_WIDTH; // Value(width)
    mBuf[6] = BACKGROUND_IMAGE_HEIGHT; // Value(height)

    mBuf[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height
    mBuf[8] = 8;
    mBuf[9] = 0;
    mBuf[10] = BACKGROUND_IMAGE_WIDTH * 1000; // set a super long virtual width to accomodate multiple pipes
    mBuf[11] = BACKGROUND_IMAGE_HEIGHT * 2;

    mBuf[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
    mBuf[13] = 8;
    mBuf[14] = 0;
    mBuf[15] = 0; // x offset
    mBuf[16] = 0; // y offset

    mBuf[17] = MBOX_TAG_SETDEPTH; //Set color depth
    mBuf[18] = 4;
    mBuf[19] = 0;
    mBuf[20] = COLOR_DEPTH; //Bits per pixel

    mBuf[21] = MBOX_TAG_SETPXLORDR; //Set pixel order
    mBuf[22] = 4;
    mBuf[23] = 0;
    mBuf[24] = PIXEL_ORDER;

    mBuf[25] = MBOX_TAG_GETFB; //Get frame buffer
    mBuf[26] = 8;
    mBuf[27] = 0;
    mBuf[28] = 16; //alignment in 16 bytes
    mBuf[29] = 0;  //will return Frame Buffer size in bytes

    mBuf[30] = MBOX_TAG_GETPITCH; //Get pitch
    mBuf[31] = 4;
    mBuf[32] = 0;
    mBuf[33] = 0; //Will get pitch value here

    mBuf[34] = MBOX_TAG_LAST;

    // Call Mailbox
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) //mailbox call is successful ?
    	&& mBuf[20] == COLOR_DEPTH //got correct color depth ?
		&& mBuf[24] == PIXEL_ORDER //got correct pixel order ?
		&& mBuf[28] != 0 //got a valid address for frame buffer ?
		) {

    	/* Convert GPU address to ARM address (clear higher address bits)
    	 * Frame Buffer is located in RAM memory, which VideoCore MMU
    	 * maps it to bus address space starting at 0xC0000000.
    	 * Software accessing RAM directly use physical addresses
    	 * (based at 0x00000000)
    	*/
    	mBuf[28] &= 0x3FFFFFFF;

        // Access frame buffer as 1 byte per each address
        fb = (unsigned char *)((unsigned long)mBuf[28]);
        uart_puts("Got allocated Frame Buffer at RAM physical address: ");
        uart_hex(mBuf[28]);
        uart_puts("\n");

        uart_puts("Frame Buffer Size (bytes): ");
        uart_dec(mBuf[29]);
        uart_puts("\n");

        width = mBuf[5];     	// Actual physical width
        height = mBuf[6];     	// Actual physical height
        pitch = mBuf[33];       // Number of bytes per line

    } else {
    	uart_puts("Unable to get a frame buffer with provided setting\n");
    }
}


void drawPixelARGB32(int x, int y, unsigned int attr)
{
	int offs = (y * pitch) + (COLOR_DEPTH/8 * x);

/*	//Access and assign each byte
    *(fb + offs    ) = (attr >> 0 ) & 0xFF; //BLUE  (get the least significant byte)
    *(fb + offs + 1) = (attr >> 8 ) & 0xFF; //GREEN
    *(fb + offs + 2) = (attr >> 16) & 0xFF; //RED
    *(fb + offs + 3) = (attr >> 24) & 0xFF; //ALPHA
*/

	//Access 32-bit together
	*((unsigned int*)(fb + offs)) = attr;
}


void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill)
{
	for (int y = y1; y <= y2; y++ )
		for (int x = x1; x <= x2; x++) {
			if ((x == x1 || x == x2) || (y == y1 || y == y2))
				drawPixelARGB32(x, y, attr);
			else if (fill)
				drawPixelARGB32(x, y, attr);
		}
}




// Function to draw line
void drawLine(int x1, int y1, int x2, int y2, unsigned int attr)
{
	for (int x = x1; x <= x2; x++ ){
        int y = (float)(y1 - y2)/(x1 - x2)*(x - x1) + y1;
        drawPixelARGB32(x, y, attr);
    }
}


// Function to calculate the square root of a number using the Newton-Raphson method
double sqrt(double number) {
    if (number < 0) {
        return -1; // Return -1 for negative inputs as square root of negative is not defined in real numbers
    }
    
    double tolerance = 0.000001; // Define the tolerance for the result
    double guess = number / 2.0; // Initial guess (can be any positive number, here half of the number)
    double result = 0.0;
    
    while (1) {
        result = 0.5 * (guess + number / guess); // Calculate the next approximation
        
        // Check if the difference between the current guess and the new result is within the tolerance
        int diff = (result > guess) ? (result - guess) : (guess - result);
        if (diff < tolerance) {
            break;
        }
        
        guess = result; // Update the guess for the next iteration
    }
    
    return result;
}


// Function to draw circle
void drawLCircle(int center_x, int center_y, int radius, unsigned int attr, int fill)
{
    //Draw the circle when going on x side
    for (int x = center_x - radius; x <= center_x + radius; x++) {
        // Calculate the corresponding y values using the circle equation
        int dy = sqrt(radius * radius - (x - center_x) * (x - center_x)); 
        int upper_y = center_y + dy;
        int lower_y = center_y - dy;

        drawPixelARGB32(x, upper_y, attr);
        drawPixelARGB32(x, lower_y, attr);

        // Fill the circle, draw a line between lower_y and upper_y
        if (fill) {
            for (int y = lower_y; y <= upper_y; y++) {
                drawPixelARGB32(x, y, attr);
            }
        }
    }

    /* Also draw the circle border when going on y side (
    since some points may be missing due to inaccurate calculation above) */

    for (int y = center_y - radius; y <= center_y + radius; y++) {
        // Calculate the corresponding x values using the circle equation
        int dx = sqrt(radius * radius - (y - center_y) * (y - center_y)); 

        int left_x = center_x - dx;
        int right_x = center_x + dx;

        drawPixelARGB32(left_x, y, attr);
        drawPixelARGB32(right_x, y, attr);
    }
}
// void drawBitmap(int x0, int y0, int width, int height, const uint32_t *bitmap) {
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             unsigned long color = bitmap[y * width + x];
            
//             drawPixelARGB32(x0 + x, y0 + y, color);
//         }
//     }
// }

void drawBitmap(int x0, int y0, int width, int height, const uint32_t *bitmap) {
    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            uint32_t color = bitmap[y * width + x];


            //Only draw if alpha is not 0 (fully transparent)
            if (color == 0x00ffffff ) {
                continue; // Skip fully transparent pixels
            } else {
                drawPixelARGB32(x0 + x, y0 + y, color);
            }
            
        }
    }
}

void display_video(int x, int y, int width, int height, const uint32_t **frames, int num_frames, int delay_count) {
    while (1) {
        for (int i = 0; i < num_frames; i++) {
            drawBitmap(x, y, width, height, frames[i]);
            delay(delay_count);
        }
    }
}
void drawBitmapFlipped(int x0, int y0, int width, int height, const uint32_t *bitmap) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // read row from bottom instead of top
            uint32_t color = bitmap[(height - 1 - y) * width + x];

            if (color == 0x00ffffff) {
                continue; // Skip transparent
            } else {
                drawPixelARGB32(x0 + x, y0 + y, color);
            }
        }
    }
}

// Function to swap the buffers
// If noBuff is true, it means we are using the top buffer.
// If noBuff is false, we are using the bottom buffer.

void swapBuffer(int bufferIndex) {
    if (bufferIndex){

        mBuf[0] = 8*4; // Length of message in bytes
        mBuf[1] = MBOX_REQUEST;
        
        mBuf[2] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
        mBuf[3] = 8;
        mBuf[4] = 0;
        mBuf[5] = 0; // x offset
        mBuf[6] = 0; // y offset

        mBuf[7] = MBOX_TAG_LAST;
        // Call Mailbox
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    }
    else{
        mBuf[0] = 8*4; // Length of message in bytes
        mBuf[1] = MBOX_REQUEST;
        
        mBuf[2] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
        mBuf[3] = 8;
        mBuf[4] = 0;
        mBuf[5] = 0; // x offset
        mBuf[6] = BACKGROUND_IMAGE_HEIGHT; // y offset

        mBuf[7] = MBOX_TAG_LAST;
        // Call Mailbox
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    }
   


}

void changeMapColor() {
    mBuf[0] = 8*4; // Length of message in bytes
    mBuf[1] = MBOX_REQUEST;

    mBuf[2] = MBOX_TAG_SETPXLORDR; //Set pixel order
    mBuf[3] = 4;
    mBuf[4] = 0;
    mBuf[5] = !PIXEL_ORDER;

    mBuf[6] = MBOX_TAG_LAST;
    // Call Mailbox
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
}

void drawChar(unsigned char ch, int x, int y, unsigned int attr, int zoom)
{
    unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for (int i = 1; i <= (FONT_HEIGHT*zoom); i++) {
		for (int j = 0; j< (FONT_WIDTH*zoom); j++) {
			unsigned char mask = 1 << (j/zoom);
            if (*glyph & mask) { //only draw pixels belong to the character glyph
			    drawPixelARGB32(x + j, y + i, attr);
            }
		}
		glyph += (i % zoom) ? 0 : FONT_BPL;
    }
}

/* Functions to display image on the screen */
void drawString(int x, int y, char *str, unsigned int attr, int zoom)
{
    while (*str) {
        if (*str == '\r') {
            x = 0;
        } else if (*str == '\n') {
            x = 0; 
			y += (FONT_HEIGHT*zoom);
        } else {
            drawChar(*str, x, y, attr, zoom);
            x += (FONT_WIDTH*zoom);
        }
        str++;
    }
}

void drawFinishedLv(int bufferIndex){
    switch (bufferIndex)
    {
        case 1:
            drawString(80, 280, "LEVEL FINISHED!!", 1, 7);
        case 0: 
            drawString(80, 280 + BACKGROUND_IMAGE_HEIGHT, "LEVEL FINISHED!!", 1, 7);
    }
}

// int drawGameStart(int *bufferIndex, int *playing, char c){

//     if (c == ' ') {
//         *playing = !(*playing);
//         return 1;

//     }
//     else 
//     {
//         drawBackground(*bufferIndex);
//         switch (*bufferIndex)
//         {
//             case 1:
//                 drawString(140, 300, "PRESS SPACE TO PLAY", 1, 5);
//             case 0: 
//                 drawString(140, 300 + BACKGROUND_IMAGE_HEIGHT, "PRESS SPACE TO PLAY", 1, 5);
//         }
//         swapBuffer(*bufferIndex);
//         *bufferIndex = !(*bufferIndex);
//         wait_msec(50);

//     }

int drawGameStart(int *bufferIndex, char c) {
    if (c == ' ') {
        return 1;   // signal "start game"
    }
    else {
        drawBackground(*bufferIndex);

        switch (*bufferIndex) {
        case 1:
            drawString(140, 300, "PRESS SPACE TO PLAY", 1, 5);
            break;
        case 0:
            drawString(140, 300 + BACKGROUND_IMAGE_HEIGHT, "PRESS SPACE TO PLAY", 1, 5);
            break;
        }

        swapBuffer(*bufferIndex);
        *bufferIndex = !(*bufferIndex);

        wait_msec(50);

        return 0;   // still waiting
    }
}
    

void drawGoodLuck(int bufferIndex){

}


void clearScreen(unsigned int attr) {
    // fill full screen
    drawRectARGB32(0, 0, (int)width - 1, (int)height - 1, attr, 1);
}
void drawIndexedSprite(int x, int y, int w, int h,
                       const unsigned char* data,
                       const unsigned int* palette,
                       int scale)
{
    if (scale < 1) scale = 1;
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            unsigned char idx = data[j * w + i];
            if (idx == 0) continue; // 0 = transparent
            unsigned int color = palette[idx];

            // vẽ block scale x scale
            for (int dy = 0; dy < scale; ++dy) {
                for (int dx = 0; dx < scale; ++dx) {
                    drawPixelARGB32(x + i*scale + dx, y + j*scale + dy, color);
                }
            }
        }
    }
}
// Bảng sin/cos rời rạc cho các góc: {-25,-10,0,30,70} độ, fixed-point ×1024
static const int COS_TAB[5] = { 929, 1008, 1024, 887, 350 };
static const int SIN_TAB[5] = {-432, -178,    0, 512, 962 };

void drawIndexedSpriteRot(int x, int y, int w, int h,
                          const unsigned char* data,
                          const unsigned int* palette,
                          int scale,
                          int angle_idx)
{
    if (scale < 1) scale = 1;
    if (angle_idx < 0) angle_idx = 0;
    if (angle_idx > 4) angle_idx = 4;

    const int c = COS_TAB[angle_idx];
    const int s = SIN_TAB[angle_idx];
    const int FP = 1024;

    const int dstW = w * scale;
    const int dstH = h * scale;
    const int cx = dstW / 2;
    const int cy = dstH / 2;

    for (int dy = 0; dy < dstH; ++dy) {
        for (int dx = 0; dx < dstW; ++dx) {
            // tọa độ tương đối quanh tâm (pixel đích)
            int rx = dx - cx;
            int ry = dy - cy;

            // ánh xạ ngược về nguồn (quay -theta)
            // sx_scaled, sy_scaled đang ở “đơn vị” (scale * FP)
            int sx_scaled =  c * rx + s * ry;   // (c*x + s*y)
            int sy_scaled = -s * rx + c * ry;   // (-s*x + c*y)

            // chia cho scale và FP để về đơn vị pixel nguồn
            int sx = sx_scaled / (scale * FP) + (w / 2);
            int sy = sy_scaled / (scale * FP) + (h / 2);

            if ((unsigned)sx < (unsigned)w && (unsigned)sy < (unsigned)h) {
                unsigned char idx = data[sy * w + sx];
                if (idx) {
                    drawPixelARGB32(x + dx, y + dy, palette[idx]);
                }
            }
        }
    }
}











   












