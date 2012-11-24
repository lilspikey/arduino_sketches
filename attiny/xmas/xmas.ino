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
#define NOTES_LEN 46

// transcribed via http://www.songsterr.com/a/wsa/theme-songs-futurama-theme-intro-arranged-for-guitar-tab-s23258t0

prog_uint16_t notes[2*NOTES_LEN] PROGMEM = {
  /* bar 1 */
  NOTE_E3, BEAT,
  NOTE_E3, BEAT,
  NOTE_REST, HALF_BEAT,
  NOTE_A3, HALF_BEAT,
  NOTE_A3, HALF_BEAT,
  NOTE_REST, HALF_BEAT,
  /* bar 2 */
  NOTE_D3, BEAT,
  NOTE_D3, BEAT,
  NOTE_REST, HALF_BEAT,
  NOTE_E3, HALF_BEAT,
  NOTE_E3, HALF_BEAT,
  NOTE_REST, HALF_BEAT,
  /* bar 3 */
  NOTE_E3, BEAT,
  NOTE_E3, BEAT,
  NOTE_REST, HALF_BEAT,
  NOTE_A3, HALF_BEAT,
  NOTE_A3, HALF_BEAT,
  NOTE_REST, HALF_BEAT,
  /* bar 4 */
  NOTE_D3, BEAT,
  NOTE_D3, BEAT,
  NOTE_REST, HALF_BEAT,
  NOTE_G3, HALF_BEAT,
  NOTE_G3, HALF_BEAT,
  NOTE_Gb3, HALF_BEAT,
  /* bar 5 */
  NOTE_E3, BEAT,
  NOTE_REST, HALF_BEAT,
  NOTE_E3, HALF_BEAT,
  NOTE_A3, BEAT,
  NOTE_Ab3, BEAT,
  /* bar 6 */
  NOTE_D3, BEAT,
  NOTE_REST, HALF_BEAT,
  NOTE_D3, HALF_BEAT,
  NOTE_Gb3, BEAT,
  NOTE_E3, BEAT,
  /* bar 7 */
  NOTE_E3, BEAT,
  NOTE_REST, HALF_BEAT,
  NOTE_E3, HALF_BEAT,
  NOTE_A3, BEAT,
  NOTE_Ab3, BEAT,
  /* bar 8 */
  NOTE_B3, BEAT,
  NOTE_REST, HALF_BEAT,
  NOTE_B3, HALF_BEAT,
  NOTE_G3, HALF_BEAT,
  NOTE_G3, HALF_BEAT,
  NOTE_Gb3, HALF_BEAT,
  NOTE_Gb3, HALF_BEAT
};


void playSong() {
  long beatDurationMillis = (60l*1000l)/(BPM*BEAT);
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
    if ( freq == NOTE_REST ) {
      noTone(SPEAKER_PIN);
      delay(duration*beatDurationMillis);
    }
    else {
      tone(SPEAKER_PIN, freq);
      digitalWrite(LED_PIN, HIGH);
      delay(4*duration*beatDurationMillis/5);
      noTone(SPEAKER_PIN);
      digitalWrite(LED_PIN, LOW);
      delay(duration*beatDurationMillis/5);
    }
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

