/*
 * ORBITE-I - Test 6 : carte micro SD (SDIO 4 bits, SD_DETECT)
 * Écrit 1000 lignes dans TEST_XXX.CSV, mesure le débit, puis relit le fichier.
 * Carte formatée en FAT32 (ou exFAT).
 */
#include <Orbite.h>
#include <STM32SD.h>

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();

  Serial.print("Carte detectee (SD_DETECT) : ");
  Serial.println(Orbite.sdCardPresent() ? "oui" : "NON");

  uint32_t t0 = millis();
  if (!Orbite.logBegin("TEST", "ligne;millis;valeur")) {
    Serial.println("ECHEC : impossible d'ouvrir un fichier (carte absente, non formatee ?)");
    Orbite.beepCount(5, 1500);
    return;
  }
  Serial.print("Fichier : "); Serial.println(Orbite.logFileName());
  for (int i = 0; i < 1000; i++) Orbite.log(i, (unsigned long)millis(), i * 0.001f);
  Orbite.logClose();
  uint32_t dt = millis() - t0;
  Serial.print("1000 lignes ecrites en "); Serial.print(dt); Serial.println(" ms");

  File f = SD.open(Orbite.logFileName());
  if (!f) { Serial.println("ECHEC : relecture impossible"); return; }
  Serial.print("Taille : "); Serial.print(f.size()); Serial.println(" octets");
  Serial.println("Debut du fichier :");
  for (int i = 0; i < 200 && f.available(); i++) Serial.write(f.read());
  f.close();
  Serial.println("\n=> carte SD OK");
  Orbite.beepCount(3);
}

void loop() {
  Orbite.update();
}
