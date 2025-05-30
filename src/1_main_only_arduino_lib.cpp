/**
 *  Ultraschall Sensor HC-SR04 Example to learn the pulseIn functionality of Timer1
 *  
 *  Extend the basic example and use direct register configuration based on the datasheet.
 */

#include <Arduino.h>

const int triggerPin = 9;
const int echoPin = 8;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  float duration, distance;

  // send 40kHz burst signal
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
}