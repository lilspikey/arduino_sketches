#ifndef CHARLIEPLEX_H
#define CHARLIEPLEX_H

#include <Arduino.h>

#define CHARLIE_PORT PORTB
#define CHARLIE_DDR DDRB


template <int SIZE>
class Charlieplex {
  unsigned char _mask;
  unsigned char _port[SIZE * (SIZE-1)];
  unsigned char _ddr[SIZE * (SIZE-1)];
  unsigned char _pins[SIZE * (SIZE-1)];
  
  void _define_pin(int pin, int maskHigh, int maskLow) {
    _ddr[pin] = maskLow | maskHigh;
    _port[pin] = maskHigh;
  }

  public:
    Charlieplex(int masks[SIZE])
     : _mask(0) {
      for ( int i = 0; i < SIZE; i++ ) {
        int m = masks[i];
        _mask |= m;
      }
      
      /*
      Define order of LEDs. They proceed in pairs using the same pair of pins. With the first
      LED in the pair being that one that would be lit be bring the first pin high and the second
      pin low
       */
      int pin = 0;
      for ( int i = 0 ; i < SIZE; i++ ) {
        for ( int j = i+1; j < SIZE; j++ ) {
          int maski = masks[i];
          int maskj = masks[j];
          _define_pin(pin, maski, maskj);
          pin++;
          _define_pin(pin, maskj, maski);
          pin++;
        }
      }
    }
    
    int size() {
      return SIZE * (SIZE-1);
    }
    
    void clear() {
      for ( int i = 0; i < SIZE * (SIZE-1); i++ ) {
       _pins[i] = 0;
      } 
    }
    
    void pin(int pinNum, int pinVal) {
      _pins[pinNum] = pinVal;
    }
    
    void display() {
      int num = size();
      for ( int pin = 0; pin < num; pin++ ) {
        if ( _pins[pin] ) {
          // first turn off everything before changing
          // pin direction - on ATtiny when not not doing this
          // was getting extra LEDs lit, when two different LEDs
          // were lit
          CHARLIE_PORT = (CHARLIE_PORT & ~_mask);
          CHARLIE_DDR  = (CHARLIE_DDR  & ~_mask) | _ddr[pin];
          CHARLIE_PORT = CHARLIE_PORT | _port[pin];
        }
        delayMicroseconds(1);
      }
      /** turn off all LEDs **/
      CHARLIE_DDR  = CHARLIE_DDR  & ~_mask;
      CHARLIE_PORT = CHARLIE_PORT & ~_mask;
    }
    
};

#endif // CHARLIEPLEX_H
