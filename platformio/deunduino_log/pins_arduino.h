#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define EXTERNAL_NUM_INTERRUPTS 16
#define NUM_DIGITAL_PINS        40
#define NUM_ANALOG_INPUTS       16

#define analogInputToDigitalPin(p)  (((p)<20)?(analogChannelToDigitalPin(p)):-1)
#define digitalPinToInterrupt(p)    (((p)<40)?(p):-1)
#define digitalPinHasPWM(p)         (p < 34)

static const uint8_t LED_BUILTIN = 2;
#define BUILTIN_LED  LED_BUILTIN // backward compatibility

static const uint8_t TX  = 1;
static const uint8_t RX  = 3;
static const uint8_t TX0 = TX;
static const uint8_t RX0 = RX;
static const uint8_t TX2 = 17;
static const uint8_t RX2 = 16;

static const uint8_t SDA = 21;
static const uint8_t SCL = 22;

static const uint8_t SS    = 5;
static const uint8_t MOSI  = 23;
static const uint8_t MISO  = 19;
static const uint8_t SCK   = 18;

static const uint8_t SDCARD_EN = 4;
static const uint8_t SDCARD_CS = 5;

static const uint8_t D0 = 0;
static const uint8_t D1 = 1;
static const uint8_t D2 = 2;
static const uint8_t D3 = 3;
static const uint8_t D4 = 4;
static const uint8_t D5 = 5;
static const uint8_t D12 = 12;
static const uint8_t D13 = 13;
static const uint8_t D14 = 14;
static const uint8_t D15 = 15;
static const uint8_t D16 = 16;
static const uint8_t D17 = 17;
static const uint8_t D18 = 18;
static const uint8_t D19 = 19;
static const uint8_t D21 = 21;
static const uint8_t D22 = 22;
static const uint8_t D23 = 23;
static const uint8_t D25 = 25;
static const uint8_t D26 = 26;
static const uint8_t D27 = 27;
static const uint8_t D32 = 32;
static const uint8_t D33 = 33;

static const uint8_t A32 = 32;
static const uint8_t A33 = 33;
static const uint8_t A34 = 34;
static const uint8_t A35 = 35;
static const uint8_t A36 = 36;
static const uint8_t A39 = 39;

static const uint8_t T0 = 4;
static const uint8_t T1 = 0;
static const uint8_t T2 = 2;
static const uint8_t T3 = 15;
static const uint8_t T4 = 13;
static const uint8_t T5 = 12;
static const uint8_t T6 = 14;
static const uint8_t T7 = 27;
static const uint8_t T8 = 33;
static const uint8_t T9 = 32;

static const uint8_t DAC1 = 25;
static const uint8_t DAC2 = 26;

static const uint8_t RTC_ALARM    = 34;
static const uint8_t VBAT_VOLTAGE = 35;

#endif /* Pins_Arduino_h */
