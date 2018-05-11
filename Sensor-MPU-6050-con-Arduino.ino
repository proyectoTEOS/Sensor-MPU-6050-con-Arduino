/*
  Created by TEOS
  Domotic with Arduino https://goo.gl/btZpjs
  YouTube https://goo.gl/k7TUSZ
  Instagram https://goo.gl/zEIjia
  Facebook https://goo.gl/eivraR
  Twitter https://goo.gl/du5Wgn
  Github https://goo.gl/Xl5IiS
  Google Plus https://goo.gl/gBnUdh
  WEB https://goo.gl/jtiYJy
*/

#include <Wire.h>

const int signalPathRT = 0x68;

const float accelerationRT = 16384.0;
const float turnRT = 131.0;
int16_t acXT, acYT, acZT, gyXT, gyYT, gyZT;
float differentialT;

float accelerationArrayT[2];
float gyroscopeArrayT[3];
float angleArrayT[3];

String resulSerialT;

long previousMillisT;

void setup() {
  Wire.begin();
  Wire.beginTransmission(signalPathRT);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(115200);
}

void loop() {
  Wire.beginTransmission(signalPathRT);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(signalPathRT, 6, true);
  acXT = Wire.read() << 8 | Wire.read();
  acYT = Wire.read() << 8 | Wire.read();
  acZT = Wire.read() << 8 | Wire.read();

  accelerationArrayT[1] = atan(-1 * (acXT / accelerationRT) / sqrt(pow((acYT / accelerationRT), 2) + pow((acZT / accelerationRT), 2))) * RAD_TO_DEG;
  accelerationArrayT[0] = atan((acYT / accelerationRT) / sqrt(pow((acXT / accelerationRT), 2) + pow((acZT / accelerationRT), 2))) * RAD_TO_DEG;

  Wire.beginTransmission(signalPathRT);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(signalPathRT, 6, true);
  gyXT = Wire.read() << 8 | Wire.read();
  gyYT = Wire.read() << 8 | Wire.read();
  gyZT = Wire.read() << 8 | Wire.read();

  gyroscopeArrayT[0] = gyXT / turnRT;
  gyroscopeArrayT[1] = gyYT / turnRT;

  differentialT = (millis() - previousMillisT) / 1000.0;
  previousMillisT = millis();

  angleArrayT[0] = 0.98 * (angleArrayT[0] + gyroscopeArrayT[0] * differentialT) + 0.02 * accelerationArrayT[0];
  angleArrayT[1] = 0.98 * (angleArrayT[1] + gyroscopeArrayT[1] * differentialT) + 0.02 * accelerationArrayT[1];

  resulSerialT = "Grados de inclinacion en X: " + String(angleArrayT[0]) + " | " + "Grados de inclinacion en Y" + String(angleArrayT[1]);
  Serial.println(resulSerialT);
  delay(100);
}
