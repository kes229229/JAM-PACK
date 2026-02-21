#pragma once
#include <Arduino.h>

// ╔══════════════════════════════════════════╗
// ║    JAM-PACK · bt_module.h               ║
// ╚══════════════════════════════════════════╝

const char** bt_items();
int          bt_count();
void         bt_run(int idx, uint32_t startTime);
