#include <Wire.h>
#include <FastLED.h>
#include "WiFi.h"
#define NUM_LEDS 256
#define DATA_PIN 5
#include "Characters.h"
#include "Dragonotchi.h"
#include "Firebase.h"
#include "LightSensor.h"

const unsigned int SW_pin = 2;
const unsigned int ROW_LENGTH = 15;
const unsigned int COL_LENGTH = 15;

CRGB leds[NUM_LEDS];

uint32_t currentSprite;
boolean inPlay = false;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(5);

  pinMode(SW_pin, INPUT);
  Serial.begin(115200);

  currentSprite = image_data_DragonTest[256];
  
  // Setup Wifi
  Serial.print("Setting up WiFi...");
  setupWifi();
  
  setupFirebase();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Update LEDS
  
  FastLED.show();
  
  // Erase previous lit LEDS
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }

  // Update sprite
  if (!inPlay) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = currentSprite;
    }
  }
  
  // Listen for HTTP Post/Get Requests
  server.handleClient();

  loopFirebase();

  websocketServer.loop();

// Blocking!!!!!! !  BAD !!!
//  auto client = websocketServer.accept();
//  if(client.available()) {
//    Serial.println("Client is available.");
//    
//    auto msg = client.readBlocking();
// 
//    // log
//    Serial.print("Got Message: ");
//    Serial.println(msg.data());
//
//    // return echo
//    client.send("Echo: " + msg.data());
//
//    // close the connection
//    client.close();
//  }
}
