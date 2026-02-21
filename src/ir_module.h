#pragma once
#include <Arduino.h>

const char** ir_items();
int          ir_count();
void         ir_run(int idx, uint32_t startTime);
