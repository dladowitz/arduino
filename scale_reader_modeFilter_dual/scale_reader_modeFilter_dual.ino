#include <Bridge.h>
#include <Console.h>
#include <Process.h>

#define NUM_READS        500
#define CALIBRATION_MODE "OFF"
#define API_ADDRESS      "http://www.littlecatlabs.co/api/v1/weights"

//Need to not store this on github in production mode
#define SCALE_ID          "70183"
#define SCALE_PASSWORD    "littlecatlabs" 

//Set communication method. Options are "Serial" for USB or "Console" for WIFI
#define COMM_METHOD "Console"

// 30lb weight = 29.4375
// 8 books     = 10.45625

//46 Ohm Resistor & usb power to laptop on HealthOMeter
float aReading = 722;
float aLoad = 18.8125; // box and two waters
float bReading = 473;
float bLoad = 8.6875; // water A
float load;

float filteredResult = 0;

int redLed = 13;
int blueLed = 3;
int greenLed = 5;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  
  if(COMM_METHOD == "Serial"){
    Serial.begin(9600);
    digitalWrite(redLed, HIGH);
    while(!Serial);
    printStringViaComm("You're connected to the Serial Monitor!!!!");
    digitalWrite(redLed, LOW);
  } else {
    Bridge.begin();
    Console.begin();
    digitalWrite(redLed, HIGH);  
    while(!Console);
    printStringViaComm("You're connected to the Console!!!!");
    digitalWrite(redLed, LOW);    
  }

  flashLed(redLed, 20, 100);
}

void loop() {
  filteredResult = modeFilter();    

  printStringViaCommNl(" ");
  printStringViaCommNl("========== Add Weight To Scale ==========");   
  
//  Wait for more than .6lbs to be put on scale
  while(filteredResult < 285.0){
    fadeLed(redLed, 1, 50);
    printStringViaCommNl(" ");
    printStringViaCommNl("waiting for weight.......");
    printStringViaCommNl(" ");
    
    filteredResult = modeFilter();    
  }


  printStringViaCommNl("========== Stabilizing and Confirming Weight ==========");       
  printStringViaCommNl(" ");   
  
  // Wait for same result 10 times in a row. Otherwise break and start over.
  for(int i = 1; i <= 10; i++) {
    flashLed(redLed, 1, 100);
    
    if(i == 1) {      
      filteredResult = modeFilter();
      printFloatViaComm(filteredResult);
      printStringViaComm(" - Match: ");
      printIntViaCommNl(i);       
    } else {
      if (filteredResult != modeFilter() ) {
        filteredResult = modeFilter();
        printStringViaCommNl(" ");
        printStringViaCommNl("========== Readings Don't Match. Starting Over ==========");        
        printStringViaCommNl(" ");        
        break; 
      } else {       
        printFloatViaComm(filteredResult);        
        printStringViaComm(" - Match: ");
        printIntViaCommNl(i);  
      }        
    }

    // Only get here to print if same result 10 times in a row      
    if (i == 10){
      printStringViaCommNl(" ");
      printStringViaCommNl("========== Weight Confirmed ==========");            
      printReadings("Digital Voltage", filteredResult);       
      printStringViaCommNl("======================================");  
      printStringViaCommNl(" ");          
      
     flashLed(greenLed, 10, 100);
     analogWrite(greenLed, 255);
     
     // Send data and pause unless calibration mode is turned on
     if (CALIBRATION_MODE == "OFF"){
       delay(3000);
       sendWeightToAPI(load);
       waitForWeightRemoval(filteredResult);     
     }

     analogWrite(greenLed, 0);          
    }  
  }    
}


void printReadings(String readingLabel, float reading){
   // Calculate load based on A and B readings above
  load = ((bLoad - aLoad)/(bReading - aReading)) * (reading - aReading) + aLoad;   
  
  printStringViaComm(readingLabel);
  printStringViaComm(": ");
  printFloatViaComm(reading);    
  printStringViaComm(" - Weight: ");
  printFloatViaComm(load);  
  printStringViaCommNl(" lbs");
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

void sendWeightToAPI(float weightToSend){
  String weightParams;
  weightParams += " --data \"scale_id=";
  weightParams += SCALE_ID;
  weightParams += "&scale_password=";
  weightParams += SCALE_PASSWORD;
  weightParams += "&weight_amount=";
  weightParams += weightToSend;
  weightParams += "\"";
  
  printStringViaCommNl(" ");
  printStringViaCommNl("========== Sending Weight to API ==========");
  printStringViaComm("Connecting to: ");
  printStringViaCommNl(API_ADDRESS);  
  fadeLed(blueLed, 1, 40);
  
  printStringViaComm("Sending ");
  printFloatViaComm(weightToSend);  
  printStringViaCommNl("lbs to server");  
  Process p;
  String cmd = "curl ";
  cmd += API_ADDRESS;
  cmd += weightParams;
  printStringViaComm("API Call: ");
  printStringViaCommNl(cmd);  
  p.runShellCommand(cmd);
  
  fadeLed(blueLed, 1, 40);
  
  printStringViaComm("Server Response: ");
  while (p.available()) {
    char c = p.read();
    printCharViaComm(c);  
  }
  printStringViaCommNl(" ");
  
  // Ensure the last bit of data is sent.
  flushViaComm();    
  printStringViaCommNl("Data sent");  
  fadeLed(blueLed, 1, 40);  

  printStringViaCommNl("===========================================");  
  printStringViaCommNl(" ");  
}

// Remove weight to reset scale
void waitForWeightRemoval(int sentWeight){
  printStringViaCommNl(" ");
  printStringViaCommNl("=========== Reseting Scale ===========");
  while(modeFilter() > 285){
    printStringViaCommNl("Remove weight to reset scale......");
    fadeLed(greenLed, 1, 100);
  }
  printStringViaCommNl(" ");
  printStringViaCommNl("========== Weight Removed. Scale Reseting ==========");  
}

// Used to choose serial or console for either Uno or Yun boards. 
// Need to figure out a conversion strategy, too many methods being added that look the same.
void printStringViaComm(String message){
  if(COMM_METHOD == "Console"){
      Console.print(message);
  } else {
      Serial.print(message);
  }
}

void printStringViaCommNl(String message){
  if(COMM_METHOD == "Console"){
      Console.println(message);
  } else {
      Serial.println(message);
  }
}

void printFloatViaComm(float message){
  if(COMM_METHOD == "Console"){
      Console.print(message);
  } else {
      Serial.print(message);
  }
}

void printFloatViaCommNl(float message){
  if(COMM_METHOD == "Console"){
      Console.println(message);
  } else {
      Serial.println(message);
  }
}

void printIntViaComm(int message){
  if(COMM_METHOD == "Console"){
      Console.print(message);
  } else {
      Serial.print(message);
  }
}

void printIntViaCommNl(int message){
  if(COMM_METHOD == "Console"){
      Console.println(message);
  } else {
      Serial.println(message);
  }
}

void printCharViaComm(char message){
  if(COMM_METHOD == "Console"){
      Console.print(message);
  } else {
      Serial.print(message);
  }
}

void printCharViaCommNl(char message){
  if(COMM_METHOD == "Console"){
      Console.println(message);
  } else {
      Serial.println(message);
  }
}
// Ensure the last bit of data is sent.
void flushViaComm(){
  if(COMM_METHOD == "Console"){
      Console.flush();
  } else {
      Serial.flush();
  }  
}
