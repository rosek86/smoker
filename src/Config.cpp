#include "Config.h"
#include <EEPROM.h>
#include <Arduino.h>

#define MAGIC_VALUE 0x5a5a5a5aUL
#define HOURS_TO_MS(h) ((h) * 60UL * 60UL * 1000UL)

void Config::load() {
  EEPROM.begin(512);

  uint32_t magic;
  EEPROM.get(0, magic);

  if (magic != MAGIC_VALUE) {
    Serial.println("Config: magic");
    restoreDefault();
    EEPROM.end();
    return;
  }

  uint32_t crc;
  EEPROM.get(4, crc);
  EEPROM.get(8, config);

  if (crc != calculateChecksum()) {
    Serial.println("Config: crc");
    restoreDefault();
    EEPROM.end();
    return;
  }

  if (config.entries > CONFIG_MAX_ENTRIES) {
    Serial.println("Config: entries");
    restoreDefault();
    EEPROM.end();
    return;
  }

  Serial.println("Config: ok");
  EEPROM.end();
}

void Config::store() {
  EEPROM.begin(512);
  EEPROM.put(0, MAGIC_VALUE);
  EEPROM.put(4, calculateChecksum());
  EEPROM.put(8, config);
  EEPROM.end();
}

void Config::print() {
  Serial.print("Entries: ");
  Serial.println(config.entries);
  Serial.flush();

  for (uint32_t i = 0; i < config.entries; i++) {
    Serial.print(i+1);
    Serial.print(" - time: ");
    Serial.print(config.data[i].time_ms / 1000);
    Serial.print(", temp: ");
    Serial.print(config.data[i].temperature);
    Serial.print(", smoke: ");
    Serial.println(config.data[i].smoke); 
    Serial.flush();
  }
}

void Config::restoreDefault() {
  config.entries = 3;
  config.data[0] = { HOURS_TO_MS(1UL), 40.0F, false };
  config.data[1] = { HOURS_TO_MS(3UL), 50.0F, true  };
  config.data[2] = { HOURS_TO_MS(1UL), 75.0F, true  };

  // default values:
  config.data[3] = { HOURS_TO_MS(1UL), 40.0F, false };
  config.data[4] = { HOURS_TO_MS(1UL), 40.0F, false };
  config.data[5] = { HOURS_TO_MS(1UL), 40.0F, false };
  config.data[6] = { HOURS_TO_MS(1UL), 40.0F, false };
  config.data[7] = { HOURS_TO_MS(1UL), 40.0F, false };
  config.data[8] = { HOURS_TO_MS(1UL), 40.0F, false };
  config.data[9] = { HOURS_TO_MS(1UL), 40.0F, false };
}

uint32_t Config::calculateChecksum() {
  return crc32((uint8_t *)&config, sizeof(config));
}

uint32_t Config::crc32(uint8_t const *data, uint32_t size) {
  const uint32_t crc_table[] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };

  uint32_t crc = ~0L;
  for (uint32_t i = 0 ; i < size; i++) {
    crc = crc_table[(crc ^ data[i]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (data[i] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}
