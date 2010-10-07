
/**
 * Testing TMP36 temperature sensor with it's output attached to analog
 * pin 0 on an arduino.
 * Outputs 10mV per degree with a 500mV offset for zero degrees C (so it can
 * measure negative temperatures)
 *
 * http://oomlout.com/TMP36/TMP36-Guide.pdf
 **/
void setup() {
  Serial.begin(9600);
}

void loop() {
  int r = analogRead(0);
  Serial.print("reading = ");
  Serial.println(r);
  
  double v = 5.0*r/1024;
  Serial.print("voltage = ");
  Serial.println(v);
  
  double t = (v-0.5) * 100;
  Serial.print("temperature (C) = ");
  Serial.println(t);
  
  delay(1000);
}
