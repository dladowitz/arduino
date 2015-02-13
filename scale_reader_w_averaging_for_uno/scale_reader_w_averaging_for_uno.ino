float aReading = 94.0;
float aLoad = 30.0; // lbs.
float bReading = 26.0;
float bLoad = 11.0; // lbs.

float readingsTotal = 0;
int   numOfReadings = 0;
float newReading = 0;
float averageReading = 0;

int   nonAveragingWriteInterval = 500;
int   averagingReadInterval = 100;
int   averagingWriteInterval = 2000;

boolean averagingMode;

void setup() {
  Serial.begin(9600);
  Serial.println("You're connected to the Serial Monitor!!!!");
}

void loop() {
  float newReading = analogRead(0);

//  Check for which mode to run in
  if(averagingMode == false) {
     if(millis() % nonAveragingWriteInterval == 0){
       float load = ((bLoad - aLoad)/(bReading - aReading)) * (newReading - aReading) + aLoad;           
       Serial.print("Immediate Reading: ");
       Serial.print(newReading, 0); // 2 decimal place
       Serial.print(" - Voltage: ");
       Serial.print(newReading * 0.004887585533, 3);
       Serial.print("V");
       Serial.print(" - Load: ");
       Serial.println(load,2);  // 1 decimal place, println adds a carriage return
     }
  } else {
    if(millis() % averagingReadInterval == 0) {
      readingsTotal += newReading;
      numOfReadings ++;
    }
   
   if(millis() % averagingWriteInterval == 0) {
     averageReading =  readingsTotal / numOfReadings; 
      
   //    Calculate load based on A and B readings above
      float load = ((bLoad - aLoad)/(bReading - aReading)) * (averageReading - aReading) + aLoad;   
      
      Serial.print("Avergae A0 Reading: ");
      Serial.print(averageReading,1); // 2 decimal place
      Serial.print(" - Voltage: ");
      Serial.print(averageReading * .0048828125, 3);
      Serial.print("V");
      Serial.print(" - Load: ");
      Serial.println(load,1);  // 1 decimal place, println adds a carriage return
  
      readingsTotal = 0;
      numOfReadings = 0;
    }
  }
}

