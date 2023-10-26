#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define USB_VID 0x303A
#define USB_PID 0x1001

#define EXTERNAL_NUM_INTERRUPTS 46
#define NUM_DIGITAL_PINS        19
#define NUM_ANALOG_INPUTS       8

#define analogInputToDigitalPin(p)  (((p)<20)?(analogChannelToDigitalPin(p)):-1)
#define digitalPinToInterrupt(p)    (((p)<48)?(p):-1)
#define digitalPinHasPWM(p)         (p < 46)

#define PIN_NEOPIXEL 48
#define NUM_NEOPIXEL  1

static const uint8_t TX  = 43;
static const uint8_t RX  = 44;
static const uint8_t TX0 = TX;
static const uint8_t RX0 = RX;
static const uint8_t TX1 = 17;
static const uint8_t RX1 = 18;

static const uint8_t SDA = 4;
static const uint8_t SCL = 2;

static const uint8_t SS    = 10;
static const uint8_t MOSI  = 11;
static const uint8_t SCK   = 12;
static const uint8_t MISO  = 13;

static const uint8_t SDCARD_CS = 14;
static const uint8_t W5500_CS  = 10;
static const uint8_t W5500_RST = 1;
static const uint8_t W5500_INT = 21;

static const uint8_t D0 = 0;
static const uint8_t D15 = 15;
static const uint8_t D16 = 16;
static const uint8_t D17 = 17;
static const uint8_t D18 = 18;
static const uint8_t D35 = 35;
static const uint8_t D36 = 36;
static const uint8_t D37 = 37;
static const uint8_t D38 = 38;
static const uint8_t D39 = 39;
static const uint8_t D40 = 40;
static const uint8_t D41 = 41;
static const uint8_t D42 = 42;
static const uint8_t D43 = 43;
static const uint8_t D44 = 44;
static const uint8_t D45 = 45;
static const uint8_t D46 = 46;
static const uint8_t D47 = 47;
static const uint8_t D48 = 48;

static const uint8_t A2 = 2;
static const uint8_t A3 = 3;
static const uint8_t A4 = 4;
static const uint8_t A5 = 5;
static const uint8_t A6 = 6;
static const uint8_t A7 = 7;
static const uint8_t A8 = 8;
static const uint8_t A9 = 9;

static const uint8_t T3 = 3;
static const uint8_t T4 = 4;
static const uint8_t T5 = 5;
static const uint8_t T6 = 6;
static const uint8_t T7 = 7;
static const uint8_t T8 = 8;
static const uint8_t T9 = 9;

#endif /* Pins_Arduino_h */
