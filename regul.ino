#include "utils.h"
#include "utilsJSON.h"

void setup() {
  Serial.begin(9600);

  // init du capteur et de OneWire
  tempSensor.begin();

  initLeds();

  initFan();

  esp = (esp_model *)malloc(sizeof(esp_model));

}

void loop() {
  temperature = getTemperature();
  luminosity = 4095 - analogRead(A5);
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.println(luminosity);
  
  checkStatus(temperature, luminosity);
  controlSensors();

  updateValues(esp, luminosity, temperature, fanSpeed, coolerState, heaterState, fireDetected);

  StaticJsonDocument<1000> statusJSON = makeJSON_fromStatus(esp);
  serializeJsonPretty(statusJSON, Serial);
  Serial.println();

  delay(1000);
}
