#ifndef BOARD_H__
#define BOARD_H__

#define GPIO00_PIN        0  // D3  GPIO00 (PROG,Boot)
#define GPIO01_PIN        1  // TX  GPIO01 (TXD0)
#define GPIO02_PIN        2  // D4  GPIO02 (LED,Boot) BTN
#define GPIO03_PIN        3  // RX  GPIO03 (RXD0)
#define GPIO04_PIN        4  // D2  GPIO04 (OLED SCL)
#define GPIO05_PIN        5  // D1  GPIO05 (OLED SDA)
#define GPIO06_PIN        6  // CLK GPIO06 (CLK) -+
#define GPIO07_PIN        7  // SD0 GPIO07 (SD0)  |
#define GPIO08_PIN        8  // SD1 GPIO08 (SD1)  | Connected to
#define GPIO09_PIN        9  // SD2 GPIO09 (SD2)  | external flash
#define GPIO10_PIN        10 // SD3 GPIO10 (SD3)  |
#define GPIO11_PIN        11 // CMD GPIO11 (CMD) -+
#define GPIO12_PIN        12 // D6  GPIO12 ()        REL
#define GPIO13_PIN        13 // D7  GPIO13 ()        REL
#define GPIO14_PIN        14 // D5  GPIO14 ()        ENC
#define GPIO15_PIN        15 // D8  GPIO15 (Boot)
#define GPIO16_PIN        16 // D0  GPIO16 (Wake)    ENC

#define I2C_SCL_PIN       GPIO04_PIN
#define I2C_SDA_PIN       GPIO05_PIN
#define BUTTON_PIN        GPIO02_PIN
#define ENC1_PIN          GPIO14_PIN
#define ENC2_PIN          GPIO12_PIN
#define HEATER_RELAY_PIN  GPIO16_PIN
#define SMOKE_RELAY_PIN   GPIO13_PIN

#define SCREEN_WIDTH      128
#define SCREEN_HEIGHT     64

#endif // BOARD_H__
