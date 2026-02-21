#pragma once
#include <Arduino.h>

const char** gpio_items();
int          gpio_count();
void         gpio_run(int idx, uint32_t startTime);
