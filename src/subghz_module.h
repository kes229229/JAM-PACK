#pragma once
#include <Arduino.h>

// ╔══════════════════════════════════════════╗
// ║    JAM-PACK · subghz_module.h           ║
// ╚══════════════════════════════════════════╝

const char** subghz_items();
int          subghz_count();
void         subghz_run(int idx, uint32_t startTime);
