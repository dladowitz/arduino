//Measures voltage on A0
//Sends http post command to server once voltage reaches a specific value
#include <Bridge.h>
#include <Process.h>
#include <Console.h>
int voltage = 0;
int time = 0;

void setup() {
//  Sets up bridge
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
//  Serial.begin(9600);
  Console.begin();
  digitalWrite(13, HIGH);

  // Wait until a Serial Monitor is connected.
//  while (!Serial);
  while (!Console);
  digitalWrite(13, LOW);
}

void loop() {
  time = millis();
  Console.print("Voltage: ");
  Console.println(voltage);  
  delay(2000);

  
  voltage = analogRead(A0);
  
  if(voltage > 500 && voltage < 600){
    digitalWrite(13, HIGH); 
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
    digitalWrite(13, HIGH); 
    delay(500);
    postToApi();
    digitalWrite(13, LOW);
    delay(5000);
  }
}

void postToApi() { 
  Process p;		                                                // Create a process and call it "p"
  String cmd = "curl http://10.0.1.91:3000/api/v1/weights --data \"scale_id=001&cat_id=30&weight_amount=12\"";
  p.runShellCommand(cmd);
  
//  p.begin("curl"); seems to use https when posting data. Need to use runShellCommand i
//  p.begin("curl");	                                                // Process that launch the "curl" command
//  p.addParameter(" --data \"scale_id=001&cat_id=30&weight_amount=12\""); // Add the URL parameter to "curl"
//  p.addParameter("http://10.0.1.91:3000/api/v1/weights");              // Add the URL parameter to "curl"
//  p.run();		                                                // Run the process and wait for its termination
  
  Console.print("Server Response: ");
  while (p.available()) {
    char c = p.read();
    Console.print(c);  
  }
  Console.println(" ");

  // Ensure the last bit of data is sent.
  Console.flush();
}
