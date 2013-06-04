#include <avr/power.h>
#include <avr/sleep.h>

#include "charlieplex.h"
#include "frames.h"

#define CHARLIE1_MASK (1 << 1)
#define CHARLIE2_MASK (1 << 2)
#define CHARLIE3_MASK (1 << 3) 
#define CHARLIE4_MASK (1 << 4)

#define SWITCH_PIN 0

Charlieplex<4> charlie((int[]){ CHARLIE1_MASK, CHARLIE2_MASK, CHARLIE3_MASK, CHARLIE4_MASK });

int current_frame = 0;
unsigned long prev_millis = 0;
boolean readyToSleep = false;

void setup() {
  current_frame = 0;
  charlie.clear();
  readyToSleep = true;
  
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);
  
  ADCSRA &= ~(1<<ADEN);                     //turn off ADC
  ACSR |= _BV(ACD);                         //disable the analog comparator
}

void gotoSleep() {
  GIMSK |= 1<<PCIE;  //Enable Pin Change Interrupt
  PCMSK |= 1<<PCINT0; //Watch for Pin Change on Pin5 (PB0)
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
      
  // waking up from sleep mode.
  sleep_disable();

  GIMSK &= ~(1<<PCIE); //Disable the interrupt so it doesn't keep flagging
  PCMSK &= ~(1<<PCINT0);
}

void loop() {
  if ( readyToSleep ) {
    charlie.clear();
    charlie.display();
    
    gotoSleep();
    // Wait for the button to be released
    while (digitalRead(SWITCH_PIN) == LOW) {  }
    
    readyToSleep = false;
    prev_millis = millis();
  }
      
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
      readyToSleep = true;
    }
    prev_millis = time;
  }
  charlie.display();
}

// Interrupt for PIN0 falling edge
ISR(PCINT0_vect) {
  
}

