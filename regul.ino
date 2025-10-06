#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>
// used Pins
#define CLIMPIN 19
#define RADPIN 21
#define FANPIN 27
#define LEDSTRIPPIN 13
#define NUMLEDS 5

// Seuil
#define SB 24
#define SH 26
#define LIGHTMAX 3500
int onMax = 0;

   


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

void controlFan(float t, int light){

  digitalWrite(2, LOW);
  if(t < SH){
      ledcWrite(FANPIN, 0);
      onMax = 0;
  }else{
    if(light < LIGHTMAX){
      if(onMax != 1){
        for(int i = 1 ; i <= 255; i = i+10){
          ledcWrite(FANPIN, i);
          delay(1000);
        }
        onMax = 1;
      } else{
        ledcWrite(FANPIN, 255);
      }
    } else{
      digitalWrite(2, HIGH);
      ledcWrite(FANPIN, 0);
      onMax = 0;
    }  
  }
}

void setup() {
  Serial.begin(9600);

  // init du capteur et de OneWire
  tempSensor.begin();

  // init ledPins as an OUTPUT
  pinMode(CLIMPIN, OUTPUT);
  pinMode(RADPIN, OUTPUT);
  pinMode(2, OUTPUT); // led d'incendie

  // init ledStrip
  strip.begin();

  // init fan
  ledcAttach(FANPIN, 0, 8);
}

void loop() {
  float temperature = getTemperature();
  int light = 4095 - analogRead(A5);
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.println(light);

  controlLeds(temperature);
  controlLedStrip(temperature);
  controlFan(temperature, light);
  
  delay(1000);
}
