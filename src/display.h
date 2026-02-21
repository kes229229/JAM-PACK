#pragma once
#include <U8g2lib.h>

// ╔══════════════════════════════════════════╗
// ║       JAM-PACK · display.h              ║
// ╚══════════════════════════════════════════╝

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void display_init();
void display_header(const char* title);
void display_list(const char** items, int count, int selected, int yOffset = 0);
void display_statusbar(const char* left, const char* right);
void display_run_screen(const char* title, const char* status, bool spinner = true, uint32_t startTime = 0);
void display_boot();
bool display_boot_done();
