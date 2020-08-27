#include <Arduino.h>
#include "Button2.h"
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "tent.h"
#include <ArduinoJson.h>

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN 0x10
#endif

#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 23

#define TFT_BL 4  // Display backlight control pin
#define ADC_EN 14 // ADC_EN is the ADC detection enable port
#define ADC_PIN 34
#define BUTTON_1 35
#define BUTTON_2 0

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library
Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

// Remove hardcoded instances
const char *ssid = "Getofmylawn";
const char *password = "Rtfm-Qf6";

AsyncWebServer server(80);

// Processor handles all placeholders in html file
String processor(const String &var) {
    if (var == "id") {
        // gets placeholders and replaces them with the value in the controller
        Serial.println("Called");
        return "ok";
    }
    return String();
}

void setup(void) {

    Serial.begin(115200);

    // Screen initialization
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(9);
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(0, 0);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    // Wifi handling
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    tft.print("Connected:\n");
    tft.println(WiFi.localIP());

    if (MDNS.begin("tenta.local")) {
        MDNS.addService("http","tcp",80);
    }

    // Load configuration
    int i;
    Tent *tents;
    if (!SPIFFS.begin(true)) {
        Serial.println("Storage failure");
    }
    File file = SPIFFS.open("/config.json","r+");
    if (!file) {
        Serial.println("Config not found\nGenerating...");
        file = SPIFFS.open("./config.json","w");


    } else {
        // Config exists prepare classes
        DynamicJsonBuffer jb;
        JsonObject& config = jb.parseObject(file);
        file.close();
        int number = config["tents"].size();
        tents = new Tent[number];
        for (i=0; i<number; i++) {
            tents[i].id = config["tents"][i]["id"];
            tents[i].up_relay = config["tents"][i]["up_relay"];
            tents[i].down_relay = config["tents"][i]["down_relay"];
            tents[i].speed = config["tents"][i]["speed"];
            tents[i].cur_position = config["tents"][i]["curposition"];
            pinMode(tents[i].up_relay, OUTPUT);
            pinMode(tents[i].down_relay, OUTPUT);
            digitalWrite(tents[i].down_relay, HIGH);
            digitalWrite(tents[i].up_relay, HIGH);
            Serial.println(tents[i].down_relay);
            Serial.println(tents[i].up_relay);
        }
    }

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    // Route to load style.css file
    server.on("/cosmetics.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/cosmetics.css", "text/css");

    });

    // Route to set GPIO to HIGH
    server.on("/on", HTTP_GET, [tents](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false, processor);
        Serial.println("On sent.");
        tents[2].setstat(100);
    });

    // Route to set GPIO to LOW
    server.on("/off", HTTP_GET, [tents](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false, processor);
        Serial.println("off sent");
        tents[2].setstat(0);
    });

    // Start server
    server.begin();
    tft.println("\nHttp server started");
    Serial.println("HTTP server started");
    
}

void loop(void) { delay(10000); }
