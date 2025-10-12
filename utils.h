#ifndef CLIMATECONTROL_H
#define CLIMATECONTROL_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>

// Used Pins
#define CLIMPIN 19
#define RADPIN 21
#define FANPIN 27
#define LEDSTRIPPIN 13
#define NUMLEDS 5

// Seuil
#define SB 25.8
#define SH 26
#define LIGHTMAX 3500

extern int luminosity;
extern float temperature;
extern bool coolerState;
extern bool heaterState;
extern bool fireDetected;
extern int fanSpeed;

// Config de température
extern OneWire oneWire;
extern DallasTemperature tempSensor;

// Config de ledStrip
extern Adafruit_NeoPixel strip;

void initLeds();
void initFan();
float getTemperature();
void setLEDS(uint32_t color);
void progressiveFan();
void checkStatus(float t, int light);
void controlSensors();

#endif