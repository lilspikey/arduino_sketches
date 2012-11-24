/**
  Xmas decoration - with help from:
  
  http://www.instructables.com/id/Fully-functional-Duplo-traffic-light/step2/Programming-the-microcontroller/
  http://hlt.media.mit.edu/?p=1706
  http://www.arduino.cc/en/Tutorial/PlayMelody
  http://code.google.com/p/arduino-tiny/

**/

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "notes.h"

//#define DEBUG


#ifdef __AVR_ATtiny85__
#define SWITCH_PIN 0
#define LED_PIN 1
#define SPEAKER_PIN 2
#else
#define SWITCH_PIN 2 // (interrupt 0)
#define LED_PIN 3
#define SPEAKER_PIN 4
#endif

#define BPM 100l
#define NOTES_LEN 4

prog_uint16_t notes[2*NOTES_LEN] PROGMEM = {
  NOTE_F4, 1,
  NOTE_G4, 1,
  NOTE_A4, 1,
  NOTE_F4, 1
};


void playSong() {
  long beatDurationMillis = (60l*1000l)/BPM;
  for ( int i = 0; i < NOTES_LEN; i++ ) {
    int note = i << 1;
    long freq = pgm_read_word_near(notes + note);
    long duration = pgm_read_word_near(notes + note + 1);
#ifdef DEBUG
    Serial.print("freq: ");
    Serial.println(freq);
    Serial.print(", duration: ");
    Serial.println(duration);
#endif
    tone(SPEAKER_PIN, freq);
    digitalWrite(LED_PIN, HIGH);
    delay(3*duration*beatDurationMillis/4);
    digitalWrite(LED_PIN, LOW);
    delay(duration*beatDurationMillis/4);
  }
  noTone(SPEAKER_PIN);
}

void setup() {
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);

  ADCSRA &= ~(1<<ADEN); //turn off ADC
  ACSR |= _BV(ACD);     //disable the analog comparator
  
#ifndef DEBUG
  Serial.begin(9600);
#endif
}

void gotoSleep() {
#ifdef __AVR_ATtiny85__
  GIMSK |= 1<<PCIE;  //Enable Pin Change Interrupt
  PCMSK |= 1<<PCINT0; //Watch for Pin Change on Pin5 (PB0)
#else
  attachInterrupt(0, interrup, FALLING);
#endif
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
      
  // waking up from sleep mode.
  sleep_disable();

#ifdef __AVR_ATtiny85__
  GIMSK &= ~(1<<PCIE); //Disable the interrupt so it doesn't keep flagging
  PCMSK &= ~(1<<PCINT0);
#else
  detachInterrupt(0);
#endif
}

void loop() {
  playSong();
  delay(200);
  digitalWrite(LED_PIN, LOW);
  
  gotoSleep();
      
  // Wait for the button to be released
  while (digitalRead(SWITCH_PIN) == LOW) {  }
}

void interrup() {}

// Interrupt for PIN0 falling edge
ISR(PCINT0_vect) {
  
}

