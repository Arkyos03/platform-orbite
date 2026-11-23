/*
 * ORBITE-I - Test 5 : GPS MAX-M10S (BEACON-I, Serial2) + antenne active
 * Dehors ou près d'une fenêtre. Premier fix : 30 s à quelques minutes.
 * Mets TRAMES_BRUTES à true pour voir les trames NMEA telles quelles
 * (utile si rien ne s'affiche : vérifie le câblage TX/RX et l'antenne).
 */
#include <Orbite.h>

const bool TRAMES_BRUTES = false;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  if (TRAMES_BRUTES) {
    Serial2.begin(GPS_BAUD);
  } else {
    Orbite.begin();
  }
}

void loop() {
  if (TRAMES_BRUTES) {
    while (Serial2.available()) Serial.write(Serial2.read());
    return;
  }
  static uint32_t last = 0;
  Orbite.update();
  if (millis() - last < 1000) return;
  last = millis();
  Serial.print("UTC "); Serial.print(Orbite.gpsTime());
  Serial.print("  fix : "); Serial.print(Orbite.gpsFix() ? "OUI" : "non");
  Serial.print("  satellites : "); Serial.print(Orbite.gpsSatellites());
  Serial.print("  HDOP : "); Serial.print(Orbite.gpsHdop(), 1);
  if (Orbite.gpsFix()) {
    Serial.print("  position : "); Serial.print(Orbite.gpsLatitude(), 6);
    Serial.print(", "); Serial.print(Orbite.gpsLongitude(), 6);
    Serial.print("  alt : "); Serial.print(Orbite.gpsAltitude(), 1); Serial.print(" m");
    Serial.print("  (il y a "); Serial.print(Orbite.gpsAge()); Serial.print(" ms)");
  }
  Serial.println();
}
