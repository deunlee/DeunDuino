// Code by Deun Lee (https://github.com/deunlee)
// MIT License

#define ETHERNET_USE_ESP32
#define ETHERNET_LARGE_BUFFERS
#define BOARD_TYPE      "ESP32"
#define W5500_RST_PORT  W5500_RST
#define PIN_MISO        MISO
#define PIN_MOSI        MOSI
#define PIN_SCK         SCK
#define PIN_SS          SS
#define _ETG_LOGLEVEL_  2 // 0 ~ 4
#define DEBUG_ETHERNET_GENERIC_PORT Serial
#include <SPI.h>
#include <Ethernet_Generic.h>

byte mac_address[6] = { 0, };
EthernetClient client;

char server[] = "ipinfo.io";
char url[] = "/json";

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 5000);

    Serial.println();
    Serial.print("[System] MAC Address Wi-Fi (default): ");
    uint64_t mac = ESP.getEfuseMac();
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", (mac >> i * 8) & 0xff);
    }
    Serial.println();

    // https://docs.espressif.com/projects/esp-idf/en/v4.4.6/esp32s3/api-reference/system/system.html#mac-address
    esp_read_mac(mac_address, ESP_MAC_ETH);
    Serial.print("[System] MAC Address for Ethernet: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", mac_address[i]);
    }
    Serial.println();

	delay(200);
    Ethernet.init(W5500_CS);

    // Use DHCP dynamic IP:
    Ethernet.begin(mac_address);

    // Use static IP:
    // IPAddress ip(192, 168, 0, 200);
    // IPAddress dns(8, 8, 8, 8);
    // Ethernet.begin(mac_address, ip, dns);

    Serial.print(F("[ETH] Connected! IP address: "));
    Serial.println(Ethernet.localIP());
    Serial.print(F("[ETH] Speed: "));
    Serial.print(Ethernet.speedReport());
    Serial.print(F(", Duplex: "));
    Serial.print(Ethernet.duplexReport());
    Serial.print(F(", Link status: "));
    Serial.println(Ethernet.linkReport());

    Serial.println();
    Serial.println(F("[ETH] Starting connection to server..."));
    if (client.connect(server, 80)) {
        Serial.println(F("[ETH] Connected to server!"));
        Serial.println();
        client.printf("GET %s HTTP/1.1\r\n", url);
        client.printf("Host: %s\r\n", server);
        client.print("Connection: close\r\n\r\n");
    }

    while (1) {
        while (client.available()) {
            Serial.write((char)client.read());
            Serial.flush();
        }
        if (!client.connected()) {
            Serial.println();
            Serial.println(F("[ETH] Disconnecting from server..."));
            client.stop();
            break;
        }
        delay(100);
    }
}

void loop() { }
