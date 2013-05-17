#include "charlieplex.h"
#include "frames.h"

#define PIN8_MASK  (1)
#define PIN9_MASK  (1 << 1)
#define PIN10_MASK (1 << 2)
#define PIN11_MASK (1 << 3)

Charlieplex<4> charlie((int[]){ PIN8_MASK, PIN9_MASK, PIN10_MASK, PIN11_MASK });

int current_frame = 0;
unsigned long prev_millis = 0;

void setup() {
  //Serial.begin(9600);
  current_frame = 0;
  charlie.clear();
  prev_millis = millis();
}

void loop() {
  unsigned long time = millis();
  if ( (time - prev_millis) > 40 ) {
    unsigned int frame = pgm_read_word_near(FRAMES + current_frame);
    int pins = charlie.size();
    for ( int i = 0; i < pins; i++ ) {
      charlie.pin(i, (1 << i) & frame? HIGH : LOW);
    }
    current_frame++;
    if ( current_frame >= NUM_FRAMES )  {
      current_frame = 0; 
    }
    prev_millis = time;
  }
  charlie.display();
}
