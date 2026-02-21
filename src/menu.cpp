// ╔══════════════════════════════════════════╗
// ║       JAM-PACK · menu.cpp               ║
// ╚══════════════════════════════════════════╝

#include "menu.h"
#include "display.h"
#include "input.h"
#include "wifi_module.h"
#include "bt_module.h"
#include "subghz_module.h"
#include "ir_module.h"
#include "gpio_module.h"
#include "settings_module.h"
#include "menu/icons.h"

// ── Главное меню ──────────────────────────
static const char* mainItems[] = {
  "WiFi", "Bluetooth", "SubGHz", "Infrared", "GPIO", "Settings"
};
static const int mainCount = 6;

// ── Состояние ─────────────────────────────
static AppState state     = ST_BOOT;
static int      mainIdx   = 0;
static int      subIdx    = 0;
static bool     running   = false;
static uint32_t runTimer  = 0;
static AppState returnSt  = ST_MAIN;

// ── Хелпер: обработать подменю ────────────
static bool handle_submenu(const char** items, int count) {
  if (btn_up())   subIdx = (subIdx - 1 + count) % count;
  if (btn_down()) subIdx = (subIdx + 1) % count;
  if (btn_back()) { state = ST_MAIN; return false; }
  if (btn_ok())   { running = true; runTimer = millis(); return true; }
  return false;
}

void menu_init() { state = ST_BOOT; }

void menu_loop() {
  switch (state) {

    // ── Boot ──────────────────────────────
    case ST_BOOT:
      display_boot();
      if (display_boot_done()) state = ST_MAIN;
      break;

    // ── Главное меню (полный экран, без заголовка) ──
    case ST_MAIN: {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_6x10_mf);

      const int rowH = 10;   // высота строки px
      const int textH = 8;   // baseline смещение

      for (int i = 0; i < mainCount; i++) {
        int boxY = i * rowH;
        int txtY = boxY + textH;

        if (i == mainIdx) {
          u8g2.setDrawColor(1);
          u8g2.drawBox(0, boxY, 128, rowH);
          u8g2.setDrawColor(0);
          u8g2.drawXBMP(2, boxY + 1, 8, 8, menuIcons[i]);
          u8g2.drawStr(13, txtY, mainItems[i]);
          u8g2.drawStr(120, txtY, ">");
          u8g2.setDrawColor(1);
        } else {
          u8g2.setDrawColor(1);
          u8g2.drawXBMP(2, boxY + 1, 8, 8, menuIcons[i]);
          u8g2.drawStr(13, txtY, mainItems[i]);
        }
      }

      u8g2.sendBuffer();

      if (btn_up())   mainIdx = (mainIdx - 1 + mainCount) % mainCount;
      if (btn_down()) mainIdx = (mainIdx + 1) % mainCount;
      if (btn_ok()) {
        subIdx  = 0;
        running = false;
        state   = (AppState)(ST_WIFI + mainIdx);
        returnSt = state;
      }
      break;
    }

    // ── WiFi ──────────────────────────────
    case ST_WIFI:
      if (!running) {
        u8g2.clearBuffer();
        display_list(wifi_items(), wifi_count(), subIdx, 0);
        display_statusbar("BACK", "OK RUN");
        u8g2.sendBuffer();
        handle_submenu(wifi_items(), wifi_count());
      } else {
        wifi_run(subIdx, runTimer);
        if (btn_back()) { running = false; state = ST_WIFI; }
      }
      break;

    // ── Bluetooth ─────────────────────────
    case ST_BT:
      if (!running) {
        u8g2.clearBuffer();
        display_list(bt_items(), bt_count(), subIdx, 0);
        display_statusbar("BACK", "OK RUN");
        u8g2.sendBuffer();
        handle_submenu(bt_items(), bt_count());
      } else {
        bt_run(subIdx, runTimer);
        if (btn_back()) { running = false; state = ST_BT; }
      }
      break;

    // ── SubGHz ────────────────────────────
    case ST_SUBGHZ:
      if (!running) {
        u8g2.clearBuffer();
        display_list(subghz_items(), subghz_count(), subIdx, 0);
        display_statusbar("BACK", "OK RUN");
        u8g2.sendBuffer();
        handle_submenu(subghz_items(), subghz_count());
      } else {
        subghz_run(subIdx, runTimer);
        if (btn_back()) { running = false; state = ST_SUBGHZ; }
      }
      break;

    // ── Infrared ──────────────────────────
    case ST_IR:
      if (!running) {
        u8g2.clearBuffer();
        display_list(ir_items(), ir_count(), subIdx, 0);
        display_statusbar("BACK", "OK RUN");
        u8g2.sendBuffer();
        handle_submenu(ir_items(), ir_count());
      } else {
        ir_run(subIdx, runTimer);
        if (btn_back()) { running = false; state = ST_IR; }
      }
      break;

    // ── GPIO ──────────────────────────────
    case ST_GPIO:
      if (!running) {
        u8g2.clearBuffer();
        display_list(gpio_items(), gpio_count(), subIdx, 0);
        display_statusbar("BACK", "OK RUN");
        u8g2.sendBuffer();
        handle_submenu(gpio_items(), gpio_count());
      } else {
        gpio_run(subIdx, runTimer);
        if (btn_back()) { running = false; state = ST_GPIO; }
      }
      break;

    // ── Settings ──────────────────────────
    case ST_SETTINGS:
      if (!running) {
        u8g2.clearBuffer();
        display_list(settings_items(), settings_count(), subIdx, 0);
        display_statusbar("BACK", "OK SEL");
        u8g2.sendBuffer();
        handle_submenu(settings_items(), settings_count());
      } else {
        settings_run(subIdx);
        if (btn_back()) { running = false; state = ST_SETTINGS; }
      }
      break;
  }
}
