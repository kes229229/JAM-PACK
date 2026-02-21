// ╔══════════════════════════════════════════╗
// ║    JAM-PACK · settings_module.cpp       ║
// ╚══════════════════════════════════════════╝

#include "settings_module.h"
#include "display.h"
#include "input.h"
#include "config.h"

int cfg_display_color = 0;
int cfg_standby_time  = STANDBY_TIME;

static const char* items[]      = { "Display Color", "Standby Time", "About" };
static const char* colorNames[] = { "Green", "White", "Amber" };
static const int   COUNT        = 3;

const char** settings_items() { return items; }
int          settings_count() { return COUNT; }

// ── Display Color ─────────────────────────
static void run_display_color() {
  if (btn_up())   cfg_display_color = (cfg_display_color - 1 + 3) % 3;
  if (btn_down()) cfg_display_color = (cfg_display_color + 1) % 3;

  u8g2.clearBuffer();
  display_header("Display Color");
  u8g2.setFont(u8g2_font_6x10_mf);
  for (int i = 0; i < 3; i++) {
    int y = 22 + i * 12;
    if (i == cfg_display_color) {
      u8g2.drawBox(0, y - 9, 128, 11);
      u8g2.setDrawColor(0);
      u8g2.drawStr(6, y, colorNames[i]);
      u8g2.drawStr(1, y, ">");
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(6, y, colorNames[i]);
    }
  }
  display_statusbar("BACK", "UP/DN");
  u8g2.sendBuffer();
}

// ── Standby Time ──────────────────────────
static void run_standby_time() {
  if (btn_up())   cfg_standby_time = min(cfg_standby_time + 10, 120);
  if (btn_down()) cfg_standby_time = max(cfg_standby_time - 10, 10);

  u8g2.clearBuffer();
  display_header("Standby Time");
  u8g2.setFont(u8g2_font_10x20_mf);
  char buf[10];
  snprintf(buf, sizeof(buf), "%d s", cfg_standby_time);
  u8g2.drawStr(38, 42, buf);
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(10, 22, "UP + / DOWN -");
  display_statusbar("BACK", "UP/DN ADJ");
  u8g2.sendBuffer();
}

// ── About ─────────────────────────────────
static void run_about() {
  u8g2.clearBuffer();
  display_header("About");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(2, 22, "JAM-PACK FW v" FW_VERSION);
  u8g2.drawStr(2, 32, "Ultimate");
  u8g2.drawStr(2, 42, "ESP32 + SH1106");
  
  display_statusbar("BACK", "");
  u8g2.sendBuffer();
}

void settings_run(int idx) {
  switch (idx) {
    case 0: run_display_color(); break;
    case 1: run_standby_time();  break;
    case 2: run_about();         break;
  }
}
