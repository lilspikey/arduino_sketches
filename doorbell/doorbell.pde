#include <Servo.h>

#define DOORBELL_PIN 5
#define SERVO_PIN 9

#define STATE_WAITING 0
#define STATE_RUNG 1
#define DOORBELL_THRESHOLD 50

#define DOORBELL_INIT_ANGLE 75
#define DOORBELL_TOP_ANGLE 90
#define DOORBELL_BOTTOM_ANGLE 180

Servo servo;
int state;

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  state = STATE_WAITING;
  servo.write(DOORBELL_INIT_ANGLE);
}

void loop() {
  int v = analogRead(DOORBELL_PIN);
  delay(100);
  switch(state) {
    case STATE_WAITING: {
      if ( v < DOORBELL_THRESHOLD ) {
        // ensure servo attached again
        servo.attach(SERVO_PIN);
        for ( int i = 0; i < 3; i++ ) {
          servo.write(DOORBELL_BOTTOM_ANGLE);
          delay(500);
          servo.write(DOORBELL_TOP_ANGLE);
          delay(500);
        }
        state = STATE_RUNG;
        servo.write(DOORBELL_INIT_ANGLE);
        delay(500);
        // turn off servo
        // so to avoid buzzing from it being
        // overloaded
        servo.detach();
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
