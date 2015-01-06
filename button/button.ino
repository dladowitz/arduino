
const int LED = 13;          // set pin for the LED
const int BUTTON = 7;        // set pin for pushbutton

int val = 0;                  // val will be used to store the state of the input pin


void setup()
{
  pinMode(LED, OUTPUT);       // set LED pin as an output.
  pinMode(BUTTON, INPUT);     // set BUTTON pin as input.
  
}

void loop()
{
  val = digitalRead(BUTTON);  // read input and store
  
  // check whether the input is HIGH (button pressed)
  
  if (val == HIGH) {
    digitalWrite(LED, HIGH);  // turn LED on
  } else {
    digitalWrite(LED, LOW);
  }
}

