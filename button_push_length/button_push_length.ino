//Change LED state after button is pressed.
//If pressed for more than 500 miliseconds increase brightness

// currently just turning on with button push

const int LED    = 9;
const int BUTTON = 7;

int button_val = 0;
int old_button_val = 0;

int let_state = 0;


void setup(){
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}


void loop(){
  button_val = digitalRead(BUTTON);
//  button_val = HIGH;¨
  
  if (button_val == HIGH) {
    analogWrite(LED, 200);
  }
}

