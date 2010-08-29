/**
 Simple sketch that cycles and RGB LED between random colours
 **/

#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

int r, g, b;
int nr, ng, nb;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  r = g = b = 0;
  nr = ng = nb = 0;
}

int move_towards(int x, int nx) {
  if ( x != nx ) {
    x = (x < nx)? x+1 : x-1;
  }
  return x;
}

void loop() {
  if ( r == nr && g == ng && b == nb ) {
    nr = random(256);
    ng = random(256);
    nb = random(256);
  }
  
  r = move_towards(r, nr);
  g = move_towards(g, ng);
  b = move_towards(b, nb);
  
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);

  delay(10);
}


