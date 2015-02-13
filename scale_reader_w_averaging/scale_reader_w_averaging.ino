#include <Console.h>

float aReading = 94.0;
float aLoad = 30.0; // lbs.
float bReading = 26.0;
float bLoad = 11.0; // lbs.

long time = 0;
float readingsTotal = 0;
int numOfReadings = 0;
float averageReading = 0;

int readInterval = 5;
int writeInterval = 2000;

void setup() {
  pinMode(13, OUTPUT);
  Bridge.begin();
  Console.begin();
  digitalWrite(13, HIGH); //waiting for a human to open the console
  while (!Console);
  digitalWrite(13, LOW);
  Console.println("You're connected to the Console!!!!");
}

void loop() {
  float newReading = analogRead(0);

  if(millis() % readInterval == 0) {
//    Console.println("Taking a reading");
    readingsTotal += newReading;
//    Console.print("readingsTotal: ");
//    Console.print(readingsTotal);
    
    numOfReadings ++;
//    Console.print(" - numOfReadings: ");
//    Console.println(numOfReadings);
    
  }
 
 if(millis() % writeInterval == 0) {
//    Console.print("readingsTotal: ");
//    Console.print(readingsTotal);
//    Console.print(" - numOfReadings: ");
//    Console.println(numOfReadings);
    
    averageReading =  readingsTotal / numOfReadings; 
    
 //    Calculate load based on A and B readings above
    float load = ((bLoad - aLoad)/(bReading - aReading)) * (averageReading - aReading) + aLoad;   
    
    Console.print("Avergae A0 Reading: ");
    Console.print(averageReading,1); // 2 decimal place
    Console.print(" - Voltage: ");
    Console.print(averageReading * .0048828125, 3);
    Console.print("V");
    Console.print(" - Load: ");
    Console.println(load,1);  // 1 decimal place, println adds a carriage return
//    time = millis();
    readingsTotal = 0;
    numOfReadings = 0;
  }
}
