#include <DallasTemperature.h>
#include <OneWire.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

////////////////////// Pins //////////////
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

int onMax = 0;

// Seuil de luminosité 
#define LIGHTMAX 3500  

//Configuration du temperature
OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);

//Config du leds Strip
Adafruit_NeoPixel strip(NUMLEDS, LEDSTIPPIN, NEO_GRB + NEO_KHZ800);




/////////////////// TEMPERATURE ///////////////////
//Pour obtenir la température
float getTemperature() {
  tempSensor.requestTemperatures();
  return tempSensor.getTempCByIndex(0);
}


//////////////// LUMIERE //////////////////////////
//Pour lire la luminosité depuis la photoresistance
int getlumiosite() {
  return analogRead(LDRPIN); // valeur entre 0 et 4095
}



/////////////////// LEDS //////////////////////////
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


/////////////////// LEDS STRIP //////////////////
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
    // Orange
    setLeds(strip.Color(255, 165, 0)); 
}


/////////////////// FAN /////////////////////////////////
// Pour controler le ventilateur selon la température
void ControlFan(float t, int ligth) {
  digitalWrite(2, LOW);
  if(t < SH){
    ledcWrite(FANPIN, 0);
    onMax = 0;
  }else{
    if(ligth < LIGHTMAX){
      if(onMax != 1){
        for(int i=1;i <= 255; i = i+10){
          ledcWrite(FANPIN, i);
        }
        onMax = 1;
      }else{
        ledcWrite(FANPIN, 255);
      }
    }else{
      digitalWrite(2,HIGH);
      ledcWrite(FANPIN, 0);
      onMax = 0; 
    }
  }
}


///////////////////// INITIALISATION /////////////////////
void initialisation(){
  Serial.begin(9600);

  // Init du capteur de température et du bus OneWire
  tempSensor.begin();

  //Init du leds strip
  strip.begin();
  strip.show();
   
  //Initialisation des leds du climatiseur / radiateur
  pinMode(CLIMPIN, OUTPUT);
  pinMode(RADPIN, OUTPUT);
  pinMode(2, OUTPUT);

  //Configuration du ventilateur 
  ledcAttach(FANPIN, 25000, 8);
}

void setup() {
  initialisation();
}


////////////////////// APEL DES FONCTIONS ///////////////////////
void appel(){
  // Lecture de la température
  float temperature = getTemperature();
  
  int light = 4095 - analogRead(A5);

  // Affichage dans le moniteur série
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Luminosite: ");
  Serial.println(light);
  ControlFan(temperature, light);
  ControlLedsStrip(temperature);
  ControlLeds(temperature);
}

void loop() {
  appel(); 
}
