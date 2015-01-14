int switchState = 0;

void setup(){
  pinMode(2, INPUT);
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop(){
  switchState = digitalRead(2);
  
  
  if (switchState == HIGH){
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    delay(200);
    
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    delay(200);
    
  } else {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(4, LOW);
  }
}

