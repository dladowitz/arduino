//Change LED state after button is pressed.
//If pressed for more than 500 miliseconds increase brightness

// currently just turning on with button push

const int LED    = 9;
const int BUTTON = 7;

int button_val = 0;
int old_button_val = 0;

int led_state = 0;

int brightness = 1;
unsigned long startTime = 0;

void setup(){
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
//  Serial.begin(9600);
}


void loop(){
  button_val = digitalRead(BUTTON);

 
  
  if (button_val == HIGH && old_button_val == LOW) {
   led_state = 1 - led_state;
   startTime = millis();
   delay(10);
  } 

  if (button_val == HIGH && old_button_val == HIGH) {
    if (led_state == 1 && (millis() - startTime) > 500) {
      brightness++;
      delay(10);
      
      if (brightness >= 255) {
        brightness = 0;
      }
    }
  }

  old_button_val = button_val;
  
  if (led_state == 1) {
    analogWrite(LED, brightness);
  } else {
    analogWrite(LED, 0);
  }

}

