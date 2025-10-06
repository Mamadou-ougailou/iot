#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>
// used Pins
#define CLIMPIN 19
#define RADPIN 21
#define FANPIN 27
#define TACHPIN 26
#define LEDSTRIPPIN 13
#define NUMLEDS 5

// Seuil
#define SB 24
#define SH 26


// Config de température 
OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);

// Config de ledStrip
Adafruit_NeoPixel strip(NUMLEDS, LEDSTRIPPIN, NEO_GRB + NEO_KHZ800);

float getTemperature(){
  float t;
  tempSensor.requestTemperatures();
  t = tempSensor.getTempCByIndex(0);
  return t;
}

void controlLeds(float t){
  if(t >= SH){
    digitalWrite(CLIMPIN, HIGH);
    digitalWrite(RADPIN, LOW);
  } else if(t <= SB){
    digitalWrite(RADPIN, HIGH);
    digitalWrite(CLIMPIN, LOW);
  }else{
    digitalWrite(CLIMPIN, LOW);
    digitalWrite(RADPIN, LOW);
  }

}
void setLEDS(uint32_t color) {
    for (int i = 0; i < NUMLEDS; i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void controlLedStrip(float t){
  if(t >= SH){
    setLEDS(strip.Color(255, 0, 0));
  } else if (t <= SB){
    setLEDS(strip.Color(0, 0, 255));
  } else{
    setLEDS(strip.Color(0, 255, 0));
  }
}


void setup() {
  Serial.begin(9600);

  // init du capteur et de OneWire
  tempSensor.begin();

  // init ledPins as an OUTPUT
  pinMode(CLIMPIN, OUTPUT);
  pinMode(RADPIN, OUTPUT);

  // init ledStrip
  strip.begin();

  // init fan
  //ledcAttach(FANPIN, 25000, 8);
  //attachInterrupt(digitalPinToInterrupt(TACHPIN), isr, RISING);
}

void loop() {
  float temperature = getTemperature();
  Serial.print(temperature);
  Serial.println(" °C");
  controlLeds(temperature);
  controlLedStrip(temperature);
   
  delay(1000);
}
