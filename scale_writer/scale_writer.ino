/*
  Yún HTTP Client

 This example for the Arduino Yún shows how create a basic
 HTTP client that connects to the internet and downloads
 content. In this case, you'll connect to the Arduino
 website and download a version of the logo as ASCII text.

 created by Tom igoe
 May 2013

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/HttpClient

 */

#include <Bridge.h>
#include <HttpClient.h>
#include <Console.h>

void setup() {
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  Console.begin();

//  while (!Serial); // wait for a serial connection
}

void loop() {
  // Initialize the client library
  HttpClient client;

  // Make a HTTP request:

//    Serial.println("GET from server. \n");
//    client.get("http://10.0.1.91:3000/api/v1/weights");
//  client.get("http://arduino.cc/asciilogo.txt");


// POST
  Process p;
  String cmd = "curl --data \"scale_id=001&cat_id=30&weight_amount=12\" http://10.0.1.91:3000/api/v1/weights";
//
  Console.println("POST to server. \n");
  p.runShellCommand(cmd);
//  Console.println(cmd);
//  p.close();


  // if there are incoming bytes available
  // from the server, read them and print them:
  while (p.available()) {
    char c = p.read();
    Console.print(c);
  }
  Console.flush();

  delay(5000);
}


