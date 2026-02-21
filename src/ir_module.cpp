// ╔══════════════════════════════════════════╗
// ║    JAM-PACK · ir_module.cpp             ║
// ╚══════════════════════════════════════════╝

#include "ir_module.h"
#include "display.h"

static const char* items[] = { "IR Send", "IR Read", "TV/PJ/AC OFF" };
static const int COUNT = 3;

const char** ir_items() { return items; }
int          ir_count() { return COUNT; }

void ir_run(int idx, uint32_t startTime) {
  switch (idx) {
    case 0: display_run_screen("IR Send",     "Sending signal...", true,  startTime); break;
    case 1: display_run_screen("IR Read",     "Waiting for IR...", false, startTime); break;
    case 2: display_run_screen("TV/PJ/AC OFF","Broadcasting OFF...",true, startTime); break;
  }
  // TODO: ir_tick(idx)
}
