/*
 * ORBITE-I - Test 1 : buzzer (SPARK-I) et LED RGB (BEACON-I)
 * Résultat attendu : 3 bips de hauteurs différentes, puis la LED passe
 * rouge, vert, bleu, blanc et s'éteint. Recommence toutes les 5 s.
 */
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  Orbite.begin();
  Orbite.statusLed(false);          // la LED est pilotée par ce programme
}

void loop() {
  Serial.println("Buzzer : 2 kHz, 4 kHz, 6 kHz");
  Orbite.beepBlocking(2000, 200); delay(150);
  Orbite.beepBlocking(4000, 200); delay(150);
  Orbite.beepBlocking(6000, 200); delay(300);

  const uint32_t couleurs[] = {0x400000, 0x004000, 0x000040, 0x303030, 0x000000};
  const char *noms[] = {"rouge", "vert", "bleu", "blanc", "eteinte"};
  for (uint8_t i = 0; i < 5; i++) {
    Serial.print("LED : "); Serial.println(noms[i]);
    Orbite.rgb(couleurs[i]);
    delay(600);
  }
  delay(2000);
}
