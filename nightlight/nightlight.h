#ifndef NIGHTLIGHT_H
#define NIGHTLIGHT_H

#include <Arduino.h>
#include <math.h>

typedef struct {
   int red;
   int green;
   int blue;
} RGB;

#define NUM_COLORS 7
#define MAX_TEMP 27.0
#define MIN_TEMP 13.0

RGB colors[NUM_COLORS] = {
  (RGB){ 255, 255, 255 }, // White
  (RGB){ 255, 0, 255 }, // Purple
  (RGB){ 0, 0, 255 }, // Blue
  (RGB){ 0, 255, 255 }, // Cyan
  (RGB){ 0, 255, 0 }, // Green
  (RGB){ 255, 255, 0 }, // Yellow
  (RGB){ 255, 0, 0 }, // Red
};

int interpolate(int col1, int col2, double dx) {
  return (int)((1-dx)*col1 + dx*col2); 
};

RGB interpolate(RGB col1, RGB col2, double dx) {
  RGB rgb;
  rgb.red = interpolate(col1.red, col2.red, dx);
  rgb.green = interpolate(col1.green, col2.green, dx);
  rgb.blue = interpolate(col1.blue, col2.blue, dx);
  return rgb;
};

RGB pick_color(int index) {
  index = max(0, min(NUM_COLORS-1, index));
  return colors[index];
}

void print_rgb(RGB rgb) {
  Serial.print("{ ");
  Serial.print(rgb.red);
  Serial.print(", ");
  Serial.print(rgb.green);
  Serial.print(", ");
  Serial.print(rgb.blue);
  Serial.println("}");
}

RGB color_for_temperature(double t) {
  double normalised = (t-MIN_TEMP)/(MAX_TEMP-MIN_TEMP);
  
  double col = normalised*NUM_COLORS;
  int col_low = (int)floor(col);
  int col_high = (int)ceil(col);

  double dx = col-col_low;
  if ( t < MIN_TEMP || t > MAX_TEMP ) {
    dx = 0; 
  }

  RGB rgb_high = pick_color(col_high),
      rgb_low  = pick_color(col_low);
  RGB rgb = interpolate(rgb_low, rgb_high, dx);
  //print_rgb(rgb);
  return rgb;
};

#endif // NIGHTLIGHT_H
