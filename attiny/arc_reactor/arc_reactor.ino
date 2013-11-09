/**
 * Simple sketch to test how little power we can use when
 * ATTiny goes to sleep.
 * Most of the extra saving seems to be due to turning off ADC
 **/

#include <avr/power.h>
#include <avr/sleep.h>

#ifdef __AVR_ATtiny85__
#define SWITCH_PIN 0
#define LED1_PIN 2
#define LED2_PIN 3
#define LED3_PIN 4
#else
#define SWITCH_PIN 2 // (interrupt 0)
#define LED_PIN 3
#endif

#define CYCLES 8

prog_uint16_t cycle_delays[CYCLES] PROGMEM = {
  750, 500, 250, 200, 150, 50, 10, 10
};

void setup() {
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);
  
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  
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

void cycle(int delayMillis) {
  for ( int pin = LED1_PIN; pin <= LED3_PIN; pin++ ) {
    digitalWrite(LED1_PIN, pin == LED1_PIN? HIGH : LOW);
    digitalWrite(LED2_PIN, pin == LED2_PIN? HIGH : LOW);
    digitalWrite(LED3_PIN, pin == LED3_PIN? HIGH : LOW);
    delay(delayMillis);
  }
}

void all(uint8_t state) {
  digitalWrite(LED1_PIN, state);
  digitalWrite(LED2_PIN, state);
  digitalWrite(LED3_PIN, state);
}

void loop() {
  gotoSleep();
  
  // Wait for the button to be released
  while (digitalRead(SWITCH_PIN) == LOW) {  }
  
  for ( int i = 0; i < CYCLES; i++ ) {
    int delayMillis = 0xFFFF & pgm_read_word_near(cycle_delays + i);
    cycle(delayMillis);
  }
  
  all(HIGH);
  delay(5000);
  
  for ( int i = CYCLES-1; i >= 0; i-- ) {
    int delayMillis = 0xFFFF & pgm_read_word_near(cycle_delays + i);
    cycle(delayMillis);
  }
  
  all(LOW);
}

void interrup() {}

// Interrupt for PIN0 falling edge
ISR(PCINT0_vect) {
  
}
