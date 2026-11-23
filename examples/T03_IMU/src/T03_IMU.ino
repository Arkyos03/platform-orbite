/*
 * ORBITE-I - Test 3 : centrales inertielles BMI088 (SPI1) et LSM6DSOX (SPI2)
 * Carte immobile et à plat : environ 9,81 m/s² sur un seul axe, ~0 sur les
 * deux autres, gyroscopes proches de 0 °/s. Tourne la carte : les deux IMU
 * doivent donner les mêmes valeurs (au repère près).
 */
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();
  Serial.print("BMI088   : "); Serial.println(Orbite.bmi088OK() ? "OK" : "ABSENT");
  Serial.print("LSM6DSOX : "); Serial.println(Orbite.lsm6dsoxOK() ? "OK" : "ABSENT");
}

void loop() {
  static uint32_t last = 0;
  Orbite.update();
  if (millis() - last < 200) return;
  last = millis();
  float ax, ay, az, gx, gy, gz;
  Orbite.accel(ax, ay, az);
  Orbite.gyro(gx, gy, gz);
  Serial.print("BMI088   a = "); Serial.print(ax, 2); Serial.print(' '); Serial.print(ay, 2); Serial.print(' '); Serial.print(az, 2);
  Serial.print(" m/s2   g = "); Serial.print(gx, 1); Serial.print(' '); Serial.print(gy, 1); Serial.print(' '); Serial.print(gz, 1);
  Serial.print(" dps   |a| = "); Serial.print(Orbite.gForce(), 2); Serial.println(" g");
  Orbite.accelLSM(ax, ay, az);
  Orbite.gyroLSM(gx, gy, gz);
  Serial.print("LSM6DSOX a = "); Serial.print(ax, 2); Serial.print(' '); Serial.print(ay, 2); Serial.print(' '); Serial.print(az, 2);
  Serial.print(" m/s2   g = "); Serial.print(gx, 1); Serial.print(' '); Serial.print(gy, 1); Serial.print(' '); Serial.println(gz, 1);
}
