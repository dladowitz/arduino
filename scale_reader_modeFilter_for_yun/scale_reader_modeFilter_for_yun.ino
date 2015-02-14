#include <Console.h>
#define NUM_READS 500

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

float newReading = 0;

int redLed = 13;
int blueLed = 5;
int greenLed = 3;

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
}

void loop() {
  float newReading = analogRead(0);

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
      analogWrite(greenLed, 255);
      printReadings("Mode Reading", filteredResult);       
      Console.println("-------------------------------------");           
    }  
  }    
}


void printReadings(String readingLabel, float reading){
   // Calculate load based on A and B readings above
  float load = ((bLoad - aLoad)/(bReading - aReading)) * (reading - aReading) + aLoad;   
  
  Console.print(readingLabel);
  Console.print(": ");
  Console.print(reading,2);
  Console.print(" - Load: ");
  Console.println(load,2); 
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

