// kernel.c
#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"

// ==================== Delay (fallback nhỏ) ====================
static void delay_cycles(unsigned int n) {
    while (n--) { asm volatile("nop"); }
}

// ==================== ARM Generic Timer helpers ====================
// Đọc tần số bộ đếm (tick/giây)
static inline unsigned long cntfrq_el0() {
    unsigned long r;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(r));
    return r;
}
// Đọc bộ đếm tick tăng dần
static inline unsigned long long cntpct_el0() {
    unsigned long long r;
    asm volatile("mrs %0, cntpct_el0" : "=r"(r));
    return r;
}

// ==================== Palette cho Flappy ====================
static const unsigned int BIRD_PALETTE[6] = {
    0x00000000, // 0 transparent
    0x00FFD200, // 1 yellow body
    0x00FF8A00, // 2 orange beak
    0x00FFFFFF, // 3 white eye
    0x00000000, // 4 black outline/pupil
    0x006A3B0F  // 5 brown wing
};

// ==================== Sprites (16x12) ====================
static const unsigned char bird_up[16*12] = {
0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,
0,0,0,4,4,1,1,1,4,0,0,0,0,0,0,0,
0,0,4,1,1,1,1,1,1,4,0,0,0,0,0,0,
0,4,1,1,1,1,1,1,1,1,4,0,0,0,0,0,
4,1,1,3,4,1,1,1,1,1,1,4,0,0,0,0,
4,1,1,4,1,1,1,1,1,1,1,4,2,2,0,0,
4,1,1,1,1,1,1,1,1,5,5,4,2,2,0,0,
4,1,1,1,1,1,5,5,5,1,1,4,0,0,0,0,
0,4,1,1,1,1,5,1,1,1,4,0,0,0,0,0,
0,0,4,1,1,1,1,1,1,4,0,0,0,0,0,0,
0,0,0,4,4,1,1,1,4,0,0,0,0,0,0,0,
0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0
};

static const unsigned char bird_mid[16*12] = {
0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,
0,0,0,4,4,1,1,1,4,0,0,0,0,0,0,0,
0,0,4,1,1,1,1,1,1,4,0,0,0,0,0,0,
0,4,1,1,1,1,1,1,1,1,4,0,0,0,0,0,
4,1,1,3,4,1,1,1,1,1,1,4,0,0,0,0,
4,1,1,4,1,1,1,1,1,1,1,4,2,2,0,0,
4,1,1,1,1,5,5,5,5,1,1,4,2,2,0,0,
4,1,1,1,1,1,1,1,1,1,1,4,0,0,0,0,
0,4,1,1,1,1,1,1,1,1,4,0,0,0,0,0,
0,0,4,1,1,1,1,1,1,4,0,0,0,0,0,0,
0,0,0,4,4,1,1,1,4,0,0,0,0,0,0,0,
0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0
};

// ==================== Main ====================
void main() {
    uart_init();
    uart_puts("\n\nHello Flappy (tilt + dirty-rect + stable dt)!\n");

    framebf_init();

    // --- Physics  ---
    const float TARGET_FPS = 60.0f;
    const float FIXED_DT_FOR_PACING = 1.0f / TARGET_FPS; // chỉ dùng để pace frame, không dùng cho physics
    const float GRAVITY     = 1500.0f;
    const float JUMP_VELOCITY = -550.0f;
    const float MAX_FALL    = 900.0f;

    const unsigned int W = framebf_width();
    const unsigned int H = framebf_height();
    const unsigned int bgColor = 0x00000000;

    // --- Bird state ---
    const int birdX = 220;
    const int birdW = 16, birdH = 12;
    const int scale = 2;
    const int birdRadius = (birdH * scale) / 2;

    float y  = H * 0.5f;
    float vy = 0.0f;

    // --- Dirty-rect chống nhấp nháy ---
    int prevDrawX = -10000, prevDrawY = -10000;
    int animCounter = 0;
    const int pad  = 4 * scale;                     // dư biên khi xoay
    const int boxW = birdW * scale + 2 * pad;
    const int boxH = birdH * scale + 2 * pad;

    // Vẽ nền ban đầu (1 lần)
    clearScreen(bgColor);

    // --- Timer setup ---
    const double freq = (double)cntfrq_el0();       // ticks per second
    unsigned long long last_tick = cntpct_el0();    // tick mốc ban đầu
    double acc_for_pacing = 0.0;                    // để pace về 60fps nếu muốn

    while (1) {
        // ====== đo dt thật ======
        unsigned long long now_tick = cntpct_el0();
        double dt = (double)(now_tick - last_tick) / freq; // giây
        last_tick = now_tick;

        // Clamp dt để tránh spike (khi debug/breakpoint)
        if (dt < 0.0)      dt = 0.0;
        if (dt > 1.0/15.0) dt = 1.0/15.0; // tối đa ~4 frame 15 fps
        // (Không ép dt = 1/60; dùng dt thật cho PHYSICS)

        // ====== Input ======
        unsigned char ch = uart_read();
        if (ch == ' ') {
            vy = JUMP_VELOCITY;   // nhảy -> sẽ nghiêng lên
        }

        // ====== Physics dùng dt thật ======
        vy += GRAVITY * (float)dt;
        if (vy > MAX_FALL) vy = MAX_FALL;
        y  += vy * (float)dt;

        // Clamp biên
        if (y < birdRadius) { y = (float)birdRadius; vy = 0.0f; }
        if (y > (float)(H - birdRadius - 1)) { y = (float)(H - birdRadius - 1); vy = 0.0f; }

        // ====== XÓA VÙNG CŨ (dirty-rect) ======
        if (prevDrawX != -10000) {
            int ex = prevDrawX - pad;
            int ey = prevDrawY - pad;
            int ex2 = ex + boxW - 1;
            int ey2 = ey + boxH - 1;
            if (ex  < 0) ex  = 0;
            if (ey  < 0) ey  = 0;
            if (ex2 >= (int)W) ex2 = (int)W - 1;
            if (ey2 >= (int)H) ey2 = (int)H - 1;
            drawRectARGB32(ex, ey, ex2, ey2, bgColor, 1);
        }

        // ====== Chọn frame cánh ======
        // Tăng theo thời gian thật để animation mượt khi FPS thay đổi
        static double wing_time_acc = 0.0;
        wing_time_acc += dt;
        // đổi frame mỗi ~0.1s
        if (wing_time_acc >= 0.1) {
            animCounter++;
            wing_time_acc -= 0.1;
        }
        const unsigned char* wingFrame = ((animCounter % 2) == 0) ? bird_up : bird_mid;

        // ====== Góc nghiêng theo vy (giữ nguyên ngưỡng) ======
        // vy (px/s):   < -350  | -350..-100 | -100..200 | 200..600 | >=600
        // angle_idx:       0   |     1      |     2     |     3    |   4
        int angle_idx;
        if (vy < -350.0f)      angle_idx = 0; // -25°
        else if (vy < -100.0f) angle_idx = 1; // -10°
        else if (vy < 200.0f)  angle_idx = 2; // 0°
        else if (vy < 600.0f)  angle_idx = 3; // +30°
        else                   angle_idx = 4; // +70°

        // ====== Render sprite xoay ======
        int drawX = birdX - (birdW * scale) / 2;
        int drawY = (int)y - (birdH * scale) / 2;
        drawIndexedSpriteRot(drawX, drawY, birdW, birdH, wingFrame, BIRD_PALETTE, scale, angle_idx);

        prevDrawX = drawX;
        prevDrawY = drawY;

        // ====== Frame pacing về ~60fps ======
        // Đảm bảo mỗi vòng lặp tốn ít nhất FIXED_DT_FOR_PACING giây
        acc_for_pacing += dt;
        while (acc_for_pacing < FIXED_DT_FOR_PACING) {
            // bận chờ một chút rồi đo lại
            delay_cycles(500);
            unsigned long long t2 = cntpct_el0();
            double add = (double)(t2 - now_tick) / freq;
            now_tick = t2;
            acc_for_pacing += add;
        }
        acc_for_pacing -= FIXED_DT_FOR_PACING;
        if (acc_for_pacing > 0.5) acc_for_pacing = 0.0; // tránh drift khi lag lâu
    }
}
