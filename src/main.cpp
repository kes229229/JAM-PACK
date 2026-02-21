// ╔══════════════════════════════════════════╗
// ║       JAM-PACK · main.cpp               ║
// ║   Точка входа — только setup/loop       ║
// ╚══════════════════════════════════════════╝

#include <Arduino.h>
#include "display.h"
#include "input.h"
#include "menu.h"

void setup() {
  Serial.begin(115200);
  display_init();
  input_init();
  menu_init();
  Serial.println("[JAM-PACK] Boot OK");
}

void loop() {
  menu_loop();
}
