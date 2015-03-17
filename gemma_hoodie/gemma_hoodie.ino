#include <Adafruit_NeoPixel.h>

//#define PIN 1

int sensorPin = 1;    // select the input pin for the potentiometer, analog 1 on GEMMA is digital 2
int ledPin = 1;      // select the pin for the LED, GEMMA has one attached to D1
int sensorValue = 0;  // variable to store the value coming from the sensor
float brightness = 0;
int random_num = 0;
String color = "white";


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
//  randomSeed(analogRead(sensorPin));
//  random_num = random(1, 6); // Set up random color
//  delay(1000);
  
  sensorValue = analogRead(sensorPin); 
  if (sensorValue > 900){
    color = "purple";
  } else if (sensorValue > 700) {
    color = "teal";
  } else if (sensorValue > 500) {
    color = "orange";
  } else if (sensorValue > 300) {
    color = "light green";
  } else if (sensorValue > 100) {
    color = "red";
  } else {
    color = "white";
  }
}
void loop() {
  
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin); 
  brightness = sensorValue / 4;


  if (color == "purple"){
    colorWipe(strip.Color(brightness, 0, brightness), 50);
  } else if (color == "light green"){
    colorWipe(strip.Color(brightness / 2, brightness, 0), 50);
  }else if (color == "teal"){
    colorWipe(strip.Color(0, brightness, brightness), 50);
  }else if (color == "orange"){
    colorWipe(strip.Color(brightness, brightness/2, 0), 50);
  } else if (color == "red"){
    colorWipe(strip.Color(brightness, 0, 0), 50);
  } else {
    colorWipe(strip.Color(brightness, brightness, brightness), 50);
  }
  
  
  delay(1000);
}


// Some example procedures showing how to display to the pixels:
// colorWipe(strip.Color(255, 0, 0), 50); // Red
// colorWipe(strip.Color(0, 255, 0), 50); // Green
// colorWipe(strip.Color(0, 0, 255), 50); // Blue

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


//Use to check voltages. Voltages are comming in from approx 1000 - 10
// Red 
//  if (sensorValue > 1000) {
//    colorWipe(strip.Color(255, 0, 0), 50); // full
//  } else if (sensorValue > 950 ) {
//    colorWipe(strip.Color(150, 0, 0), 50); // bright
//  } else if (sensorValue > 940 ){
//    colorWipe(strip.Color(50, 0, 0), 50); // medium
//  } else if (sensorValue > 930 ){
//    colorWipe(strip.Color(10, 0, 0), 50); // light
//  } else if (sensorValue > 920 ){
//    colorWipe(strip.Color(5, 0, 0), 50); // dim
//  } else if (sensorValue > 910 ) {
//    colorWipe(strip.Color(1, 0, 0), 50); // super dim
//  } else if (sensorValue > 900 ) {
//    colorWipe(strip.Color(0, 0, 0), 50); // off?
//  }
//  
//// Blue
//  else if (sensorValue > 860 ) {
//    colorWipe(strip.Color(0, 255, 0), 50); // full
//  } else if (sensorValue > 850 ) {
//    colorWipe(strip.Color(0, 150, 0), 50); // bright
//  } else if (sensorValue > 840 ){
//    colorWipe(strip.Color(0, 50, 0), 50); // medium
//  } else if (sensorValue > 830 ){
//    colorWipe(strip.Color(0, 10, 0), 50); // light 
//  } else if (sensorValue > 820 ){
//    colorWipe(strip.Color(0, 5, 0), 50); // dim
//  } else if (sensorValue > 810 ) {
//    colorWipe(strip.Color(0, 1, 0), 50); // super dim
//  } else if (sensorValue > 800 ) {
//    colorWipe(strip.Color(0, 0, 0), 50); // off?
//  }
// 
//// Green
//  else if (sensorValue > 760 ) {
//    colorWipe(strip.Color(0, 0, 255), 50); // full
//  } else if (sensorValue > 750 ) {
//    colorWipe(strip.Color(0, 0, 150), 50); // bright
//  } else if (sensorValue > 740 ){
//    colorWipe(strip.Color(0, 0, 50), 50); // medium
//  } else if (sensorValue > 730 ){
//    colorWipe(strip.Color(0, 0, 10), 50); // light 
//  } else if (sensorValue > 720 ){
//    colorWipe(strip.Color(0, 0, 5), 50); // dim
//  } else if (sensorValue > 710 ) {
//    colorWipe(strip.Color(0, 0, 1), 50); // super dim
//  } else if (sensorValue > 700 ) {
//    colorWipe(strip.Color(0, 0, 0), 50); // off?
//  }
//  
//// Purlpe
//  else if (sensorValue > 660 ) {
//    colorWipe(strip.Color(255, 0, 255), 50); // full
//  } else if (sensorValue > 650 ) {
//    colorWipe(strip.Color(150, 0, 150), 50); // bright
//  } else if (sensorValue > 640 ){
//    colorWipe(strip.Color(50, 0, 50), 50); // medium
//  } else if (sensorValue > 630 ){
//    colorWipe(strip.Color(10, 0, 10), 50); // light 
//  } else if (sensorValue > 620 ){
//    colorWipe(strip.Color(5, 0, 5), 50); // dim
//  } else if (sensorValue > 610 ) {
//    colorWipe(strip.Color(1, 0, 1), 50); // super dim
//  } else if (sensorValue > 600 ) {
//    colorWipe(strip.Color(0, 0, 0), 50); // off?
//  }  
//  
//// Pink
//  else if (sensorValue > 560 ) {
//    colorWipe(strip.Color(255, 0, 127), 50); // full
//  } else if (sensorValue > 550 ) {
//    colorWipe(strip.Color(150, 0, 75), 50); // bright
//  } else if (sensorValue > 540 ){
//    colorWipe(strip.Color(50, 0, 25), 50); // medium
//  } else if (sensorValue > 530 ){
//    colorWipe(strip.Color(10, 0, 5), 50); // light 
//  } else if (sensorValue > 520 ){
//    colorWipe(strip.Color(5, 0, 3), 50); // dim
//  } else if (sensorValue > 510 ) {
//    colorWipe(strip.Color(1, 0, 1), 50); // super dim
//  } else if (sensorValue > 500 ) {
//    colorWipe(strip.Color(0, 0, 0), 50); // off?
//  }     
//
//// Orange
//  else if (sensorValue > 460 ) {
//    colorWipe(strip.Color(255, 127, 0), 50); // full
//  } else if (sensorValue > 450 ) {
//    colorWipe(strip.Color(150, 75, 0), 50); // bright
//  } else if (sensorValue > 440 ){
//    colorWipe(strip.Color(50, 25, 0), 50); // medium
//  } else if (sensorValue > 430 ){
//    colorWipe(strip.Color(10, 5, 0), 50); // light 
//  } else if (sensorValue > 420 ){
//    colorWipe(strip.Color(5, 3, 0), 50); // dim
//  } else if (sensorValue > 410 ) {
//    colorWipe(strip.Color(1, 1, 0), 50); // super dim
//  } else if (sensorValue > 400 ) {
//    colorWipe(strip.Color(0, 0, 0), 50); // off?
//  }    





//
//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.
//uint32_t Wheel(byte WheelPos) {
//  if(WheelPos < 85) {
//   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//  } else if(WheelPos < 170) {
//   WheelPos -= 85;
//   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  } else {
//   WheelPos -= 170;
//   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  }
//}

