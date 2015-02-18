#include <Bridge.h>
#include <Console.h>
#include <Process.h>

// --------------------------------- Modes  -------------------------------------
#define COMM_METHOD     "Console"
#define NUM_READS        500
#define MATCH_NUMBER     10
#define CALIBRATION_MODE "ON"
#define STABILIZE_MODE   "ON"
#define STABILIZE_RANGE  2
#define API_ADDRESS      "http://www.littlecatlabs.co/api/v1/weights"
//#define SCALE_ID          "10577" // Aiko 
//#define SCALE_ID          "70183" // Dusty
#define SCALE_ID          "Debug" // Debug
#define SCALE_PASSWORD    "littlecatlabs" 
// -----------------------------------------------------------------------------

// ---- Known weights ----
// 30lb weight       =  29.4375
// 8 books           =  10.45625
// Water A           =  8.6875
// Water B           =  8.7812
// Box with 2 waters =  18.8125
// Printer           =  16.875
// ----------------------

//46 Ohm Resistor & usb power pack on HealthOMeter
float aReading = 717;
float aLoad = 18.8125; // box and two waters
float bReading = 469;
float bLoad = 8.6875; // water A

//float aReading = 410;
//float aLoad = 4; // 30lb dumbell
//float bReading = 334;
//float bLoad = 2; // 8 books


float load;
float filteredResult = 0;
float stabilizationTotal = 0;

int redLed = 13;
int blueLed = 5;
int greenLed = 3;
int yellowLed = 6;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  digitalWrite(greenLed, HIGH);
  
  if(COMM_METHOD == "Serial"){
    Bridge.begin();
    Serial.begin(9600);
    digitalWrite(blueLed, HIGH);
    while(!Serial){
      printStringViaCommNl("Waiting for Serial Connection......");
    };
    printStringViaComm("You're connected to the Serial Monitor!!!!");
    digitalWrite(blueLed, LOW);
  } else if (COMM_METHOD == "Console") {
    Bridge.begin();
    Console.begin();
    digitalWrite(yellowLed, HIGH);  
    while(!Console);
    printStringViaComm("You're connected to the Console!!!!");
    digitalWrite(yellowLed, LOW);    
  } else if(COMM_METHOD == "None") {
    digitalWrite(redLed, HIGH); 
    Bridge.begin();
    digitalWrite(redLed, LOW);
  }
  
  // 20 times flash indicates we've made it through Comm and Bridge Setup
  flashLed(redLed, 20, 100);
  
  printStringViaCommNl(" ");  
  printStringViaCommNl("========== Modes and Settings ==========");  
  printStringViaComm("Comm Method:         ");
  printStringViaCommNl(COMM_METHOD);  
  printStringViaComm("Number of Reads:     ");
  printIntViaCommNl(NUM_READS);  
  printStringViaComm("Match Number:        ");  
  printIntViaCommNl(MATCH_NUMBER);  
  printStringViaComm("Calibration Mode:    ");  
  printStringViaCommNl(CALIBRATION_MODE);    
  printStringViaComm("Stabilization Mode:  ");  
  printStringViaCommNl(STABILIZE_MODE);    
  printStringViaComm("Stabilization Range: ");  
  printStringViaCommNl(STABILIZE_MODE); 
  printStringViaComm("Scale Id:            ");  
  printStringViaCommNl(SCALE_ID);    
  printStringViaComm("Server Address:      ");  
  printStringViaCommNl(API_ADDRESS);  
  printStringViaCommNl("========================================");  
  printStringViaCommNl(" ");  
}

void loop() {    
  printStringViaCommNl(" ");
  printStringViaCommNl("========== Add Weight To Scale ==========");   
  
  filteredResult = modeFilter();

  // Wait for more than .5lbs to be put on scale
  while(filteredResult < 285.0){
    fadeLed(redLed, 1, 50);
    printStringViaCommNl(" ");
    printStringViaCommNl("waiting for weight.......");
    printStringViaCommNl(" ");
    
    filteredResult = modeFilter();    
  }


  printStringViaCommNl("========== Stabilizing and Confirming Weight ==========");       
  printStringViaCommNl(" ");   
  
  // Wait for same result MATCH_NUMBER times in a row. Otherwise break and start over.
  // Should make this into a method
  for(int i = 1; i <= MATCH_NUMBER; i++) {
    flashLed(redLed, 1, 100);
    
    if(i == 1) {      
      filteredResult = modeFilter();
      printFloatViaComm(filteredResult);
      printStringViaComm(" - Initial: ");
      printIntViaCommNl(i);   
      if(STABILIZE_MODE == "ON"){
        stabilizationTotal += filteredResult;
      }  
    } else {
      float newFilteredResult = modeFilter();        
      
      // Matching Mode
      if(STABILIZE_MODE == "OFF"){
        if (newFilteredResult != filteredResult ) {
          printFloatViaComm(newFilteredResult);
          printStringViaCommNl(" - BAD MATCH");        
          printStringViaCommNl(" ");
          printStringViaCommNl("========== Starting Over ==========");        
          printStringViaCommNl(" ");
          filteredResult = newFilteredResult;        
          break; 
        } else {       
          printFloatViaComm(newFilteredResult);        
          printStringViaComm(" - Match: ");
          printIntViaCommNl(i);  
        }  
      //     
      } else if(STABILIZE_MODE == "ON"){
        if (newFilteredResult < filteredResult - STABILIZE_RANGE || newFilteredResult > filteredResult + STABILIZE_RANGE) {
          printFloatViaComm(newFilteredResult);
          printStringViaCommNl(" - Outside of Stablize Range");        
          printStringViaCommNl(" ");
          printStringViaCommNl("========== Starting Over ==========");        
          printStringViaCommNl(" ");
          filteredResult = newFilteredResult;  
          stabilizationTotal = 0;
          break; 
        } else {       
          printFloatViaComm(newFilteredResult);        
          printStringViaComm(" - In Range: ");
          printIntViaCommNl(i);  
          stabilizationTotal += newFilteredResult;
        }  
      }
    }

    // Only get here to print if same result show up multiple times     
    if (i == MATCH_NUMBER){
      if(STABILIZE_MODE == "ON"){
        filteredResult = (stabilizationTotal/MATCH_NUMBER);
        // Reset for next weight
        stabilizationTotal = 0;
      }
      
      printStringViaCommNl(" ");
      printStringViaCommNl("========== Weight Confirmed ==========");          
      printReadings("Digital Voltage", filteredResult);       
      printStringViaCommNl("======================================");  
      printStringViaCommNl(" ");          
      
     flashLed(yellowLed, 10, 100);
     analogWrite(yellowLed, 255);
     
     // Send data and pause unless calibration mode is turned on
     if (CALIBRATION_MODE == "OFF"){
       delay(3000);
       sendWeightToAPI(load);
       waitForWeightRemoval(filteredResult);     
     }

     analogWrite(yellowLed, 0);          
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
   for(int i = 0; i < NUM_READS; i++){
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
    fadeLed(yellowLed, 1, 50);
  }
  printStringViaCommNl(" ");
  printStringViaCommNl("========== Weight Removed. Scale Reseting ==========");  
}


// Used to choose serial or console for either Uno or Yun boards. 
// Need to figure out a conversion strategy, too many methods being added that look the same.
void printStringViaComm(String message){
  if(COMM_METHOD == "Console"){
      Console.print(message);
  } else if(COMM_METHOD == "Serial") {
      Serial.print(message);
  } else {
    // print nothing
  }
}

void printStringViaCommNl(String message){
  if(COMM_METHOD == "Console"){
      Console.println(message);
  } else if(COMM_METHOD == "Serial") {
      Serial.println(message);
  } else {
    // print nothing
  }
}

void printFloatViaComm(float message){
  if(COMM_METHOD == "Console"){
      Console.print(message);
  } else if(COMM_METHOD == "Serial") {
      Serial.print(message);
  } else {
    // print nothing
  }
}

void printFloatViaCommNl(float message){
  if(COMM_METHOD == "Console"){
      Console.println(message);
  } else if(COMM_METHOD == "Serial") {
      Serial.println(message);
  } else {
    // print nothing
  }
}

void printIntViaComm(int message){
  if(COMM_METHOD == "Console"){
      Console.print(message);
  } else if(COMM_METHOD == "Serial") {
      Serial.print(message);
  } else {
    // print nothing
  }
}

void printIntViaCommNl(int message){
  if(COMM_METHOD == "Console"){
      Console.println(message);
  } else if(COMM_METHOD == "Serial") {
      Serial.println(message);
  } else {
    // print nothing
  }
}

void printCharViaComm(char message){
  if(COMM_METHOD == "Console"){
      Console.print(message);
  } else if(COMM_METHOD == "Serial") {
      Serial.print(message);
  } else {
    // print nothing
  }
}

void printCharViaCommNl(char message){
  if(COMM_METHOD == "Console"){
      Console.println(message);
  } else if(COMM_METHOD == "Serial") {
      Serial.println(message);
  } else {
    // print nothing
  }
}
// Ensure the last bit of data is sent.
void flushViaComm(){
  if(COMM_METHOD == "Console"){
      Console.flush();
  } else if(COMM_METHOD == "Serial") {
      Serial.flush();
  } else {
    // flush nothing
  }
}

