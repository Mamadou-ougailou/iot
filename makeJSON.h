#ifndef MAKEJSON_H
#define MAKEJSON_H

#include <ArduinoJson.h>
#include <Arduino.h>

// Structure to hold ESP model data
struct esp_model {
    int luminosity = 0;
    float temperature = 0.0;
    bool coolerState = false;
    bool heaterState = false;
    bool fireDetected = false;
    int fanSpeed = 0;

    static constexpr float highTempThreshold = 26.0;
    static constexpr float lowTempThreshold = 25.8;
    static constexpr double latitude = 43.62454;
    static constexpr double longitude = 7.050628;
    static const char* room;
    static const char* address;
    static const char* target_ip;
    static constexpr int target_port = 1880;
    static constexpr int target_sp = 2;

    String WiFiSSID;
    String MAC;
    String IP;
};

// Static initializations
inline const char* esp_model::room = "512";
inline const char* esp_model::address = "Les lucioles";
inline const char* esp_model::target_ip = "127.0.0.1";

esp_model *esp;

// Function declarations
void updateValues(esp_model& em, int luminosity, float temperature, int fanSpeed, bool coolerState, bool heaterState, bool fireDetected);
StaticJsonDocument<1000> makeJSON_fromStatus(esp_model *em);
StaticJsonDocument<1000> makeJSON_fromlocation(double lat, double lon);

#endif