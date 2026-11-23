/*
 * ORBITE-I - Test 2 : baromètres MS5611 (SPI1) et BMP390 (SPI2)
 * Affiche la pression et l'altitude de chaque capteur. Les deux altitudes
 * doivent rester à quelques dizaines de cm l'une de l'autre et proches de 0
 * (le sol est pris au démarrage). Monte la carte d'un étage : environ +3 m.
 * Le MS5611 est sensible à la lumière : mets-le à l'ombre (mousse).
 */
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();
  Serial.print("MS5611 : "); Serial.println(Orbite.ms5611OK() ? "OK" : "ABSENT");
  Serial.print("BMP390 : "); Serial.println(Orbite.bmp390OK() ? "OK" : "ABSENT");
  Serial.println("pression_Pa;temperature_C;alt_MS5611_m;alt_BMP390_m;altitude_m;vitesse_m/s");
}

void loop() {
  static uint32_t last = 0;
  Orbite.update();
  if (millis() - last < 200) return;
  last = millis();
  Serial.print(Orbite.pressure(), 1);       Serial.print(';');
  Serial.print(Orbite.temperature(), 2);    Serial.print(';');
  Serial.print(Orbite.altitudeMS5611(), 2); Serial.print(';');
  Serial.print(Orbite.altitudeBMP390(), 2); Serial.print(';');
  Serial.print(Orbite.altitude(), 2);       Serial.print(';');
  Serial.println(Orbite.verticalSpeed(), 2);
}
