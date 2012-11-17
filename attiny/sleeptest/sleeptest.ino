/**
 * Simple sketch to test how little power we can use when
 * ATTiny goes to sleep.
 * Most of the extra saving seems to be due to turning off ADC
 **/

#include <avr/power.h>
#include <avr/sleep.h>

#define SWITCH_PIN 0
#define LED_PIN 1

void setup() {
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);
  
  pinMode(LED_PIN, OUTPUT);
  
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
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  
  gotoSleep();
  
  // Wait for the button to be released
  while (digitalRead(SWITCH_PIN) == LOW) {  }
}

// Interrupt for PIN0 falling edge
ISR(PCINT0_vect) {
  
}
