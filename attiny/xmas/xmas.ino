/**
  Xmas decoration - with help from:
  
  http://www.instructables.com/id/Fully-functional-Duplo-traffic-light/step2/Programming-the-microcontroller/
  http://hlt.media.mit.edu/?p=1706
  http://www.arduino.cc/en/Tutorial/PlayMelody
  
**/

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#define SWITCH_PIN 0
#define LED_PIN 1
#define SPEAKER_PIN 2

#define  C     3830    // 261 Hz 
#define  D     3400    // 294 Hz 
#define  E     3038    // 329 Hz 
#define  F     2864    // 349 Hz 
#define  G     2550    // 392 Hz 
#define  A     2272    // 440 Hz 
#define  B     2028    // 493 Hz 
#define  C2    1912    // 523 Hz 
// Define a special note, 'R', to represent a rest
#define  R     0

int melody[] = {  C2,  B,  G,  C2,  B,  E,  C2,  C,  G, A, C2 };
int beats[]  = { 16, 16, 16,  8,  8,  16, 32, 16, 16, 16, 8, 8 }; 
int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.

// Set overall tempo
long tempo = 10000;
// Set length of pause between notes
int pause = 1000;
// Loop variable to increase Rest length
int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES

// Initialize core variables
int tone_ = 0;
int beat = 0;
long duration  = 0;

// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(SPEAKER_PIN,HIGH);
      delayMicroseconds(tone_ / 2);

      // DOWN
      digitalWrite(SPEAKER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }                                 
}

void setup() {
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);

  ADCSRA &= ~(1<<ADEN); //turn off ADC
  ACSR |= _BV(ACD);     //disable the analog comparator
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

void playSong() {
  for (int i=0; i<MAX_COUNT; i++) {
    tone_ = melody[i];
    beat = beats[i];

    duration = beat * tempo; // Set up timing

    playTone();
    // A pause between notes...
    delayMicroseconds(pause);
  }
}

void loop() {
  playSong();
  
  gotoSleep();
      
  // Wait for the button to be released
  while (digitalRead(SWITCH_PIN) == LOW) {  }
}

// Interrupt for PIN0 falling edge
ISR(PCINT0_vect) {
  
}
