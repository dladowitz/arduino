const int LED = 13;
const int BUTTON = 7;
      int button_state = LOW;
      int led_state = LOW;
      
void setup(){
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);

}

void changeState(){  
  if (led_state == HIGH){
    digitalWrite(LED, LOW);
    led_state = LOW;
  } else {
    digitalWrite(LED, HIGH);
    led_state = HIGH;
  }
  
  button_state = LOW;
  delay(500);     //prevents button push from being registed twice
}
  
void loop(){

  button_state = digitalRead(BUTTON);
  
  if (button_state == HIGH){
    changeState();
  }
}

