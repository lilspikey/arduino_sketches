#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

template <class T, int SIZE>
class CircularBuffer {
    T _buffer[SIZE];
    int _current;
  
  public:
    CircularBuffer() {
      _current = 0;
    }
    
    void init(const T& val) {
      for ( int i = 0; i < SIZE; i++ ) {
        _buffer[i] = val;
      }
    }
    
    void add(const T& val) {
      _buffer[_current] = val;
      _current = (_current + 1) % SIZE;
    }
    
    T avg() {
      T sum(0);
      for ( int i = 0; i < SIZE; i++ ) {
        sum = sum + _buffer[i];
      }
      return sum/SIZE;
    }
    
    T median() {
      T sorted[SIZE];
      memcpy(sorted, _buffer, SIZE * sizeof(T));
      
      // selection sort
      for ( int i = 0; i < SIZE; i++ ) {
        int minj = i;
        for ( int j = i+1; j < SIZE; j++ ) {
          if ( sorted[j] < sorted[i] ) {
            minj = j;
          }
        }
        
        if ( minj != i ) {
          double tmp = sorted[i];
          sorted[i] = sorted[minj];
          sorted[minj] = tmp;
        }
      }
      /*
      for ( int i = 0; i < SIZE; i++ ) {
        Serial.print(i);
        Serial.print(" = ");
        Serial.println(sorted[i]);
      }
      Serial.println();
      
      delay(2000);*/
      
      int mid_point = SIZE/2;
      return sorted[mid_point];
    }
};

#endif // CIRCULAR_BUFFER_H

