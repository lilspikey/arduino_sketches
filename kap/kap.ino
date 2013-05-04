#include <Servo.h>

#define SERVO_PIN 9

#define TOP_ANGLE 100
#define BOTTOM_ANGLE 85

Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  servo.write(TOP_ANGLE);
}

void loop() {
  delay(5000);
  servo.write(BOTTOM_ANGLE);
  delay(600);
  servo.write(TOP_ANGLE);
}
