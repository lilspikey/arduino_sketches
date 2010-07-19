#include <Servo.h>

#define DOORBELL_PIN 5
#define STATE_WAITING 0
#define STATE_RUNG 1
#define DOORBELL_THRESHOLD 50

Servo servo;
int state;

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  state = STATE_WAITING;
}

void loop() {
  int v = analogRead(DOORBELL_PIN);
  delay(100);
  switch(state) {
    case STATE_WAITING: {
      if ( v < DOORBELL_THRESHOLD ) {
        for ( int i = 0; i < 3; i++ ) {
          servo.write(140);
          delay(150);
          servo.write(70);
          delay(500);
        }
        state = STATE_RUNG;
      }
    }
    break;
    case STATE_RUNG: {
      if ( v > DOORBELL_THRESHOLD ) {
        state = STATE_WAITING;
      } 
    }
    break;
  }
}
