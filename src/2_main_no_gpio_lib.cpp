/**
 *  Ultraschall Sensor HC-SR04 Example to learn the pulseIn functionality of Timer1
 *  
 *  Extend the basic example and use direct register configuration based on the datasheet.
 */

 #include <Arduino.h>

#define TRIGGER_PIN_DDR   DDRB
#define TRIGGER_PIN_PORT  PORTB
#define TRIGGER_PIN       PB1
 
#define ECHO_PIN_DDR      DDRB
#define ECHO_PIN_PORT     PORTB
#define ECHO_PIN          PB0   // ICP1 - Input Catpure Unit

const int echoPin = 8;

void setup() {
  // set trigger pin to output
  TRIGGER_PIN_DDR |= (1 << TRIGGER_PIN);

  // set echo pin to input (not required)
  ECHO_PIN_DDR &= ~(1 << ECHO_PIN);

  Serial.begin(115200);
}

void loop() {
  float duration, distance;

  // send 40kHz burst signal
  // set trigger to low
  TRIGGER_PIN_PORT &= ~(1 << TRIGGER_PIN);
  delayMicroseconds(2);
  
  // set trigger to high
  TRIGGER_PIN_PORT |= (1 << TRIGGER_PIN);
  delayMicroseconds(10);
  
  // set trigger back to low
  TRIGGER_PIN_PORT &= ~(1 << TRIGGER_PIN);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
}