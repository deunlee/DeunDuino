#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

uint32_t wheel(byte pos) { // pos(0 ~ 255) -> color(R->G->B->R...)
    pos = 255 - pos;
    if (pos < 85) {
        return pixel.Color(255 - pos * 3, 0, pos * 3);
    }
    if (pos < 170) {
        pos -= 85;
        return pixel.Color(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return pixel.Color(pos * 3, 255 - pos * 3, 0);
}

void color_wipe(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < pixel.numPixels(); i++) {
        pixel.setPixelColor(i, c);
        pixel.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait) {
    uint16_t i, j;
    for (j = 0; j < 256; j++) {
        for (i = 0; i < pixel.numPixels(); i++) {
            pixel.setPixelColor(i, wheel((i + j) & 255));
        }
        pixel.show();
        delay(wait);
    }
}

void rainbow_cycle(uint8_t wait) {
    uint16_t i, j;
    for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
        for (i = 0; i < pixel.numPixels(); i++) {
            pixel.setPixelColor(i, wheel(((i * 256 / pixel.numPixels()) + j) & 255));
        }
        pixel.show();
        delay(wait);
    }
}

void theater_chase(uint32_t c, uint8_t wait) {
    for (int j = 0; j < 10; j++) { // 10 cycles of chasing
        for (int q = 0; q < 3; q++) {
            for (uint16_t i = 0; i < pixel.numPixels(); i = i + 3) {
                pixel.setPixelColor(i + q, c); // Turn every third pixel on
            }
            pixel.show();
            delay(wait);
            for (uint16_t i = 0; i < pixel.numPixels(); i = i + 3) {
                pixel.setPixelColor(i + q, 0); // Turn every third pixel off
            }
        }
    }
}

void theater_chase_rainbow(uint8_t wait) {
    for (int j = 0; j < 256; j++) { // Cycle all 256 colors in the wheel
        for (int q = 0; q < 3; q++) {
            for (uint16_t i = 0; i < pixel.numPixels(); i = i + 3) {
                pixel.setPixelColor(i + q, wheel((i + j) % 255)); // Turn every third pixel on
            }
            pixel.show();
            delay(wait);
            for (uint16_t i = 0; i < pixel.numPixels(); i = i + 3) {
                pixel.setPixelColor(i + q, 0); // Turn every third pixel off
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

    pixel.begin();
    pixel.setBrightness(30); // 0 ~ 255

    pixel.setPixelColor(0, pixel.Color(255, 0, 0)); // Red
    pixel.show();
    delay(500);

    pixel.clear(); // Off
    pixel.show();
    delay(500);

    pixel.setPixelColor(0, pixel.Color(0, 255, 0)); // Green
    pixel.show();
    delay(500);

    pixel.clear(); // Off
    pixel.show();
    delay(500);

    pixel.setPixelColor(0, pixel.Color(0, 0, 255)); // Blue
    pixel.show();
    delay(500);

    pixel.clear(); // Off
    pixel.show();
    delay(500);
}

void loop() {
    // color_wipe(pixel.Color(255, 0, 0), 50); // Red
    // color_wipe(pixel.Color(0, 255, 0), 50); // Green
    // color_wipe(pixel.Color(0, 0, 255), 50); // Blue

    // theater_chase(pixel.Color(127, 127, 127), 50); // White
    // theater_chase(pixel.Color(127, 0, 0), 50); // Red
    // theater_chase(pixel.Color(0, 0, 127), 50); // Blue

    rainbow(10);
    // rainbow_cycle(10);
    // theater_chase_rainbow(50);
}
