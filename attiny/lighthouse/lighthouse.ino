/**
 * Simple sketch to test how little power we can use when
 * ATTiny goes to sleep.
 * Most of the extra saving seems to be due to turning off ADC
 **/

#include <avr/power.h>
#include <avr/sleep.h>

#ifdef __AVR_ATtiny85__
#define SWITCH_PIN 0
#define LED_PIN 1
#else
#define SWITCH_PIN 2 // (interrupt 0)
#define LED_PIN 9
#endif

#define PULSE_DURATION 1000
#define SIN_LEN 27

prog_uchar sin_table[SIN_LEN] PROGMEM = { 0, 30, 61, 90, 118, 144, 169, 190, 209, 225, 238, 247, 253, 255, 253, 247, 238, 225, 209, 190, 169, 144, 118, 90, 61, 30, 0 };

void setup() {
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);
  
  pinMode(LED_PIN, OUTPUT);
  
  ADCSRA &= ~(1<<ADEN);                     //turn off ADC
  ACSR |= _BV(ACD);                         //disable the analog comparator
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

void pulse() {
  long start = millis();
  long current = -1;
  while ( (current = millis()) - start < PULSE_DURATION ) {
    int sin_index = (int)(SIN_LEN*(current - start)/PULSE_DURATION);
    int pwm = 0xFF & pgm_read_byte_near(sin_table + sin_index);
    analogWrite(LED_PIN, pwm);
    delay(10);
  }
  
  // ensure fully turned off
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  gotoSleep();
  
  // Wait for the button to be released
  while (digitalRead(SWITCH_PIN) == LOW) {  }
  
  for ( int i = 0; i < 2; i++ ) {
    pulse();
    delay(250);
  }
}

void interrup() {}

// Interrupt for PIN0 falling edge
ISR(PCINT0_vect) {
  
}
