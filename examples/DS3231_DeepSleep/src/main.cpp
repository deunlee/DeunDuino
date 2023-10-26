// Code by Deun Lee (https://github.com/deunlee)
// MIT License

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>

#define uS_TO_S_FACTOR 1000000ULL
RTC_DATA_ATTR int boot_count = 0;

RTC_DS3231 rtc;
const char days_of_week[7][12] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

void init_rtc();
void print_current_time();
void print_alarms();

void init_rtc() {
    if (!rtc.begin(&Wire)) { // includes Wire.begin()
        Serial.println(F("[RTC] The RTC module is not recognized!"));
        while (1);
    }

    // The 32kHz output is enabled by default, but disable it because it is not used.
    rtc.disable32K();
    // The square wave output(SQW) and the alarm interrupt output(INT) share the same pin.
    // To use the alarm interrupt, the square wave output must be turned off.
    rtc.writeSqwPinMode(DS3231_OFF);
    // Turn off two alarms. (not to disable or remove, just off)
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);

    if (rtc.lostPower()) {
        Serial.println(F("[RTC] Lost time because of power loss. Set time to compile time."));
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // rtc.adjust(DateTime(2023, 10, 20, 14, 0, 0));
    }

    // DS3231_Ax_Second fires an alarm when seconds match. (Ignore year, month, day, hour and minute)
    // DS3231_Ax_Hour fires an alarm when hours, minutes and seconds match.
    // For detailed alarm modes, see print_alarms() function.
    rtc.setAlarm1(DateTime(2000, 1, 1, 0, 0, 30), DS3231_A1_Second);
    rtc.setAlarm2(DateTime(2000, 1, 1, 0, 0,  0), DS3231_A2_PerMinute);
    print_alarms();

    // disableAlarm() disables physical signal output on the INT pin.
    // It has no effect when checking alarms with I2C using alarmFired().
    // When setAlarm() is called, disableAlarm() is cleared and INT pin is activated again if an alarm fired.
    rtc.disableAlarm(1);
    rtc.disableAlarm(2);
}

void print_current_time() {
    DateTime now = rtc.now();

    Serial.printf("[RTC] Time: %4d-%02d-%02d (%s) %02d:%02d:%02d / Unix=%u / Temp=%.1f'C\r\n",
        now.year(), now.month(), now.day(), days_of_week[now.dayOfTheWeek()],
        now.hour(), now.minute(), now.second(), now.unixtime(), rtc.getTemperature());
}

void print_alarms() {
    // Alarm 1
    DateTime         alarm1 = rtc.getAlarm1();
    Ds3231Alarm1Mode mode1  = rtc.getAlarm1Mode();
    Serial.printf("[RTC] Alarm1: %02d %02d:%02d:%02d (mode=",
        alarm1.day(), alarm1.hour(), alarm1.minute(), alarm1.second());
    switch (mode1) {
        case DS3231_A1_PerSecond: Serial.print(F("PerSecond")); break;
        case DS3231_A1_Second:    Serial.print(F("Second"));    break;
        case DS3231_A1_Minute:    Serial.print(F("Minute"));    break;
        case DS3231_A1_Hour:      Serial.print(F("Hour"));      break;
        case DS3231_A1_Date:      Serial.print(F("Date"));      break; // (day of month)
        case DS3231_A1_Day:       Serial.print(F("Day"));       break; // (day of week)
    }
    Serial.printf(", fire=%s)\r\n", rtc.alarmFired(1) ? "YES" : "no");

    // Alarm 2
    DateTime         alarm2 = rtc.getAlarm2();
    Ds3231Alarm2Mode mode2  = rtc.getAlarm2Mode();
    Serial.printf("[RTC] Alarm2: %02d %02d:%02d:%02d (mode=",
        alarm2.day(), alarm2.hour(), alarm2.minute(), alarm2.second());
    switch (mode2) {
    case DS3231_A2_PerMinute: Serial.print(F("PerMinute")); break;
    case DS3231_A2_Minute:    Serial.print(F("Minute"));    break;
    case DS3231_A2_Hour:      Serial.print(F("Hour"));      break;
    case DS3231_A2_Date:      Serial.print(F("Date"));      break;
    case DS3231_A2_Day:       Serial.print(F("Day"));       break;
    }
    Serial.printf(", fire=%s)\r\n", rtc.alarmFired(2) ? "YES" : "no");
}

void print_wakeup_reason() {
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println(F("[System] Wakeup caused by external signal using RTC_IO"));
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println(F("[System] Wakeup caused by external signal using RTC_CNTL"));
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println(F("[System] Wakeup caused by timer"));
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println(F("[System] Wakeup caused by touchpad"));
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println(F("[System] Wakeup caused by ULP program"));
        break;
    default:
        Serial.print(F("[System] Wakeup was NOT caused by deep sleep: "));
        Serial.println(wakeup_reason);
        break;
    }
}

void setup() {
    Serial.begin(115200);
    Serial.printf("\n\n[System] Boot number: %d\r\n", ++boot_count);
    print_wakeup_reason();

    init_rtc();
}

const uint32_t TIME_TO_RUNNING = 30;  // in seconds
const uint32_t TIME_TO_SLEEP   = 300; // in seconds

void loop() {
    print_current_time();

    if (rtc.alarmFired(1)) {
        Serial.println("[RTC] Alarm1 fired!");
        rtc.clearAlarm(1);
    }
    if (rtc.alarmFired(2)) {
        Serial.println("[RTC] Alarm2 fired!");
        rtc.clearAlarm(2);
    }

    if (millis() >= TIME_TO_RUNNING * 1000) {
        DateTime now = rtc.now();
        DateTime future(now + TimeSpan(TIME_TO_SLEEP));
        Serial.println();
        Serial.printf("[System] Going to deep-sleep now for %d seconds!\r\n", TIME_TO_SLEEP);
        Serial.printf("[System] The ESP32 will wake up at %4d-%02d-%02d %02d:%02d:%02d.\r\n",
            future.year(), future.month(), future.day(), future.hour(), future.minute(), future.second());

        esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
        Serial.println("[System] Configured all RTC peripherals to be powered down in sleep.");
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
        // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
        // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
        esp_deep_sleep_start();
        Serial.println();
        Serial.flush();
        Serial.println(F("[System] This will never be printed."));
    }

    delay(1000);
}

// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/deep-sleep-stub.html

