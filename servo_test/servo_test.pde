#include <Servo.h>

Servo servo;

void setup() {
  servo.attach(9);
  Serial.begin(9600);
}

int read_position() {
  int pos = 0;
  while( Serial.available() ) {
    int n = Serial.read();
    delay(1);
    if ( n >= '0' && n <= '9' ) {
      n -= '0';
      pos = 10*pos + n;
    }
    else {
      break;
    }
  }
  return pos;
}

void loop() {
  if ( Serial.available() ) {
    int pos = read_position(); 
    pos = pos >=0? (pos < 180? pos : 180) : 0;
    Serial.println(pos);
    
    servo.write(pos);
    delay(100);
  }
}


