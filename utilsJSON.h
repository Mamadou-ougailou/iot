#include "makeJSON.h"
esp_model *esp;

void initEsp(esp_model *esp){
  esp->highTempThreshold = SH;
  esp->lowTempThreshold = SB;
  strcpy(esp->room, "123");
  strcpy(esp->address, "Jean Medécin");
  esp->latitude = 43.62453842;
  esp->longitude = 7.050628185;
}

void updateValues( esp_model *esp, int luminosity, float temperature, int fanSpeed, bool coolerState, bool heaterState, bool fireDetected){
  esp->luminosity = luminosity;
  esp->temperature = temperature;
  esp->fanSpeed = fanSpeed;
  esp->coolerState = coolerState;
  esp->heaterState = heaterState;
  esp->fireDetected = fireDetected;
}


StaticJsonDocument<1000> makeJSON_fromStatus(esp_model *em){
  StaticJsonDocument<1000> doc;

  // Status section
  JsonObject status = doc.createNestedObject("status");
  status["temperature"] = em->temperature;
  status["light"] = em->luminosity;
  status["regul"] = (em->coolerState || em->heaterState) ? (em->coolerState ? "COLD" : "HEAT") : "HALT";
  status["fire"] = em->fireDetected;
  status["heat"] = em->heaterState ? "ON" : "OFF";
  status["cold"] = em->coolerState ? "ON" : "OFF";
  status["fanspeed"] = em->fanSpeed;

  // Location section
  JsonObject location = doc.createNestedObject("location");
  location["room"] = esp->room;
  JsonObject gps = location.createNestedObject("gps");
  gps["lat"] = esp->latitude;
  gps["lon"] = esp->longitude;
  location["address"] = esp->address;

  // Regul section
  JsonObject regul = doc.createNestedObject("regul");
  regul["lt"] = esp->lowTempThreshold; 
  regul["ht"] = esp->highTempThreshold; 

  // Info section (hypothetical, as not in esp_model)
  JsonObject info = doc.createNestedObject("info");
  info["ident"] = "ESP32 OG"; 
  info["user"] = "OG";         
  info["loc"] = "Nice";   

  // Net section
  JsonObject net = doc.createNestedObject("net");
  net["uptime"] = 0; // Example: System uptime
  net["ssid"] = "NOP";
  net["mac"] = "NOP";
  net["ip"] = "NOP";

  // Reporthost section
  JsonObject reporthost = doc.createNestedObject("reporthost");
  reporthost["target_ip"] = "127.0.0.1";
  reporthost["target_port"] = em->target_port;
  reporthost["sp"] = em->target_sp;

  return doc;
}