#pragma once
#include <Arduino.h>

// ╔══════════════════════════════════════════╗
// ║    JAM-PACK · wifi_module.h             ║
// ╚══════════════════════════════════════════╝

const char** wifi_items();
int          wifi_count();
void         wifi_run(int idx, uint32_t startTime);
