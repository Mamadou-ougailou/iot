#include <DallasTemperature.h>
#include <OneWire.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

// Pins
#define CLIMPIN 19
#define RADPIN 18
#define FANPIN 27
#define LEDSTIPPIN 13
#define NUMLEDS 5
#define TACHPIN 26
#define LDRPIN 33  // Photoresistor 

// Seuils du temperature
#define SB 25
#define SH 26

// Seuil de luminosité 
#define LUM_THRESHOLD 1000  

//Configuration du temperature
OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);

//Config du leds Strip
Adafruit_NeoPixel strip(NUMLEDS, LEDSTIPPIN, NEO_GRB + NEO_KHZ800);

int num = 0;  // Compteur d'impulsions du tachymètre



//Pour obtenir la température
float getTemperature() {
  tempSensor.requestTemperatures();
  return tempSensor.getTempCByIndex(0);
}

//Pour lire la luminosité depuis la photoresistance
int getlumiosite() {
  return analogRead(LDRPIN); // valeur entre 0 et 4095
}

//Pour controoler les leds (climatiseur et radiateur)
void ControlLeds(float temp) {
  if (temp >= SH) {
    digitalWrite(CLIMPIN, HIGH);
    digitalWrite(RADPIN, LOW);
  } else if (temp <= SB) {
    digitalWrite(RADPIN, HIGH);
    digitalWrite(CLIMPIN, LOW);
  } else {
    digitalWrite(CLIMPIN, LOW);
    digitalWrite(RADPIN, LOW);
  }
}

// Pour partager la meme couleur sur la bande de leds Strip
void setLeds(uint32_t color) {
  for (int i = 0; i < NUMLEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

// Pour controler leds strip selon la température
void ControlLedsStrip(float temp) {
  if (temp >= SH)
    // rouge
    setLeds(strip.Color(255, 0, 0)); 
  else if (temp <= SB)
    // vert
    setLeds(strip.Color(0, 255, 0)); 
  else
    // bleu
    setLeds(strip.Color(0, 0, 255)); 
}

// Appeler chaque tour du ventilo 
void IRAM_ATTR isr() {
  num++;
}

// Pour controler le ventilateur selon la température
void ControlVentilo(float t) {
  if (t >= SH) {
    ledcWrite(FANPIN, 255);  // max vitesse
  } else if (t <= SB) {
    ledcWrite(FANPIN, 0);    // off
  } else {
    ledcWrite(FANPIN, 127);  // moyenne
  }
}


 // Si présence de lumière -> le ventillo s'éteint et le strip devient rouge
void ControlLumiere( int lumiosite, float temperature ){
    if (lumiosite < LUM_THRESHOLD) {
    ledcWrite(FANPIN, 0);  // Éteindre le ventilateur
    setLeds(strip.Color(255, 0, 0));  // LEDs rouges
  } 
  // Sinon -> fonctionnement normal selon la température
  else {
    ControlLeds(temperature);
    ControlLedsStrip(temperature);
    ControlVentilo(temperature);
  }

}


void setup() {
  Serial.begin(9600);

  // Init du capteur de température et du bus OneWire
  tempSensor.begin();

  //Init du leds strip
  strip.begin();
  strip.show();
   
  //Initialisation des leds du climatiseur / radiateur
  pinMode(CLIMPIN, OUTPUT);
  pinMode(RADPIN, OUTPUT);

  //Configuration du ventilateur 
  ledcAttach(FANPIN, 25000, 8);

  // Attache de l'interruption pour le tachymètre
  attachInterrupt(digitalPinToInterrupt(TACHPIN), isr, RISING);
}



void loop() {
  // Lecture de la température
  float temperature = getTemperature();

  // Lecture de la luminosité (photoresistor)
  int luminosite = getlumiosite();

  // Affichage dans le moniteur série
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Luminosite: ");
  Serial.println(luminosite);
  
  ControlLumiere(luminosite,temperature );
  
}
