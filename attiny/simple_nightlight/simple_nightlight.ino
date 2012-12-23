#ifdef __AVR_ATtiny85__
#define SWITCH_PIN 0
#define RED_PIN 1
#define GREEN_PIN 2
#define BLUE_PIN 3
#else
#define SWITCH_PIN 2
#define LED_PIN 3
// TODO
#endif

#define COLORS 7

#define STATE_IDLE 0
#define STATE_BUTTON_DOWN 1
#define STATE_BUTTON_RELEASED 2
#define STATE_TRANSITION 3
#define STATE_SHOW_COLOR 4

#define WHITE 0
#define YELLOW 1
#define RED 2
#define PURPLE 3
#define BLUE 4
#define CYAN 5
#define GREEN 6

int state = STATE_SHOW_COLOR;
int debounce = 0;
int color = 0;
int next_color = 0;

void setup() {
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void showColor(int color) {
  switch(color) {
    case WHITE: {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
    }
    break;
    case YELLOW: {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
    }
    break;
    case RED: {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);   
    }
    break;
    case PURPLE: {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);          
    }
    break;
    case BLUE: {
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
    }
    break;
    case CYAN: {
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);   
    }
    break;
    case GREEN: {
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);   
    }
    break;
  }
}

void blendColors(int percentage, int color1, int color2) { 
  for ( int i = 0; i < 100; i++ ) {
    showColor(i > percentage? color1 : color2);
    delayMicroseconds(1);
  }
}

void loop() {
  switch(state) {
    case STATE_IDLE: {
      if ( digitalRead(SWITCH_PIN) == LOW ) {
        state = STATE_BUTTON_DOWN;
        debounce = 0;
      }
    }
    break;
    case STATE_BUTTON_DOWN: {
      // make sure button held down for long enough
      if ( digitalRead(SWITCH_PIN) == LOW ) {
        debounce++;
      }
      else if ( debounce >= 2 ) {
        state = STATE_BUTTON_RELEASED;
      }
      else {
        state = STATE_IDLE;
      }
    }
    break;
    case STATE_BUTTON_RELEASED: {
      next_color = color + 1;
      if ( next_color >= COLORS ) {
        next_color = 0;
      }
      state = STATE_TRANSITION;
    }
    break;
    case STATE_TRANSITION: {
      state = STATE_SHOW_COLOR;
      for ( int i = 0; i < 100; i++ ) {
        blendColors(i+1, color, next_color);
      }
      color = next_color;
    }
    break;
    case STATE_SHOW_COLOR: {
      showColor(color);      
      state = STATE_IDLE;
    }
    break;
  }
  delay(50);
}

