#pragma once

// ╔══════════════════════════════════════════╗
// ║       JAM-PACK · menu.h                 ║
// ╚══════════════════════════════════════════╝

enum AppState {
  ST_BOOT,
  ST_MAIN,
  ST_WIFI,
  ST_BT,
  ST_SUBGHZ,
  ST_IR,
  ST_GPIO,
  ST_SETTINGS
};

void menu_init();
void menu_loop();
