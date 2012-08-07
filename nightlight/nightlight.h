#ifndef NIGHTLIGHT_H
#define NIGHTLIGHT_H

#include "Arduino.h"

typedef struct {
   int red;
   int blue;
   int green;
} RGB;

int interpolate(int col1, int col2, double dx) {
  return (int)(dx*col1 + (1-dx)*col2); 
};

RGB interpolate(RGB col1, RGB col2, double dx) {
  RGB rgb;
  rgb.red = interpolate(col1.red, col2.red, dx);
  rgb.green = interpolate(col1.green, col2.green, dx);
  rgb.blue = interpolate(col1.blue, col2.blue, dx);
  return rgb;
};

RGB color_for_temperature(double t) {
  RGB rgb;
  if ( t >= 27 ) {
    rgb = (RGB){ 255, 0, 0 };
  }
  else if ( t >= 24 ) {
    rgb = (RGB){ 255, 255, 0 };
  }
  else if ( t >= 21 ) {
    rgb = (RGB){ 0, 255, 0 };
  }
  else if ( t >= 18 ) {
    rgb = (RGB){ 0, 255, 255 };
  }
  else if ( t >= 15 ) {
    rgb = (RGB){ 0, 0, 255 };
  }
  else if ( t >= 13 ) {
    rgb = (RGB){ 255, 0, 255 };
  }
  else {
    rgb = (RGB){ 255, 255, 255 };
  }
  return rgb;
};

#endif // NIGHTLIGHT_H
