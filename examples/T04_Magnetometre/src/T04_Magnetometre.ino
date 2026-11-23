/*
 * ORBITE-I - Test 4 : magnétomètre MMC5983MA (BEACON-I, SPI2)
 * Carte à plat : le cap change quand tu la fais tourner (pas de calibration,
 * la valeur absolue peut être décalée). Champ terrestre en France : ~0,47 gauss.
 * Approche un aimant : les valeurs doivent bondir.
 */
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();
  Serial.print("MMC5983MA : "); Serial.println(Orbite.magOK() ? "OK" : "ABSENT");
}

void loop() {
  static uint32_t last = 0;
  Orbite.update();
  if (millis() - last < 250) return;
  last = millis();
  float x, y, z;
  Orbite.mag(x, y, z);
  Serial.print("B = "); Serial.print(x, 3); Serial.print(' '); Serial.print(y, 3); Serial.print(' '); Serial.print(z, 3);
  Serial.print(" G   |B| = "); Serial.print(sqrtf(x * x + y * y + z * z), 3);
  Serial.print(" G   cap = "); Serial.print(Orbite.heading(), 0); Serial.println(" deg");
}
