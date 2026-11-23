/*
 * ORBITE-I - Test 12 : tir d'une voie pyro (banc d'essai)
 *
 *   !!! DANGER : ce programme DECLENCHE les charges. A utiliser uniquement
 *   !!! avec une ampoule, une LED + résistance ou un inflammateur SEUL (sans
 *   !!! poudre), loin de toute personne. Jamais dans la fusée assemblée.
 *
 * Dans le moniteur série, tape exactement "FEU 1" (ou 2, 3, 4) puis Entrée.
 * La voie n'est tirée que si le rail est armé (goupille retirée) et qu'il y a
 * continuité. Compte à rebours de 5 s avec bips, annulable en tapant un caractère.
 */
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();
  Serial.println("Banc pyro : tape \"FEU n\" (n = 1..4)");
}

static bool compteARebours() {
  for (int s = 5; s > 0; s--) {
    Serial.print(s); Serial.println("...  (tape un caractere pour annuler)");
    Orbite.beep(4000, 100);
    uint32_t t0 = millis();
    while (millis() - t0 < 1000) {
      Orbite.update();
      if (Serial.available()) { while (Serial.available()) Serial.read(); return false; }
    }
  }
  return true;
}

void loop() {
  Orbite.update();
  if (!Serial.available()) return;
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  if (!cmd.startsWith("FEU ")) { Serial.println("Commande inconnue"); return; }
  int n = cmd.substring(4).toInt();
  if (n < 1 || n > 4) { Serial.println("Voie 1 a 4"); return; }

  Serial.print("Rail pyro : "); Serial.print(Orbite.pyroVoltage(), 2); Serial.println(" V");
  if (!Orbite.pyroArmed()) { Serial.println("REFUS : rail desarme (goupille en place)"); return; }
  if (!Orbite.continuity(n)) { Serial.println("REFUS : pas de continuite sur cette voie"); return; }
  if (!compteARebours()) { Serial.println("ANNULE"); return; }

  Serial.print("FEU voie "); Serial.println(n);
  Orbite.fire(n, 1000);
  uint32_t t0 = millis();
  while (millis() - t0 < 1500) Orbite.update();
  Serial.print("Continuite apres tir : ");
  Serial.println(Orbite.continuity(n) ? "toujours presente (inflammateur intact ?)" : "ouverte (inflammateur grille)");
}
