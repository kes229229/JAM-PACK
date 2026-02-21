#pragma once

// ╔══════════════════════════════════════════╗
// ║       JAM-PACK · config.h                ║
// ║   Пины и глобальные настройки            ║
// ╚══════════════════════════════════════════╝

// ── Дисплей SH1106 (I2C) ──────────────────
#define PIN_SCL   22
#define PIN_SDA   21

// ── Кнопки ────────────────────────────────
#define PIN_UP    27
#define PIN_DOWN  26
#define PIN_OK    33
#define PIN_BACK  32

// ── CC1101 (SPI) ──────────────────────────
#define PIN_CC_CS   4
#define PIN_CC_GDO0 15
#define PIN_CC_SCK  18
#define PIN_CC_MOSI 23
#define PIN_CC_MISO 19

// ── IR ────────────────────────────────────
#define PIN_IR_TX 12
#define PIN_IR_RX 14

// ── NRF24 ─────────────────────────────────
#define PIN_NRF_CE   17
#define PIN_NRF_CSN  16
#define PIN_NRF_SCK  18
#define PIN_NRF_MOSI 23
#define PIN_NRF_MISO 19

// ── GPIO / iButton ────────────────────────
#define PIN_GPIO_A 2
#define PIN_GPIO_B 25
#define PIN_GPIO_C 17
#define PIN_GPIO_D 16
#define PIN_GPIO_E 13
#define PIN_GPIO_F 12

// ── SD Card ───────────────────────────────
#define PIN_SD_CS   5
#define PIN_SD_MOSI 23
#define PIN_SD_CLK  18
#define PIN_SD_MISO 19

// ── Общие настройки ───────────────────────
#define FW_VERSION     "1.0.0"
#define STANDBY_TIME   30       // секунды до выключения дисплея
#define DEBOUNCE_MS    40
