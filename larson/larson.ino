
#define PIN_COUNT 6
int pins[PIN_COUNT] = { 3, 5, 6, 9, 10, 11 };
int states[PIN_COUNT];
int current_pin = 0;
int dir = 1;
int update_count = 0;

void setup() {
  for ( int i = 0; i < PIN_COUNT; i++ ) {
    pinMode(pins[i], OUTPUT);
    states[i] = 0;
  }
}

void updatePins() {
  for ( int i = 0; i < PIN_COUNT; i++ ) {
    analogWrite(pins[i], states[i]);
  }
  delay(35);
}

void decay() {
  for ( int i = 0; i < PIN_COUNT; i++ ) {
    states[i] /= 2;
  }
}

void loop() {
  decay();
  states[current_pin] = 255;
  updatePins();
  
  update_count++;
  if ( update_count > 2 ) {
    update_count = 0;
    current_pin += dir;
    if ( current_pin == 0 ) {
      dir = 1;
    }
    else if ( current_pin == (PIN_COUNT-1) ) {
      dir = -1;
    }
  }
}
