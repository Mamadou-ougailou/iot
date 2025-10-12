#include "utils.h"

// Variable definitions
int luminosity = 0;
float temperature = 0;
bool coolerState = false;
bool heaterState = false;
bool fireDetected = false;
int fanSpeed = 0;

// Config de température
OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);

// Config de ledStrip
Adafruit_NeoPixel strip(NUMLEDS, LEDSTRIPPIN, NEO_GRB + NEO_KHZ800);

void initLeds(){
  // init ledPins as an OUTPUT
  pinMode(CLIMPIN, OUTPUT);
  pinMode(RADPIN, OUTPUT);
  pinMode(2, OUTPUT); // led d'incendie
  digitalWrite(CLIMPIN, LOW);
  digitalWrite(RADPIN, LOW);
  digitalWrite(2, LOW);

  // init LedStrips 
  strip.begin();
  strip.clear();
  strip.show();
}

void initFan(){
  // init fan
  ledcAttach(FANPIN, 25000, 8);
  ledcWrite(FANPIN, 0);
}

float getTemperature(){
  float t;
  tempSensor.requestTemperatures();
  t = tempSensor.getTempCByIndex(0);
  return t;
}

void setLEDS(uint32_t color) {
  for (int i = 0; i < NUMLEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void progressiveFan(){
  if(fanSpeed == 0){
    for(int i = 64 ; i < 256; i += 64){
      ledcWrite(FANPIN, i);
      delay(1000);
    }
    fanSpeed = 255;
  } else {
    ledcWrite(FANPIN, 255);
  }
}

void checkStatus(float t, int light){
  if(light > LIGHTMAX && t > SH){
    fireDetected = true;
    coolerState = false;
    heaterState = false;
  } else {
    fireDetected = false;
    if(t > SH){
      coolerState = true;
      heaterState = false;
    } else if (t < SH){
      coolerState = false;
    }

    if(t < SB){
      heaterState = true;
      coolerState = false;
    } else if(t > SB){
      heaterState = false;
    } 
  }
}

void controlSensors(){
  if(coolerState){
    digitalWrite(2, LOW);
    setLEDS(strip.Color(255, 0, 0));
    digitalWrite(CLIMPIN, HIGH);
    digitalWrite(RADPIN, LOW);
    progressiveFan();
  } else {
    ledcWrite(FANPIN, 0);
    fanSpeed = 0;
    digitalWrite(2, LOW);
    if(heaterState){
      setLEDS(strip.Color(0, 0, 255));
      digitalWrite(RADPIN, HIGH);
      digitalWrite(CLIMPIN, LOW);
    } else if(fireDetected){
      setLEDS(strip.Color(255, 255, 255));
      digitalWrite(2, HIGH);
      digitalWrite(RADPIN, LOW);
      digitalWrite(CLIMPIN, LOW);
    } else {
      setLEDS(strip.Color(0, 255, 0));
      digitalWrite(RADPIN, LOW);
      digitalWrite(CLIMPIN, LOW);
    }
  }
}