// ╔══════════════════════════════════════════╗
// ║   JAM-PACK · serial_control.cpp         ║
// ║   Управление через USB Serial (115200)  ║
// ╚══════════════════════════════════════════╝

#include "serial_control.h"
#include "menu.h"

// Протокол: JSON строки через \n
// Сайт → ESP32:  {"cmd":"nav","dir":"ok"}
// ESP32 → Сайт:  {"state":"wifi","idx":0,"items":["Deauther","Beacon Spam"]}

extern AppState currentState;  // из menu.cpp
extern int      menuIndex;
extern int      subIndex;
extern bool     running;

static String inputBuf = "";

// Все меню для отправки на сайт
static const char* mainItems[]     = { "WiFi","Bluetooth","SubGHz","Infrared","GPIO","Settings" };
static const char* wifiItems[]     = { "Deauther","Beacon Spam","EvilPortal","Wardriving" };
static const char* btItems[]       = { "BLE Spam iOS","BLE Spam Android","BLE Spam Win","BadBLE" };
static const char* subghzItems[]   = { "SubRead","SubSend","Analyzer","Bruteforce" };
static const char* irItems[]       = { "IR Send","IR Read","TV/PJ/AC OFF" };
static const char* gpioItems[]     = { "iButton Read","iButton Write","iButton Cfg","NRF24 Spectrum","NRF24 Cfg","Serial[DEV]" };
static const char* settingsItems[] = { "Display Color","Standby Time","About" };

static const char** stateItems[] = {
  nullptr, mainItems, wifiItems, btItems,
  subghzItems, irItems, gpioItems, settingsItems
};
static const int stateCounts[] = { 0, 6, 4, 4, 4, 3, 6, 3 };

static const char* stateNames[] = {
  "boot","main","wifi","bt","subghz","ir","gpio","settings"
};

// ── Отправить текущее состояние на сайт ───
static void send_state() {
  int si = (int)currentState;
  int idx = (currentState == ST_MAIN) ? menuIndex : subIndex;

  Serial.print(F("{\"type\":\"state\",\"state\":\""));
  Serial.print(stateNames[si]);
  Serial.print(F("\",\"idx\":"));
  Serial.print(idx);
  Serial.print(F(",\"running\":"));
  Serial.print(running ? "true" : "false");
  Serial.print(F(",\"items\":["));

  if (stateItems[si] != nullptr) {
    for (int i = 0; i < stateCounts[si]; i++) {
      Serial.print(F("\""));
      Serial.print(stateItems[si][i]);
      Serial.print(F("\""));
      if (i < stateCounts[si] - 1) Serial.print(F(","));
    }
  }

  Serial.println(F("]}"));
}

// ── Симулировать нажатие кнопки ───────────
static void press_btn(const String& btn) {
  // Импортируем функции из input.cpp напрямую через флаги
  extern volatile bool _btn_up_flag;
  extern volatile bool _btn_down_flag;
  extern volatile bool _btn_ok_flag;
  extern volatile bool _btn_back_flag;

  if      (btn == "up")   _btn_up_flag   = true;
  else if (btn == "down") _btn_down_flag = true;
  else if (btn == "ok")   _btn_ok_flag   = true;
  else if (btn == "back") _btn_back_flag = true;
}

// ── Парсим JSON команду ───────────────────
// Формат: {"cmd":"nav","btn":"ok"}
//         {"cmd":"get_state"}
//         {"cmd":"run","idx":0}
static void parse_command(const String& json) {
  // cmd
  int ci = json.indexOf("\"cmd\":\"");
  if (ci < 0) return;
  ci += 7;
  int ce = json.indexOf("\"", ci);
  String cmd = json.substring(ci, ce);

  if (cmd == "get_state") {
    send_state();
    return;
  }

  if (cmd == "nav") {
    int bi = json.indexOf("\"btn\":\"");
    if (bi < 0) return;
    bi += 7;
    int be = json.indexOf("\"", bi);
    String btn = json.substring(bi, be);
    press_btn(btn);
    delay(20);
    send_state();
    return;
  }

  if (cmd == "ping") {
    Serial.println(F("{\"type\":\"pong\",\"fw\":\"JAM-PACK\",\"ver\":\"1.0.0\"}"));
    return;
  }
}

void serial_control_init() {
  // Уже инициализирован в main.cpp
  Serial.println(F("{\"type\":\"boot\",\"fw\":\"JAM-PACK\",\"ver\":\"1.0.0\"}"));
}

void serial_control_tick() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputBuf.trim();
      if (inputBuf.length() > 0) {
        parse_command(inputBuf);
      }
      inputBuf = "";
    } else {
      inputBuf += c;
      if (inputBuf.length() > 256) inputBuf = ""; // защита от переполнения
    }
  }
}
