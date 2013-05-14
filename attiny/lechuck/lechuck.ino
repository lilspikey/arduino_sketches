#include "charlieplex.h"

#define PIN8_MASK  (1)
#define PIN9_MASK  (1 << 1)
#define PIN10_MASK (1 << 2)
#define PIN11_MASK (1 << 3)

Charlieplex<4> charlie((int[]){ PIN8_MASK, PIN9_MASK, PIN10_MASK, PIN11_MASK });

int current;
int val;

void setup() {
  Serial.begin(9600);
  current = 0;
  val = 0;
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
  }
  charlie.display();
}
