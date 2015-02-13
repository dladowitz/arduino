#include <Console.h>
#define NUM_READS 500

//200 Ohm Resistor
//float aReading = 98.0;
//float aLoad = 29.4375; // 30lb weight
//float bReading = 28.0;
//float bLoad = 10.45625; // 8 books

//46 Ohm Resistor
//float aReading = 458.0;
//float aLoad = 29.4375; // 30lb weight
//float bReading = 129.0;
//float bLoad = 10.45625; // 8 books

//46 Ohm Resistor & usb power pack
float aReading = 281.0;
float aLoad = 18.8125; // box and two waters
float bReading = 109.0;
float bLoad = 8.6875; // water 8

float readingsTotal = 0;
int   numOfReadings = 0;
float newReading = 0;
float averageReading = 0;

float lowestReading = 1023;
float highestReading = 0;

int   nonAveragingWriteInterval = 500;
int   averagingReadInterval = 50;
int   averagingWriteInterval = 1000;

String averagingMode;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  Bridge.begin();
  Console.begin();
  
  digitalWrite(13, HIGH); //waiting for a human to open the console
  while (!Console);
  digitalWrite(13, LOW);
  Console.println("You're connected to the Console!!!!");
  
  // Set Averaging Mode
  averagingMode = "modeFilter";
//  analogReference(2); //set analog reference to internal 1.1V
}

void loop() {
  float newReading = analogRead(0);

//  Check for which mode to run in
  if(averagingMode == "none") {
     if(millis() % nonAveragingWriteInterval == 0){     
       printReadings("Current A0", newReading);
     }
     
  // Average reading over time   
  } else if(averagingMode == "modeFilter"){
    float filteredResult = 0;
    
    // Wait for same result 10 times in a row. Otherwise break and start over.
    for(int i = 1; i <= 10; i++) {
      if(i == 1) {      
        filteredResult = modeFilter();
      } else {
        if (filteredResult != modeFilter() ) {
          filteredResult = modeFilter();
          break; 
        } else {
          Console.print(filteredResult);
          Console.print(" - Match: ");
          Console.println(i);  
        }        
      }

      // Only get here to print if same result 10 times in a row      
      if (i == 10){
        Console.println("-------------- Winnner --------------");
        analogWrite(254);
        printReadings("Mode Reading", filteredResult);       
        Console.println("-------------------------------------");           
      }  
    }
  
  
  
  
  
  
  
  
  
  } else if(averagingMode == "average") {
    if(millis() % averagingReadInterval == 0) {
      readingsTotal += newReading;
      numOfReadings ++;
    }
   
    if(millis() % averagingWriteInterval == 0) {
      averageReading =  readingsTotal / numOfReadings; 
      
      printReadings("Average A0", averageReading);
      
      readingsTotal = 0;
      numOfReadings = 0;
    }
    
    // Lowest reading over time interval - Not working correctly. Might be left broken.
    } else if (averagingMode == "lowest") {   
      int stablizingPeriod = 4000;

      // Run for the first 10 seconds
      while(millis() < 20000){
        if(millis() % averagingReadInterval == 0) {
          if(newReading < lowestReading){
            lowestReading = newReading;
          }
        
          if(newReading > highestReading){
            highestReading = newReading;
          }
        } 
        
       if(millis() % averagingWriteInterval == 0) {
          printReadings("Lowest A0", lowestReading);
       }
     }
     
     // After 10 seconds 
     // newReading needs to be 1/3rd lower than set low to change it
     if(newReading < (lowestReading - (lowestReading/3))){
       lowestReading = newReading;
      }

     // newReading needs to be 1/3rd higher than set high to change it  
     if(newReading > (highestReading + (highestReading/3))){
       highestReading = newReading;
     }  
   
     if(millis() % averagingWriteInterval == 0) {
        printReadings("Lowest A0", lowestReading);
     }     
  }
}


void printReadings(String readingLabel, float reading){
   // Calculate load based on A and B readings above
  float load = ((bLoad - aLoad)/(bReading - aReading)) * (reading - aReading) + aLoad;   
  
  Console.print(readingLabel);
  Console.print(": ");
  Console.print(reading,2);
//  Console.print(" - Voltage: ");
//  Console.print(reading * .0048828125, 3);
//  Console.print("V");
  Console.print(" - Load: ");
  Console.println(load,2);  // 1 decimal place, println adds a carriage return
}

// From http://www.elcojacobs.com/eleminating-noise-from-sensor-readings-on-arduino-with-digital-filtering/

float modeFilter(){
   // read multiple values and sort them to take the mode
   int sortedValues[NUM_READS];
   for(int i=0;i<NUM_READS;i++){
     int value = analogRead(0);
     int j;
     if(value<sortedValues[0] || i==0){
        j=0; //insert at first position
     }
     else{
       for(j=1;j<i;j++){
          if(sortedValues[j-1]<=value && sortedValues[j]>=value){
            // j is insert position
            break;
          }
       }
     }
     for(int k=i;k>j;k--){
       // move all values higher than current reading up one position
       sortedValues[k]=sortedValues[k-1];
     }
     sortedValues[j]=value; //insert current reading
   }
   //return scaled mode of 10 values
   float returnval = 0;
   for(int i=NUM_READS/2-5;i<(NUM_READS/2+5);i++){
     returnval +=sortedValues[i];
   }
   returnval = returnval/10;
   return returnval;
}

