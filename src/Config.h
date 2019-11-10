#ifndef CONFIG_H__
#define CONFIG_H__

#include <stdint.h>
#include <stdbool.h>

#define CONFIG_MAX_ENTRIES 10

struct ConfigEntry {
  uint32_t time_ms;
  float temperature;
  bool smoke;
};

struct ConfigData {
  uint32_t entries;
  ConfigEntry data[CONFIG_MAX_ENTRIES];
};

class Config {
public:
  void load();
  void store();
  void print();

  void getData(ConfigData &data) {
    data = config;
  }

  void setData(ConfigData const &data) {
    config = data;
  }
private:
  ConfigData config;

  void restoreDefault();
  uint32_t calculateChecksum();
  uint32_t crc32(uint8_t const *data, uint32_t size);
};

#endif // CONFIG_H__
