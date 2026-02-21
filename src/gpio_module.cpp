// ╔══════════════════════════════════════════╗
// ║    JAM-PACK · gpio_module.cpp           ║
// ╚══════════════════════════════════════════╝

#include "gpio_module.h"
#include "display.h"
#include "input.h"
#include "config.h"

static const char* items[] = {
  "iButton Read", "iButton Write", "iButton Cfg",
  "NRF24 Spectrum","NRF24 Cfg",   "Serial [DEV]"
};
static const int COUNT = 6;

const char** gpio_items() { return items; }
int          gpio_count() { return COUNT; }

// ── Экраны конфигурации (статические) ─────
static void show_ibutton_cfg() {
  u8g2.clearBuffer();
  display_header("iButton Cfg");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(2, 22, "A:G2  B:G25");
  u8g2.drawStr(2, 32, "C:G17 D:G16");
  u8g2.drawStr(2, 42, "E:G13 F:G12");
  display_statusbar("BACK", "");
  u8g2.sendBuffer();
}

static void show_nrf24_cfg() {
  u8g2.clearBuffer();
  display_header("NRF24 Cfg");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(2, 22, "CE:G17 CSN:G16");
  u8g2.drawStr(2, 32, "SCK:G18");
  u8g2.drawStr(2, 42, "MOSI:G23 MISO:G19");
  display_statusbar("BACK", "");
  u8g2.sendBuffer();
}

static void show_serial_dev() {
  u8g2.clearBuffer();
  display_header("Serial");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(8, 34, "[IN DEVELOPMENT]");
  display_statusbar("BACK", "");
  u8g2.sendBuffer();
}

void gpio_run(int idx, uint32_t startTime) {
  switch (idx) {
    case 0: display_run_screen("iButton Read",   "Place on reader...", false, startTime); break;
    case 1: display_run_screen("iButton Write",  "Writing key...",     true,  startTime); break;
    case 2: show_ibutton_cfg(); break;
    case 3: display_run_screen("NRF24 Spectrum", "Scanning 2.4GHz...", true,  startTime); break;
    case 4: show_nrf24_cfg(); break;
    case 5: show_serial_dev(); break;
  }
  // TODO: gpio_tick(idx)
}
