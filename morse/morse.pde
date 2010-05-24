
#define OUTPUT_PIN 13

#define STATE_IDLE 0
#define STATE_DOT 1
#define STATE_DASH 2
#define STATE_SPACE 3
#define STATE_GAP 4

#define GAP_LENGTH 150
#define DOT_LENGTH 150
#define DASH_LENGTH 450
#define SPACE_LENGTH 450

#define MESSAGE_MAX 512

char* morse_table[] = {
  ".-",   /* A */
  "-...", /* B */
  "-.-.", /* C */
  "-..",  /* D */
  ".",    /* E */
  "..-.", /* F */
  "--.",  /* G */
  "....", /* H */
  "..",   /* I */
  ".---", /* J */
  "-.-",  /* K */
  ".-..", /* L */
  "--",   /* M */
  "-.",   /* N */
  "---",  /* O */
  ".--.", /* P */
  "--.-", /* Q */
  ".-.",  /* R */
  "...",  /* S */
  "-",    /* T */
  "..-",  /* U */
  "...-", /* V */
  ".--",  /* W */
  "-..-", /* X */
  "-.--", /* Y */
  "--..", /* Z */

  "-----", /* 0 */  
  ".----", /* 1 */
  "..---", /* 2 */
  "...--", /* 3 */
  "....-", /* 4 */
  ".....", /* 5 */
  "-....", /* 6 */
  "--...", /* 7 */
  "---..", /* 8 */
  "----.", /* 9 */

};

struct state {
  long timer;
  int state;
};

int seq_num = 0;
char *sequence = NULL;
struct state morse;

int char_num = 0;
int message_len = 0;
char message[MESSAGE_MAX];

void change_state(struct state *pstate, int new_state) {
  pstate->timer = millis();
  pstate->state = new_state;
}

void run_morse() {
  long current_time = millis();
  long state_duration = (current_time - morse.timer);
  int next_state = morse.state;
  switch(morse.state) {
    case STATE_DOT:
      if ( state_duration < DOT_LENGTH ) {
        digitalWrite(OUTPUT_PIN, HIGH);
      }
      else {
        next_state = STATE_GAP;
      }
    break;
    case STATE_DASH:
      if ( state_duration < DASH_LENGTH ) {
        digitalWrite(OUTPUT_PIN, HIGH);
      }
      else {
        next_state = STATE_GAP;
      }
    break;
    case STATE_SPACE:
      if ( state_duration < SPACE_LENGTH ) {
        digitalWrite(OUTPUT_PIN, LOW);
      }
      else {
        next_state = STATE_GAP;
      }
    break;
    case STATE_GAP:
      if ( state_duration < GAP_LENGTH ) {
        digitalWrite(OUTPUT_PIN, LOW);
      }
      else {
        next_state = STATE_IDLE;
      }
  }
  
  if ( morse.state != next_state ) {
    change_state(&morse, next_state);
  }
  
  if ( morse.state == STATE_IDLE ) {
    if ( sequence && seq_num < strlen(sequence) ) {
      char next = sequence[seq_num];
      seq_num++;
      switch(next) {
        case '.':
          change_state(&morse, STATE_DOT);
          break;
        case '-':
          change_state(&morse, STATE_DASH);
          break;
        default:
          change_state(&morse, STATE_SPACE);
      }
    }
    else {
      if ( char_num < message_len ) {
        int offset = -1;
        char message_char = message[char_num];
        if ( 'a' <= message_char && message_char <= 'z' ) {
          offset = (int)(message_char - 'a');
        }
        else if ( 'A' <= message_char && message_char <= 'Z' ) {
          offset = (int)(message_char - 'A');
        }
        else if ( '0' <= message_char && message_char <= '9' ) {
          offset = (int)(message_char - '0') + 26;
        }
        if ( offset > 0 ) {
          sequence = morse_table[offset];
          seq_num = 0;
        }
        char_num++;
        change_state(&morse, STATE_SPACE);
      }
      else {
        sequence = NULL;
      }
    }
  }
}

void setup() {
  pinMode(OUTPUT_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if ( char_num >= message_len ) {
    /* message finished, so reset */
    char_num = 0;
    message_len = 0;
  }
  
  if ( Serial.available() ) {
    int in = Serial.read();
    /* record bytes if there is space */
    if ( message_len < MESSAGE_MAX ) {
      message[message_len] = (char)in;
      message_len++;
      Serial.print(in, BYTE);
    }
    else {
      Serial.print(-1, BYTE);
    }
  }
  else {
    run_morse();
  }
}


