
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

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

#define MODE_PIN 10

#define RGB_BUFFER_SIZE 10


// circular buffer of RGB colors
RGB rgb_buffer[RGB_BUFFER_SIZE];
int rgb_current;

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // switch using pullup resistor
  pinMode(MODE_PIN, INPUT);
  digitalWrite(MODE_PIN, HIGH);
  
  RGB zero = { 0, 0, 0 };
  for ( int i = 0; i < RGB_BUFFER_SIZE; i++ ) {
      rgb_buffer[i] = zero;
  }
  rgb_current = 0;
}

void update_LED() {
  int red = 0, green = 0, blue = 0;
  for ( int i = 0; i < RGB_BUFFER_SIZE; i++ ) {
      RGB* rgb = &(rgb_buffer[i]);
      red += rgb->red;
      green += rgb->green;
      blue += rgb->blue;
  }
  red /= RGB_BUFFER_SIZE;
  green /= RGB_BUFFER_SIZE;
  blue /= RGB_BUFFER_SIZE;
  
  analogWrite(RED_PIN, 255-red);
  analogWrite(GREEN_PIN, 255-green);
  analogWrite(BLUE_PIN, 255-blue);
}

void rgb(int r, int g, int b) {
  RGB rgb = { r, g, b };
  rgb_buffer[rgb_current] = rgb;
  rgb_current = (rgb_current+1) % RGB_BUFFER_SIZE;
}

void loop() {
  if ( digitalRead(MODE_PIN) == HIGH ) {
    rgb(255, 255, 255);
  }
  else {
    int r = analogRead(0);
    double v = 5.0*r/1024;
    double t = (v-0.5) * 100;
    Serial.print("temperature (C) = ");
    Serial.println(t);
    
    RGB high, low;
    high = color_for_temperature(t+3);
    low = color_for_temperature(t);
    double dx = fmod(t, 3)/3.0;
    dx = dx < 0? 0 : dx;
    
    RGB col = interpolate(high, low, dx);
    rgb(col.red, col.blue, col.green);
  }
  update_LED();
  delay(250);
}
