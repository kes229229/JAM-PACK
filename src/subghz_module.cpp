// ╔══════════════════════════════════════════╗
// ║    JAM-PACK · subghz_module.cpp         ║
// ╚══════════════════════════════════════════╝

#include "subghz_module.h"
#include "display.h"
#include "config.h"

static const char* items[] = {
  "SubRead", "SubSend", "Analyzer", "Bruteforce"
};
static const int COUNT = 4;

const char** subghz_items() { return items; }
int          subghz_count() { return COUNT; }

void subghz_run(int idx, uint32_t startTime) {
  switch (idx) {
    case 0:
      display_run_screen("SubRead",    "Listening 433MHz...", true, startTime);
      // TODO: cc1101_read_tick()
      break;
    case 1:
      display_run_screen("SubSend",    "Loading file...",     true, startTime);
      // TODO: cc1101_send_tick()
      break;
    case 2:
      display_run_screen("Analyzer",   "Scanning spectrum...",true, startTime);
      // TODO: cc1101_analyzer_tick()
      break;
    case 3:
      display_run_screen("Bruteforce", "Running Came...",     true, startTime);
      // TODO: cc1101_bruteforce_tick()
      break;
  }
}
