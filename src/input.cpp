// ╔══════════════════════════════════════════╗
// ║       JAM-PACK · input.cpp              ║
// ╚══════════════════════════════════════════╝

#include "input.h"
#include "config.h"

struct Btn {
  uint8_t  pin;
  bool     lastState;
  uint32_t lastDebounce;
};

static Btn bUp   = { PIN_UP,   HIGH, 0 };
static Btn bDown = { PIN_DOWN, HIGH, 0 };
static Btn bOk   = { PIN_OK,   HIGH, 0 };
static Btn bBack = { PIN_BACK, HIGH, 0 };

void input_init() {
  pinMode(PIN_UP,   INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_OK,   INPUT_PULLUP);
  pinMode(PIN_BACK, INPUT_PULLUP);
}

static bool _pressed(Btn &b) {
  bool reading = digitalRead(b.pin);
  if (reading == LOW && b.lastState == HIGH) {
    if (millis() - b.lastDebounce > DEBOUNCE_MS) {
      b.lastDebounce = millis();
      b.lastState    = LOW;
      return true;
    }
  }
  if (reading == HIGH) b.lastState = HIGH;
  return false;
}

bool btn_up()   { return _pressed(bUp);   }
bool btn_down() { return _pressed(bDown); }
bool btn_ok()   { return _pressed(bOk);   }
bool btn_back() { return _pressed(bBack); }
