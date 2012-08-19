
/**
 * TMP36 temperature sensor with it's output attached to analog
 * pin 0 on an arduino.
 * Outputs 10mV per degree with a 500mV offset for zero degrees C (so it can
 * measure negative temperatures)
 *
 * http://oomlout.com/TMP36/TMP36-Guide.pdf
 *
 * 
 *
 **/

#include "nightlight.h"
#include "circular_buffer.h"

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

#define MODE_PIN 10

#define TEMP_ANALOG_PIN 0

#define VOLTAGE_SCALE 4.96 // not quite 5

//#define RUN_TEST

// circular buffer of RGB colors
CircularBuffer<RGB, 50> rgb_buffer;
CircularBuffer<double, 45> temperature_buffer;
double last_temperature;
int read_count;

double read_temp() {
  int r = analogRead(TEMP_ANALOG_PIN);
  double v = VOLTAGE_SCALE*r/1024;
  double t = (v-0.5) * 100;
  return t; 
}

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // switch using pullup resistor
  pinMode(MODE_PIN, INPUT);
  digitalWrite(MODE_PIN, HIGH);
  
  RGB zero(0, 0, 0);
  rgb_buffer.init(zero);
  
  double t = read_temp();
  temperature_buffer.init(t);
  last_temperature = 0;
  read_count = 0;
  
#ifdef RUN_TEST
  delay(1000);
  Serial.println("BEGIN TEST");
  
  for ( int i = 9; i < 29; i++ ) {
    Serial.print("T = ");
    Serial.println(i);
    RGB col = color_for_temperature(i);
    print_rgb(col);
    Serial.println();
    delay(500);
  }
  
  Serial.println("END TEST");
  delay(1000);
#endif
}

void update_LED() {
  RGB avg = rgb_buffer.avg();
  
  analogWrite(RED_PIN, 255-avg.red);
  analogWrite(GREEN_PIN, 255-avg.green);
  analogWrite(BLUE_PIN, 255-avg.blue);
}

void rgb(int r, int g, int b) {
  RGB rgb(r, g, b);
  rgb_buffer.add(rgb);
}

void update_temp() {
  if ( read_count == 0 ) {
    double t = read_temp();
    // ignore out of range values
    if ( t > 0 && t < 40 ) {
      temperature_buffer.add(t);
    }
  }
  read_count = (read_count + 1) % 10;
}

double calc_temperature() {
  return temperature_buffer.median();
}

void print_temperature(double t) {
  // only print if temperature changes by decent amount
  if ( fabs(last_temperature-t) > 0.1 ) {
    Serial.print(t);
    Serial.println("C");
    last_temperature = t;
  }
}

void loop() {
  update_temp();
  if ( digitalRead(MODE_PIN) == HIGH ) {
    rgb(255, 255, 255);
  }
  else {
    double t = calc_temperature();
    print_temperature(t);
    RGB col = color_for_temperature(t);
    rgb(col.red, col.green, col.blue);
  }
  update_LED();
  delay(100);
}
