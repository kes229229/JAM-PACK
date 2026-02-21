#pragma once
#include <Arduino.h>

const char** settings_items();
int          settings_count();
void         settings_run(int idx);

extern int  cfg_display_color;  // 0=Green 1=White 2=Amber
extern int  cfg_standby_time;   // секунды
