#define DOORBELL_PIN 0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int v = analogRead(DOORBELL_PIN);    
  delay(1000);
  Serial.println(v);  
}
