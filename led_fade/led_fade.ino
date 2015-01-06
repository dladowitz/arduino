const int LED = 9;          // set pin for the LED
const int BUTTON = 7;        // set pin for pushbutton
int brightness = 124;
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
  

  for (brightness=0; brightness <= 255; brightness++) {
    analogWrite(LED, brightness);  // turn LED on 
    delay(5);
  }
  
  if (brightness >= 255) {
    for (brightness = 255; brightness > 0; brightness--) {
      analogWrite(LED, brightness);
      delay(5);
    }    
  }
 
}
