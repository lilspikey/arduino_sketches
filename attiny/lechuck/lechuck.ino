#include "charlieplex.h"

#define PIN8_MASK  (1)
#define PIN9_MASK  (1 << 1)
#define PIN10_MASK (1 << 2)

Charlieplex<3> charlie((int[]){ PIN8_MASK, PIN9_MASK, PIN10_MASK });

int current;
int val;

void setup() {
  Serial.begin(9600);
  current = 0;
  val = 0;
  //pinMode(8, OUTPUT);
  //pinMode(9, OUTPUT);
}

void loop() {
  charlie.pin(current, HIGH);
  val++;
  if ( val >= 20000 ) {
    current++;
    val = 0;
  }
  if ( current >= charlie.size() ) {
    current = 0;
    charlie.clear();
    //Serial.println(current);
  }
  //Serial.println(charlie.size());
  charlie.display();
  //delay(250);
  //DDRB = DDRB | PIN8_MASK | PIN9_MASK;
  //PORTB = (PORTB & ~PIN8_MASK) | PIN9_MASK;
  //digitalWrite(9, LOW);
  //digitalWrite(8, HIGH);
  /*Serial.println(PORTB);
  
  Serial.println(PIN8_MASK);
  Serial.println(PIN9_MASK);
  Serial.println();*/
}
