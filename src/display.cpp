// ╔══════════════════════════════════════════╗
// ║       JAM-PACK · display.cpp            ║
// ╚══════════════════════════════════════════╝

#include "display.h"
#include "config.h"
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, PIN_SCL, PIN_SDA);

static uint32_t _bootStart = 0;
static bool     _bootDone  = false;

void display_init() {
  Wire.begin(PIN_SDA, PIN_SCL);
  u8g2.begin();
  u8g2.setContrast(200);
  _bootStart = millis();
}

// ── Заголовок (инвертированная полоса) ────
void display_header(const char* title) {
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.setDrawColor(1);
  u8g2.drawBox(0, 0, 128, 11);
  u8g2.setDrawColor(0);
  u8g2.drawStr(2, 9, title);
  u8g2.setDrawColor(1);
  for (int i = 0; i < 4; i++) {
    int h = 2 + i * 2;
    u8g2.drawBox(116 + i * 3, 10 - h, 2, h);
  }
}

// ── Список пунктов меню ───────────────────
// yOffset=0  → список с самого верха экрана
// yOffset=12 → список под заголовком
void display_list(const char** items, int count, int selected, int yOffset) {
  u8g2.setFont(u8g2_font_6x10_mf);
  const int rowH  = 10;   // высота строки
  const int textH = 8;    // высота символов шрифта 6x10

  // сколько строк влезает от yOffset до статусбара (y=56)
  int visRows = (56 - yOffset) / rowH;
  if (visRows < 1) visRows = 1;

  int scrollStart = (selected >= visRows) ? selected - visRows + 1 : 0;

  for (int i = 0; i < visRows && (i + scrollStart) < count; i++) {
    int idx  = i + scrollStart;
    int boxY = yOffset + i * rowH;       // верх строки
    int txtY = boxY + textH;             // baseline текста

    if (idx == selected) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, boxY, 128, rowH);
      u8g2.setDrawColor(0);
      u8g2.drawStr(2, txtY, ">");
      u8g2.drawStr(12, txtY, items[idx]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.setDrawColor(1);
      u8g2.drawStr(12, txtY, items[idx]);
    }
  }
}

// ── Статус-бар снизу ──────────────────────
void display_statusbar(const char* left, const char* right) {
  u8g2.setFont(u8g2_font_4x6_mf);
  u8g2.drawHLine(0, 56, 128);
  u8g2.drawStr(1, 63, left);
  int rw = u8g2.getStrWidth(right);
  u8g2.drawStr(127 - rw, 63, right);
}

// ── Экран выполнения функции ──────────────
void display_run_screen(const char* title, const char* status, bool spinner, uint32_t startTime) {
  static uint8_t  spinIdx  = 0;
  static uint32_t spinTick = 0;
  const char*     spinSet  = "|/-\\";

  if (spinner && millis() - spinTick > 150) {
    spinIdx = (spinIdx + 1) % 4;
    spinTick = millis();
  }

  u8g2.clearBuffer();
  display_header(title);

  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(4, 26, status);

  if (spinner) {
    char sp[2] = { spinSet[spinIdx], 0 };
    u8g2.drawStr(62, 40, sp);
  }

  if (startTime > 0) {
    u8g2.setFont(u8g2_font_4x6_mf);
    char elapsed[20];
    snprintf(elapsed, sizeof(elapsed), "t: %lus", (millis() - startTime) / 1000UL);
    u8g2.drawStr(2, 53, elapsed);
  }

  display_statusbar("BACK STOP", "");
  u8g2.sendBuffer();
}

// ── Экран загрузки ────────────────────────
void display_boot() {
  static uint8_t  dots    = 0;
  static uint32_t dotTick = 0;

  if (millis() - dotTick > 400) { dots = (dots + 1) % 4; dotTick = millis(); }

  int progress = (int)((millis() - _bootStart) / 40);
  if (progress > 128) progress = 128;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_10x20_mf);
  u8g2.drawStr(10, 26, "JAM-PACK");

  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(28, 38, "FW v" FW_VERSION);

  u8g2.drawFrame(0, 46, 128, 8);
  u8g2.drawBox(0, 46, progress, 8);

  String loading = "Loading";
  for (int i = 0; i < (int)dots; i++) loading += ".";
  u8g2.setFont(u8g2_font_4x6_mf);
  u8g2.drawStr(2, 62, loading.c_str());

  u8g2.sendBuffer();

  if (progress >= 128 && !_bootDone) {
    delay(300);
    _bootDone = true;
  }
}

bool display_boot_done() { return _bootDone; }
