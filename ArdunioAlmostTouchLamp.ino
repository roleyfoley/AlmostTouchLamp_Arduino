// Proximty controlled Lamp using NeoPixels 
// Written by Me 

// -- Sensor Read - Value in 16bit -  0 and 65,535
// vcnl.readAmbient()  - vcnl.readProximity()
// -- Pixel Write - Pixels in GRB format - 0 - 255 
// -- Pixels are Warm White Pixels for this lamp so doesn't matter
// pixels.setPixelColor(<Pixel>, pixels.Color(<0-255>,<0-255>,<0-255>));
// pixels.show();

// Proximity and Ambient Light Sensor Setup
#include <Wire.h>
#include <Adafruit_VCNL4010.h>

Adafruit_VCNL4010 vcnl; 

// NeoPixel Setup
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

const int PixelPIN = 8;
const int NUMPIXELS = 5;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PixelPIN, NEO_GRB);

// ** Behaviour Constants
// How close you need to be (65535 is on top of it, 0 is away from it)  
const unsigned int TriggerProx = 4000;
// Time you need to leave your hand over the prox sensor
const unsigned int TriggerDelay = 200; 
// Time it takes between brigning up the next light
const unsigned int LightDelay = 50;

// ** Running Variables
unsigned int PreviousProx = 0;
String LampState = "OFF";
unsigned int LightNumber = 0;

// ***** Script Begin ****
void setup() {
  pixels.begin();
  pixels.show();
  vcnl.begin(); 
}

void loop() {
  // Get the current reading
  unsigned int CurrentProx = vcnl.readProximity();

  // See if something is close 
  if (CurrentProx > TriggerProx) {

     // Check to see if reading is the same after a delay
     delay(TriggerDelay);
     unsigned int DelayProx = vcnl.readProximity();

     if (DelayProx >= TriggerProx) {

        if (LightNumber == 0 && LampState == "OFF") {
            LampState = "ON";
            while( vcnl.readProximity() >= TriggerProx ) {
              pixels.setPixelColor(LightNumber, pixels.Color(255,255,255));
              pixels.show();
              delay(LightDelay);
              if (LightNumber != (NUMPIXELS - 1)) {
              LightNumber++; 
              }
            }
            
        }
        
        if (LightNumber > 0 && LampState == "ON" && vcnl.readProximity() >= TriggerProx ) { 
            LampState = "OFF";
            while( vcnl.readProximity() >= TriggerProx ) {
              pixels.setPixelColor(LightNumber, pixels.Color(0,0,0));
              pixels.show();
              delay(LightDelay);
              if (LightNumber != 0) {
              LightNumber--; 
              } 
            }
        }
         
      }
  }
}

