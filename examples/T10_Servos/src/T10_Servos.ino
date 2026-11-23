/*
 * ORBITE-I - Test 10 : servos (REACTOR-I, 6 V, TIM4 50 Hz)
 * Les 4 servos vont à 0°, 90° puis 180°, et reviennent au milieu.
 * Ensuite, tape un angle (0 à 180) dans le moniteur série : les 4 servos y vont.
 */
#include <Orbite.h>

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Orbite.begin();
  const int angles[] = {0, 90, 180, 90};
  for (int a : angles) {
    Serial.print("Angle "); Serial.println(a);
    for (uint8_t n = 1; n <= 4; n++) Orbite.servoWrite(n, a);
    delay(1000);
  }
  Serial.println("Tape un angle (0..180) :");
}

void loop() {
  Orbite.update();
  if (Serial.available()) {
    int a = Serial.parseInt();
    if (a >= 0 && a <= 180) {
      for (uint8_t n = 1; n <= 4; n++) Orbite.servoWrite(n, a);
      Serial.print("-> "); Serial.println(a);
    }
    while (Serial.available()) Serial.read();
  }
}
