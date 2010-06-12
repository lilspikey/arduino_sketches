/*

Crude "theremin" of sorts.

Push button on digital pin 12 toggles sound on and off.

Potentiometer on analog pin 4 controls tone (slides up and down scale).

Buzzer on digital pin 4 generates sound.

*/

#define POT_PIN 4
#define PUSH_PIN 12
#define LED_PIN 13
#define BUZZER_PIN 4

#define SCALE_LEN 12

int scale[SCALE_LEN] = {
  261, /* C */
  272, /* C# */
  294, /* D */
  313, /* Eb */
  327, /* E */
  348, /* F */
  367, /* F# */
  392, /* G */
  418, /* Ab */
  436, /* A */
  470, /* Bb */
  490  /* B */
};

boolean sound_on;
int push_prev;
int prev_tone;

void setup() {
  sound_on = false;
  push_prev = 0;
  prev_tone = 0;
  
  pinMode(PUSH_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if ( sound_on ) {
    int pot = analogRead(POT_PIN);
    int note = ((SCALE_LEN*pot) >> 10);
    int current_tone = scale[note];
    if ( current_tone != prev_tone ) {
      noTone(BUZZER_PIN);
      tone(BUZZER_PIN, current_tone);
      prev_tone = current_tone;
    }
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }
  
  int push = digitalRead(PUSH_PIN);
  if ( push != push_prev ) {
    if ( !push ) {
      sound_on = !sound_on;
      prev_tone = 0;
      delay(100);
    }
  }
  push_prev = push;
}
