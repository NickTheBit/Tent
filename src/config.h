#include <Arduino.h>
#include <iostream>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "tent.h"

class Config {

    public:
        Tent *tents;
        Config();
};

Config::Config() {
    int i,j;
    Serial.begin(115200);
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
        int arraySize = config["tents"].size();
        tents = new Tent[arraySize];
        for (i=0; i<arraySize; i++) {
            tents[i].id = config["tents"][i]["id"];
            tents[i].up_relay = config["tents"][i]["up_relay"];
            tents[i].down_relay = config["tents"][i]["down_relay"];
            tents[i].speed = config["tents"][i]["speed"];
            tents[i].cur_position = config["tents"][i]["curposition"];
        }
    }
    
}