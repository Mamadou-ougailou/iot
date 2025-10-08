#ifndef MAKEJSON_H
#define MAKEJSON_H
#include <string.h>
#include <ArduinoJson.h>

typedef struct {
    int luminosity;
    float temperature;

    const float highTempThreshold = 26.0; 
    const float lowTempThreshold = 25.9;
    bool coolerState;
    bool heaterState;
    bool fireDetected;
    int fanSpeed; 

    // Résidence Newton 2400 Route de Dolines, 06560 Valbonne France
    const double latitude = 43.62454;
    const double longitude = 7.050628;
    char room[50] = "512";
    char address[200] = "Les lucioles";
    // Network
    String WiFISSID;
    String MAC;
    String IP;
    // Reporting 
    String target_ip = "127.0.0.1";
    int target_port = 1880;
    int target_sp = 2;
} esp_model;

StaticJsonDocument<1000> makeJSON_fromlocation(float lat, float lgn);
StaticJsonDocument<1000> makeJSON_fromStatus(esp_model *em);

#endif