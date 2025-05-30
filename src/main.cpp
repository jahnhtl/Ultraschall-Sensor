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

  // reset Timer1 due to used by Arduino Framework
  TCCR1A = 0x00;
  TCCR1B = 0x00;
  TIMSK1 = 0x00;

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
  
  // configure Timer1 for rising edge of puls
  TCCR1B |= (1 << ICES1);       // Input Capture Edge Selection - Rising Edge
  TCCR1B |= (1 << CS11);        // Start Timer1 with prescaler = 8 (CHECK IF REALLY NEEDED!!)
 
  // Loop till rising edge detected
  while ((TIFR1 & (1 << ICF1)) == LOW);
  TIFR1 |= (1 << ICF1);         // Clear Flag
  TCCR1B &= (1 << CS11);        // Stop Timer 1 (CHECK IF REALLY NEEDED!!)
   
  // configuter Timer1 for falling edge of puls and overflow
  TCNT1 = 18890;                // Set Timer1 to 18890 -> timeout occurs if distance > 4m
  TCCR1B &= (1 << ICES1);       // Change Input Capture Edge Selection - Falling Edge
  TCCR1B |= (1 << CS11);        // Start Timer1 with prescaler = 8 (CHECK IF REALLY NEEDED!!)
 
  // Loop till falling edge detected and no overflow occured
  while ((TIFR1 & (1 << ICF1)) == LOW && (TIFR1 & (1 << TOV1)) == LOW);
  TIFR1 |= (1 << ICF1);         // Clear Flag
  TCCR1B &= (1 << CS11);        // Stop Timer 1
 
  // Take Input Capture Value from Timer -> to get duration in microseconds shift right needed due to prescaler of 8
  duration = (ICR1 - 18890) >> 1;   // shift right by 1 = divided by 2

  if ((TIFR1 & (1 << TOV1)) == LOW) {
    distance = ((float) duration * .0343) / 2;
    Serial.print("Distance: ");
    Serial.println(distance);
  }
  else {  // overflow occured
    TIFR1 |= (1 << TOV1);           // reset overflow flag
    Serial.println("Distance: ---");
  }

  delay(100);
}