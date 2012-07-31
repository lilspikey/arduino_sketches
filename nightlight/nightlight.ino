
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
 
#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

#define MODE_PIN 10

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // switch using pullup resistor
  pinMode(MODE_PIN, INPUT);
  digitalWrite(MODE_PIN, HIGH);  
}

void rgb(int r, int g, int b) {
  analogWrite(RED_PIN, 255-r);
  analogWrite(GREEN_PIN, 255-g);
  analogWrite(BLUE_PIN, 255-b);
}

void loop() {
  if ( digitalRead(MODE_PIN) == HIGH ) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  }
  else {
    int r = analogRead(0);
    double v = 5.0*r/1024;
    double t = (v-0.5) * 100;
    Serial.print("temperature (C) = ");
    Serial.println(t);
    
    if ( t >= 27 ) {
      rgb(255, 0, 0);
    }
    else if ( t >= 24 ) {
      rgb(255, 255, 0);
    }
    else if ( t >= 21 ) {
      rgb(0, 255, 0);
    }
    else if ( t >= 18 ) {
      rgb(0, 255, 255);
    }
    else if ( t >= 15 ) {
      rgb(0, 0, 255);
    }
    else if ( t >= 13 ) {
      rgb(255, 0, 255);
    }
    else {
      rgb(255, 255, 255);
    }
  }
  delay(1000);
}
