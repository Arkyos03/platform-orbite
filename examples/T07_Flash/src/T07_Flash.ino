/*
 * ORBITE-I - Test 7 : mémoire flash W25Q128JV (CORE-I, SPI3)
 * Lit l'identifiant JEDEC (attendu : EF 40 18), efface le dernier secteur
 * de 4 Ko, y écrit 256 octets puis les relit.
 */
#include <Orbite.h>

static const SPISettings FLASH_SPI(20000000, MSBFIRST, SPI_MODE0);
static const uint32_t ADRESSE = 0xFFF000;   // dernier secteur de 4 Ko

static void flashCmd(const uint8_t *tx, uint16_t ntx, uint8_t *rx = nullptr, uint16_t nrx = 0) {
  OrbiteSPI3.beginTransaction(FLASH_SPI);
  digitalWrite(CS_FLASH, LOW);
  for (uint16_t i = 0; i < ntx; i++) OrbiteSPI3.transfer(tx[i]);
  for (uint16_t i = 0; i < nrx; i++) rx[i] = OrbiteSPI3.transfer(0x00);
  digitalWrite(CS_FLASH, HIGH);
  OrbiteSPI3.endTransaction();
}

static void flashWait() {
  uint8_t cmd = 0x05, st = 1;
  do { flashCmd(&cmd, 1, &st, 1); } while (st & 0x01);
}

static void flashWriteEnable() {
  uint8_t cmd = 0x06;
  flashCmd(&cmd, 1);
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();

  uint8_t id[3];
  uint8_t cmd = 0x9F;
  flashCmd(&cmd, 1, id, 3);
  Serial.print("JEDEC ID : ");
  Serial.print(id[0], HEX); Serial.print(' '); Serial.print(id[1], HEX); Serial.print(' '); Serial.println(id[2], HEX);
  if (id[0] != 0xEF || id[2] != 0x18) { Serial.println("ECHEC : W25Q128 non reconnue"); Orbite.beepCount(5, 1500); return; }

  flashWriteEnable();
  uint8_t erase[4] = {0x20, (uint8_t)(ADRESSE >> 16), (uint8_t)(ADRESSE >> 8), (uint8_t)ADRESSE};
  flashCmd(erase, 4);
  flashWait();

  uint8_t page[4 + 256] = {0x02, (uint8_t)(ADRESSE >> 16), (uint8_t)(ADRESSE >> 8), (uint8_t)ADRESSE};
  for (int i = 0; i < 256; i++) page[4 + i] = (uint8_t)(i ^ 0x5A);
  flashWriteEnable();
  flashCmd(page, sizeof(page));
  flashWait();

  uint8_t read[4] = {0x03, (uint8_t)(ADRESSE >> 16), (uint8_t)(ADRESSE >> 8), (uint8_t)ADRESSE};
  uint8_t data[256];
  flashCmd(read, 4, data, 256);
  int erreurs = 0;
  for (int i = 0; i < 256; i++) if (data[i] != (uint8_t)(i ^ 0x5A)) erreurs++;
  Serial.print("Ecriture / relecture : "); Serial.print(256 - erreurs); Serial.println(" / 256 octets corrects");
  if (erreurs == 0) { Serial.println("=> flash OK"); Orbite.beepCount(3); }
  else Orbite.beepCount(5, 1500);
}

void loop() {
  Orbite.update();
}
