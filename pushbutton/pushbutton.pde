#define DOORBELL_PIN 9

int prev = 1;

void setup() {
  Serial.begin(9600);
  pinMode(DOORBELL_PIN, INPUT);
}

void loop() {
  int next = digitalRead(DOORBELL_PIN);
  delay(1000);
  if ( next != prev ) {
    prev = next;
    if ( !next ) {
      Serial.println("DING DONG");
    }
  }
}
