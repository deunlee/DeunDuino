// Code by Deun Lee (https://github.com/deunlee)
// MIT License

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

// #define DISPLAY_SH1106G
// #define DISPLAY_SSD1306
#define DISPLAY_SH1107

#define AUTO_DETECT_I2C
#define SCREEN_ADDRESS 0x3C // 0x3C or 0x3D
#define OLED_RESET       -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#if defined(DISPLAY_SH1106G) || defined(DISPLAY_SH1107)
#include <Adafruit_SH110X.h>
#define COLOR_WHITE    SH110X_WHITE
#define COLOR_BLACK    SH110X_BLACK
#define COLOR_INVERSE  SH110X_INVERSE
#define SCREEN_WIDTH    64
#define SCREEN_HEIGHT  128
#define SH110X_NO_SPLASH
#if defined(DISPLAY_SH1106G)
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#elif defined(DISPLAY_SH1107)
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif
#endif

#ifdef DISPLAY_SSD1306
#include <Adafruit_SSD1306.h>
#define COLOR_WHITE    SSD1306_WHITE
#define COLOR_BLACK    SSD1306_BLACK
#define COLOR_INVERSE  SSD1306_INVERSE
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT   64 // 32
#define SSD1306_NO_SPLASH
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

#define NUMFLAKES 10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH  16
static const unsigned char PROGMEM logo_bmp[] =
    { 0b00000000, 0b11000000,
      0b00000001, 0b11000000,
      0b00000001, 0b11000000,
      0b00000011, 0b11100000,
      0b11110011, 0b11100000,
      0b11111110, 0b11111000,
      0b01111110, 0b11111111,
      0b00110011, 0b10011111,
      0b00011111, 0b11111100,
      0b00001101, 0b01110000,
      0b00011011, 0b10100000,
      0b00111111, 0b11100000,
      0b00111111, 0b11110000,
      0b01111100, 0b11110000,
      0b01110000, 0b01110000,
      0b00000000, 0b00110000 };

void draw_single_pixel() {
    display.clearDisplay();
    display.drawPixel(10, 10, COLOR_WHITE);
    // You must call display() after making any drawing commands to make them visible.
    display.display();
    delay(1000);
}

void draw_line() {
    display.clearDisplay();
    for (int i = 0; i < display.width(); i += 4) {
        display.drawLine(0, 0, i, display.height() - 1, COLOR_WHITE);
        display.display();
        delay(1);
    }
    for (int i = 0; i < display.height(); i += 4) {
        display.drawLine(0, 0, display.width() - 1, i, COLOR_WHITE);
        display.display();
        delay(1);
    }
    delay(250);
    display.clearDisplay();
    for (int i = 0; i < display.width(); i += 4) {
        display.drawLine(0, display.height() - 1, i, 0, COLOR_WHITE);
        display.display();
        delay(1);
    }
    for (int i = display.height() - 1; i >= 0; i -= 4) {
        display.drawLine(0, display.height() - 1, display.width() - 1, i, COLOR_WHITE);
        display.display();
        delay(1);
    }
    delay(250);
    display.clearDisplay();
    for (int i = display.width() - 1; i >= 0; i -= 4) {
        display.drawLine(display.width() - 1, display.height() - 1, i, 0, COLOR_WHITE);
        display.display();
        delay(1);
    }
    for (int i = display.height() - 1; i >= 0; i -= 4) {
        display.drawLine(display.width() - 1, display.height() - 1, 0, i, COLOR_WHITE);
        display.display();
        delay(1);
    }
    delay(250);
    display.clearDisplay();
    for (int i = 0; i < display.height(); i += 4) {
        display.drawLine(display.width() - 1, 0, 0, i, COLOR_WHITE);
        display.display();
        delay(1);
    }
    for (int i = 0; i < display.width(); i += 4) {
        display.drawLine(display.width() - 1, 0, i, display.height() - 1, COLOR_WHITE);
        display.display();
        delay(1);
    }
    delay(1000);
}

void draw_rect() {
    display.clearDisplay();
    for (int i = 0; i < display.height() / 2; i += 2) {
        display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, COLOR_WHITE);
        display.display();
        delay(1);
    }
    delay(1000);
}

void draw_filled_rect() {
    display.clearDisplay();
    for (int i = 0, color = 1; i < display.height() / 2; i += 3) {
        display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
        display.display();
        delay(1);
        color++;
    }
    delay(1000);
    display.clearDisplay();
    for (int i = 0; i < display.height() / 2; i += 3) {
        display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, COLOR_INVERSE);
        display.display();
        delay(1);
    }
    delay(1000);
}

void draw_circle() {
    display.clearDisplay();
    for (int i = 0; i < max(display.width(), display.height()) / 2; i += 2) {
        display.drawCircle(display.width() / 2, display.height() / 2, i, COLOR_WHITE);
        display.display();
        delay(1);
    }
    delay(1000);
}

void draw_filled_circle() {
    display.clearDisplay();
    for (int i = max(display.width(), display.height()) / 2; i > 0; i -= 3) {
        display.fillCircle(display.width() / 2, display.height() / 2, i, COLOR_INVERSE);
        display.display();
        delay(1);
    }
    delay(1000);
}

void draw_round_rect() {
    display.clearDisplay();
    for (int i = 0; i < display.height() / 2 - 2; i += 2) {
        display.drawRoundRect(
            i, i, display.width() - 2 * i, display.height() - 2 * i,
            display.height() / 4, COLOR_WHITE);
        display.display();
        delay(1);
    }
    delay(1000);
}

void draw_filled_round_rect() {
    display.clearDisplay();
    for (int i = 0; i < display.height() / 2 - 2; i += 2) {
        display.fillRoundRect(
            i, i, display.width() - 2 * i, display.height() - 2 * i,
            display.height() / 4, COLOR_INVERSE);
        display.display();
        delay(1);
    }
    delay(1000);
}

void draw_triangle() {
    display.clearDisplay();
    for (int i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
        display.drawTriangle(
            display.width() / 2, display.height() / 2 - i,
            display.width() / 2 - i, display.height() / 2 + i,
            display.width() / 2 + i, display.height() / 2 + i, COLOR_WHITE);
        display.display();
        delay(1);
    }
    delay(1000);
}

void draw_filled_triangle() {
    display.clearDisplay();
    for (int i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
        display.fillTriangle(
            display.width() / 2, display.height() / 2 - i,
            display.width() / 2 - i, display.height() / 2 + i,
            display.width() / 2 + i, display.height() / 2 + i, COLOR_INVERSE);
        display.display();
        delay(1);
    }
    delay(1000);
}

void draw_char() {
    display.clearDisplay();
    display.setTextSize(1); // Normal 1:1 pixel scale
    display.setTextColor(COLOR_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);
    // Use full 256 char 'Code Page 437' font.
    // Not all the characters will fit on the display. This is normal.
    // Library will draw what it can and the rest will be clipped.
    for (int i = 0; i < 256; i++) {
        if (i == '\n') continue;
        display.write(i);
    }
    display.display();
    delay(1000);
}

void draw_styles() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(COLOR_WHITE);
    display.setCursor(0, 0);
    display.println("Hello, world!");
    display.setTextColor(COLOR_BLACK, COLOR_WHITE); // Draw inverted text
    display.println(3.141592);
    display.setTextSize(2); // Draw 2x-scale text
    display.setTextColor(COLOR_WHITE);
    display.print("0x");
    display.println(0xDEADBEEF, HEX);
    display.display();
    delay(1000);
}

#ifdef DISPLAY_SSD1306
void draw_scrolling_text() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(COLOR_WHITE);
    display.setCursor(10, 0);
    display.println(F("scroll"));
    display.display();
    delay(100);
    display.startscrollright(0x00, 0x0F);
    delay(1000);
    display.stopscroll();
    delay(1000);
    display.startscrollleft(0x00, 0x0F);
    delay(1000);
    display.stopscroll();
    delay(1000);
    display.startscrolldiagright(0x00, 0x07);
    delay(1000);
    display.startscrolldiagleft(0x00, 0x07);
    delay(1000);
    display.stopscroll();
    delay(1000);
}
#else
void draw_scrolling_text() { }
#endif

void draw_bitmap() {
    display.clearDisplay();
    display.drawBitmap(
        (display.width() - LOGO_WIDTH) / 2,
        (display.height() - LOGO_HEIGHT) / 2,
        logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(1000);
}

void animate(const uint8_t* bitmap, uint16_t w, uint16_t h) {
    struct icons {
        int16_t x;
        int16_t y;
        int16_t delta_y;
    } icons[NUMFLAKES];
    for (int i = 0; i < NUMFLAKES; i++) {
        icons[i].x = random(display.width()) - LOGO_WIDTH / 2;
        icons[i].y = 0;
        icons[i].delta_y = random(1, 6);
        Serial.print("x: ");
        Serial.print(icons[i].x, DEC);
        Serial.print(" y: ");
        Serial.print(icons[i].y, DEC);
        Serial.print(" dy: ");
        Serial.println(icons[i].delta_y, DEC);
    }
    while (1) {
        display.clearDisplay();
        for (int i = 0; i < NUMFLAKES; i++) {
            display.drawBitmap(icons[i].x, icons[i].y, bitmap, w, h, COLOR_WHITE);
        }
        display.display();
        delay(200);
        for (int i = 0; i < NUMFLAKES; i++) {
            icons[i].y += icons[i].delta_y;
            if (icons[i].y > display.height()) {
                icons[i].x = random(display.width()) - LOGO_WIDTH / 2;
                icons[i].y = -LOGO_HEIGHT;
                icons[i].delta_y = random(1, 6);
            }
        }
    }
}

void i2c_scan() {
    byte address, error, count = 0;
    Serial.print(F("[I2C] Scanning I2C...\n"));
    Wire.begin();
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.print(F("[I2C] Device found at address 0x"));
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.print("  !\n");
            count++;
        } else if (error == 4) {
            Serial.print(F("[I2C] Unknown error at address 0x"));
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (count) {
        Serial.print(F("[I2C] Scan done!\n\n"));
    } else {
        Serial.print(F("[I2C] No I2C devices found\n\n"));
    }
}

bool i2c_check(byte address) {
    Wire.begin();
    Wire.beginTransmission(address);
    return Wire.endTransmission() == 0;
}

void setup() {
    Serial.begin(115200);
    delay(200);
    i2c_scan();

    byte i2c_address = SCREEN_ADDRESS;

#ifdef AUTO_DETECT_I2C
    if      (i2c_check(0x3C)) i2c_address = 0x3C;
    else if (i2c_check(0x3D)) i2c_address = 0x3D;
#endif

#if defined(DISPLAY_SH1106G) || defined(DISPLAY_SH1107)
    if (!display.begin(i2c_address, true)) {
        Serial.print(F("SH110X allocation failed.\n"));
        while (1);
    }
    display.setContrast(127); // 0 ~ 127
#elif defined(DISPLAY_SSD1306)
    if (!display.begin(SSD1306_SWITCHCAPVCC, i2c_address)) {
        Serial.print(F("SSD1306 allocation failed.\n"));
        while (1);
    }
#endif

    draw_single_pixel();
    draw_line();
    draw_rect();
    draw_filled_rect();
    draw_circle();
    draw_filled_circle();
    draw_round_rect();
    draw_filled_round_rect();
    draw_triangle();
    draw_filled_triangle();
    draw_char();
    draw_styles();
    draw_scrolling_text();
    draw_bitmap();

    // Invert and restore display, pausing in-between.
    display.invertDisplay(true);
    delay(1000);
    display.invertDisplay(false);
    delay(1000);

    animate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT);
}

void loop() { }
