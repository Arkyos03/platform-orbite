/*
 * ORBITE-I - Test 9 : réception LoRa
 * À mettre sur la deuxième carte, en face de T08_LoRa_Emission.
 * Affiche chaque message reçu avec la puissance (RSSI) et le rapport signal/bruit.
 */
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();
  if (!Orbite.loraBegin()) {
    Serial.println("ECHEC : le module LoRa ne repond pas");
    while (true) Orbite.update();
  }
  Orbite.loraListen();
  Serial.println("En ecoute sur 869,525 MHz...");
}

void loop() {
  Orbite.update();
  if (Orbite.loraAvailable()) {
    uint8_t buf[64];
    uint8_t n = Orbite.loraRead(buf, sizeof(buf) - 1);
    buf[n] = 0;
    Serial.print("Recu : "); Serial.print((char *)buf);
    Serial.print("   RSSI "); Serial.print(Orbite.loraRSSI(), 1);
    Serial.print(" dBm   SNR "); Serial.print(Orbite.loraSNR(), 1); Serial.println(" dB");
    Orbite.beep(3000, 20);
  }
}
