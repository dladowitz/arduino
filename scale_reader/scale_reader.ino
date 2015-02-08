//Potentiometer controls voltage into A0
//When A0 is over 500 on/off light turns on. 
//Pin6 led displays variable brightness depending on the level of voltage in A0. 
//Pin6 led has its brightness mapped so 500 = 0 brightness and 1024 = 255 brighness



#include <Console.h>

int sensorPin = A0;    // select the input pin for the potentiometer
//int greenLedPin = 13;  // select the pin for the LED
//int blueLedPin =  6;
int sensorValue = 0;  // variable to store the value coming from the sensor
//int brightness = 0;

void setup() {
  // declare the ledPin as an OUTPUT:
//  pinMode(greenLedPin, OUTPUT);
//  pinMode(blueLedPin, OUTPUT);
  Bridge.begin();
  Console.begin();
  
  while (!Console){
    ; // wait for Console port to connect.
  }
  Console.println("You're connected to the Console!!!!");

}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  Console.println(sensorValue);
  
//  
//  if(sensorValue >= 500){
//    // turn the ledPin on
//    digitalWrite(greenLedPin, HIGH);
//
//    brightness = map(sensorValue, 500, 1023, 0, 255);
//    Console.println(brightness);
//    
//    analogWrite(blueLedPin, brightness);
//  } else {
//    // turn the ledPin off:
//    digitalWrite(greenLedPin, LOW);
//    analogWrite(blueLedPin, 0);
//  }  
}
