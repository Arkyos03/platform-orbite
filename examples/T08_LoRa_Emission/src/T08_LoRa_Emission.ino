/*
 * ORBITE-I - Test 8 : émission LoRa (E22-900M22S, BEACON-I)
 * Envoie "ORBITE-I PING n" toutes les secondes à 869,525 MHz.
 * Pour recevoir : une deuxième carte ORBITE-I avec l'exemple T09_LoRa_Reception
 * (ou 07_Station_Sol pour la télémétrie complète).
 * ATTENTION : antenne 868 MHz branchée AVANT de mettre sous tension.
 */
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();
  if (!Orbite.loraBegin()) {
    Serial.println("ECHEC : le module LoRa ne repond pas (SPI3, CS_LORA, BUSY, RST ?)");
    Orbite.beepCount(5, 1500);
    while (true) Orbite.update();
  }
  Serial.print("LoRa OK, temps d'emission d'un paquet de 20 octets : ");
  Serial.print(Orbite.loraTimeOnAir(20)); Serial.println(" ms");
}

void loop() {
  static uint32_t last = 0, n = 0;
  Orbite.update();
  if (millis() - last < 1000) return;
  last = millis();
  char msg[32];
  int len = snprintf(msg, sizeof(msg), "ORBITE-I PING %lu", (unsigned long)n++);
  if (Orbite.loraSend((const uint8_t *)msg, len)) {
    Serial.print("Envoye : "); Serial.println(msg);
    Orbite.beep(4000, 20);
  } else {
    Serial.println("Module occupe");
  }
}
