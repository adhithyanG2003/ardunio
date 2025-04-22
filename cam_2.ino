#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "12345678";
const char* password = "12345678";
const char* serverUrl = "http://192.0.0.4:8000/upload";  // Android app server URL

#define TOUCH_PIN 4  // GPIO 4 (T0)

// Camera Configuration
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
IPAddress local_IP(192, 168, 43, 50);    // Your chosen IPv4 address
IPAddress gateway(192, 168, 43, 1);        // Your hotspot's gateway
IPAddress subnet(255, 255, 255, 0);         // Subnet mask

// Optionally set DNS servers if needed
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    // Camera Init
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    // Check for PSRAM and configure accordingly
    if (psramFound()) {
        Serial.println("PSRAM found: Using higher resolution settings.");
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        Serial.println("PSRAM not found: Using lower resolution settings.");
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.print("Camera init failed with error 0x");
        Serial.println(err, HEX);
        return;
    }
    Serial.println("Camera initialized successfully!");
}

void loop() {
    int touchValue = touchRead(TOUCH_PIN);
    Serial.print("Touch value: ");
    Serial.println(touchValue);

    if (touchValue < 30) {  // If touch sensor is activated
        Serial.println("Touch detected! Capturing image...");
        captureAndSend();
        delay(5000);  // Prevent multiple triggers
    }
}

void captureAndSend() {
    WiFiClient client;
    HTTPClient http;

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "image/jpeg");

    int httpResponseCode = http.POST(fb->buf, fb->len);

    if (httpResponseCode > 0) {
        Serial.print("Image sent successfully! HTTP response code: ");
        Serial.println(httpResponseCode);
    } else {
        Serial.print("Error sending image, HTTP response code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
    esp_camera_fb_return(fb);
}
