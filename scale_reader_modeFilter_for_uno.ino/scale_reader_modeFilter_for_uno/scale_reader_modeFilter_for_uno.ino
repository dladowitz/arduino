#include <Console.h>
#define NUM_READS 500

// 30lb weight = 29.4375
//  8 books = 10.45625

//46 Ohm Resistor & usb power to laptop on HealthOMeter
float aReading = 742.0;
float aLoad = 18.8125; // box and two waters
float bReading = 488.0;
float bLoad = 8.6875; // water A

float filteredResult = 0;

int redLed = 13;
int blueLed = 3;
int greenLed = 5;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  
  Serial.begin(9600);
  flashLed(redLed, 20, 100);
  
  Serial.println("You're connected to the Serial Monitor!!!!");
}

void loop() {
  filteredResult = modeFilter();    

  Serial.println(" ");
  Serial.println("========== Add Weight To Scale ==========");   
  
//  Wait for more than .6lbs to be put on scale
  while(filteredResult < 285.0){
    fadeLed(greenLed, 1, 50);
    Serial.println(" ");
    Serial.println("waiting for weight.......");    
    
    filteredResult = modeFilter();    
  }

  Serial.println(" ");
  Serial.println("========== Stabilizing and Confirming Weight ==========");        
  Serial.println(" ");   
  
  // Wait for same result 10 times in a row. Otherwise break and start over.
  for(int i = 1; i <= 10; i++) {
    flashLed(redLed, 1, 100);
    
    if(i == 1) {      
      filteredResult = modeFilter();
      Serial.print(filteredResult);
      Serial.print(" - Match: ");
      Serial.println(i);       
    } else {
      if (filteredResult != modeFilter() ) {
        filteredResult = modeFilter();
        Serial.println(" ");
        Serial.println("========== Readings Don't Match. Starting Over ==========");        
        Serial.println(" ");        
        break; 
      } else {
        Serial.print(filteredResult);
        Serial.print(" - Match: ");
        Serial.println(i);  
      }        
    }

    // Only get here to print if same result 10 times in a row      
    if (i == 10){
      Serial.println(" ");
      Serial.println("========== Weight Confirmed ==========");            
      printReadings("Mode Reading", filteredResult);       
      Serial.println("======================================");  
      Serial.println(" ");          
      
     flashLed(greenLed, 10, 100);
     analogWrite(greenLed, 255);
     delay(3000);
     
     sendWeightToAPI(filteredResult);
     
     waitForWeightRemoval(filteredResult);

     analogWrite(greenLed, 0);          
    }  
  }    
}


void printReadings(String readingLabel, float reading){
   // Calculate load based on A and B readings above
  float load = ((bLoad - aLoad)/(bReading - aReading)) * (reading - aReading) + aLoad;   
  
  Serial.print(readingLabel);
  Serial.print(": ");
  Serial.print(reading,2);
  Serial.print(" - Load: ");
  Serial.println(load,2); 
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

void flashLed(int pinValue, int cycles, int delayInterval){
  for(int i = 1; i <= cycles; i++){
    digitalWrite(pinValue, HIGH);
    delay(delayInterval);    
    digitalWrite(pinValue, LOW);    
    delay(delayInterval);    
  }
}

void fadeLed(int pinValue, int cycles, int delayInterval){
  for(int i = 1; i <= cycles; i++){
      // fade in from min to max in increments of 5 points:
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
      // sets the value (range from 0 to 255):
      analogWrite(pinValue, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(delayInterval);
    }
  
    // fade out from max to min in increments of 5 points:
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
      // sets the value (range from 0 to 255):
      analogWrite(pinValue, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(delayInterval);
    }
  }
}

void sendWeightToAPI(int filteredResult){
  Serial.println(" ");
  Serial.println("========== Sending Weight to API ==========");
  Serial.println("Connecting...............");
  Serial.println(filteredResult);  
  fadeLed(blueLed, 4, 40);
  // add code here for curl to server
  Serial.println("Data sent");  
}

// Remove weight to reset scale
void waitForWeightRemoval(int sentWeight){
  Serial.println(" ");
  Serial.println("========== Reseting Scale ==========");
  while(modeFilter() > 285){
    Serial.println("Remove weight to reset scale......");
    fadeLed(greenLed, 1, 100);
  }
  Serial.println(" ");
  Serial.println("========== Weight Removed. Scale Reseting ==========");  
}
