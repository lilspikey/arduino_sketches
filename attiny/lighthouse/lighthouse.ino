/**
 * When button pressed LED will pulse twice, then goes back to sleep
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

#define SIN_LEN 201

prog_uchar sin_table[SIN_LEN] PROGMEM = {
  0, 4, 8, 12, 16, 20, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63, 67, 71, 74, 78,
  82, 86, 90, 93, 97, 101, 104, 108, 112, 115, 119, 122, 126, 129, 133, 136, 140,
  143, 146, 149, 153, 156, 159, 162, 165, 168, 171, 174, 177, 180, 183, 185, 188,
  191, 193, 196, 199, 201, 203, 206, 208, 210, 213, 215, 217, 219, 221, 223, 225,
  227, 228, 230, 232, 234, 235, 237, 238, 239, 241, 242, 243, 244, 245, 246, 247,
  248, 249, 250, 251, 251, 252, 252, 253, 253, 254, 254, 254, 254, 254, 255, 254,
  254, 254, 254, 254, 253, 253, 252, 252, 251, 251, 250, 249, 248, 247, 246, 245,
  244, 243, 242, 241, 239, 238, 237, 235, 234, 232, 230, 228, 227, 225, 223, 221,
  219, 217, 215, 213, 210, 208, 206, 203, 201, 199, 196, 193, 191, 188, 185, 183,
  180, 177, 174, 171, 168, 165, 162, 159, 156, 153, 149, 146, 143, 140, 136, 133,
  129, 126, 122, 119, 115, 112, 108, 104, 101, 97, 93, 90, 86, 82, 78, 74, 71, 67,
  63, 59, 55, 51, 47, 43, 39, 35, 31, 27, 23, 20, 16, 12, 8, 4, 0
};

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

void pulse(long duration) {
  long start = millis();
  long current = -1;
  while ( (current = millis()) - start < duration ) {
    int sin_index = (int)((SIN_LEN*(current - start))/duration);
    int pwm = 0xFF & pgm_read_byte_near(sin_table + sin_index);
    analogWrite(LED_PIN, pwm);
    delay(5);
  }
  
  // ensure fully turned off
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  gotoSleep();
  
  // Wait for the button to be released
  while (digitalRead(SWITCH_PIN) == LOW) {  }
  
  pulse(3000);
  delay(1000);
  pulse(7000);
  delay(1000);
  pulse(3000);
}

void interrup() {}

// Interrupt for PIN0 falling edge
ISR(PCINT0_vect) {
  
}
