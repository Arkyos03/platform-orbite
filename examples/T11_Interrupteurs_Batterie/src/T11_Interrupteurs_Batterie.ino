/*
 * ORBITE-I - Test 11 : interrupteurs (SPARK-I) et mesures de tension
 * Affiche l'état de SW_LAUNCH, SW_PIN (goupille), SW_AUX, la tension batterie
 * et celle du rail pyro. Un bip à chaque changement d'état d'un interrupteur.
 * Compare les tensions avec un multimètre (écart attendu < 0,2 V).
 */
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();
}

void loop() {
  static uint32_t last = 0;
  static uint8_t prev = 0xFF;
  Orbite.update();

  uint8_t etat = Orbite.launchSwitch() | (Orbite.pinRemoved() << 1) | (Orbite.auxSwitch() << 2);
  if (etat != prev) { Orbite.beep(3000, 40); prev = etat; last = 0; }

  if (millis() - last < 500) return;
  last = millis();
  Serial.print("SW_LAUNCH : "); Serial.print(Orbite.launchSwitch() ? "ferme " : "ouvert");
  Serial.print("   goupille : "); Serial.print(Orbite.pinRemoved() ? "RETIREE " : "en place");
  Serial.print("   SW_AUX : "); Serial.print(Orbite.auxSwitch() ? "ferme " : "ouvert");
  Serial.print("   batterie : "); Serial.print(Orbite.batteryVoltage(), 2);
  Serial.print(" V   rail pyro : "); Serial.print(Orbite.pyroVoltage(), 2);
  Serial.println(Orbite.pyroArmed() ? " V (ARME)" : " V");
}
