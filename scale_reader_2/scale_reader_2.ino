#include <Console.h>


float aReading = 300.0;
float aLoad = 480.6; // lbs.
float bReading = 135.0;
float bLoad = 195.7; // lbs.

long time = 0;
int interval = 500; // Take a reading every 500 ms

void setup() {
  Bridge.begin();
  Console.begin();
  
  while (!Console){
    ; // wait for Console port to connect.
  }
  Console.println("You're connected to the Console!!!!");
}

void loop() {
  float newReading = analogRead(0);
  
  // Calculate load based on A and B readings above
  float load = ((bLoad - aLoad)/(bReading - aReading)) * (newReading - aReading) + aLoad;
  
  // millis returns the number of milliseconds since the board started the current program
  if(millis() > time + interval) {
    Console.print("Reading: ");
    Console.print(newReading,1); // 1 decimal place
    Console.print("  Load: ");
    Console.println(load,1);  // 1 decimal place, println adds a carriage return
    time = millis();
  }
}
