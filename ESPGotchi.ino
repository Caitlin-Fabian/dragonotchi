#include <Wire.h>
#include <FastLED.h>
#include "WiFi.h"
#define NUM_LEDS 256
#define DATA_PIN 5
#include "Characters.h"
#include "Firebase.h"
#include "LightSensor.h"

const unsigned int SW_pin = 2;
const unsigned int ROW_LENGTH = 15;
const unsigned int COL_LENGTH = 15;

int iterator = 0;
unsigned long timeSinceSwitch = 0;
String currentSprite;

CRGB leds[NUM_LEDS];

boolean inPlay = false;
boolean startedTimer = false;
int cleanlinessTimerLimit = 10000 * 4;
int cleanlinessTimer = 0;

int boredomTimerLimit = 10000 * 4;
int boredomTimer = 0;

int hungerTimerLimit = 10000 * 4;
int hungerTimer = 0;

int frameRate = 1000;
int tick = 0;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(20);

  pinMode(SW_pin, INPUT);
  Serial.begin(115200);
  
  // Setup Wifi
  Serial.print("Setting up WiFi...");
  setupWifi();
  
  setupFirebase();
  
  currentSprite = "Normal";
}

void updateHealth() {
  

  // update health
  if (cleanlinessLevel + hungerLevel + boredomLevel >= MAX_CLEANLINESS + MAX_HUNGER + MAX_BOREDOM) {
    health = 5;
  } else if (cleanlinessLevel + hungerLevel+ boredomLevel >= MAX_CLEANLINESS + MAX_HUNGER + MAX_BOREDOM - dropAmount) {
    health = 4;
  } else if (cleanlinessLevel + hungerLevel+ boredomLevel >= MAX_CLEANLINESS + MAX_HUNGER + MAX_BOREDOM - dropAmount * 2) {
    health = 3;
  } else if (cleanlinessLevel + hungerLevel+ boredomLevel >= MAX_CLEANLINESS + MAX_HUNGER + MAX_BOREDOM - dropAmount * 3) {
    health = 2;
  } else if (cleanlinessLevel + hungerLevel+ boredomLevel >= MAX_CLEANLINESS + MAX_HUNGER + MAX_BOREDOM - dropAmount * 4) {
    health = 1;
  } else if (cleanlinessLevel + hungerLevel+ boredomLevel >= MAX_CLEANLINESS + MAX_HUNGER + MAX_BOREDOM - dropAmount * 5) {
    health = 0;
  }

  if (Firebase.ready()) { 
    
    if (Firebase.setInt(fbdo, "/health", health)){
        Serial.println("UPDATED HEALTH");
    }
    else {
      Serial.println("FAILED TO UPDATE HEALTH");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}

void loop() {
  loopFirebase();
  
  // readLightSensor();
  updateHealth();

  if (dragonExists) {
    // Decrease cleanliness
    if ((millis() - cleanlinessTimer > cleanlinessTimerLimit || cleanlinessTimer == 0))
    {
      cleanlinessTimer = millis();
      if (cleanlinessLevel > 0) {
        cleanlinessLevel-= 1;

        if (Firebase.ready()) {
          if (Firebase.setInt(fbdo, "/cleanlinessLevel", cleanlinessLevel)){
              Serial.println("UPDATED CLEAN");
          }
          else {
            Serial.println("FAILED TO UPDATE CLEAN");
            Serial.println("REASON: " + fbdo.errorReason());
          }
        }
      }
    }
    // increase boredoom
    if ((millis() - boredomTimer > boredomTimerLimit || boredomTimer == 0))
    {
      boredomTimer = millis();
      if (boredomLevel > 0) {
        boredomLevel-= 1;
        if (Firebase.ready()) {
          if (Firebase.setInt(fbdo, "/boredomLevel", boredomLevel)){
              Serial.println("UPDATED BOREDOM");
          }
          else {
            Serial.println("FAILED TO UPDATE BOREDOM");
            Serial.println("REASON: " + fbdo.errorReason());
          }
        }
      }
    }
  
    // reset cleanliness timer if used mop
    if (usedMop) {
      cleanlinessTimer = millis();
      usedMop = false;
      if (Firebase.ready()) {
        if (Firebase.setBool(fbdo, "/usedMop", usedMop)){
            Serial.println("UPDATED USED MOP");
        }
        else {
          Serial.println("FAILED TO UPDATE USED MOP");
          Serial.println("REASON: " + fbdo.errorReason());
        }
      }
    }
  
    // Decrease health if it hasn't been fed
    if ((millis() - hungerTimer > hungerTimerLimit || hungerTimer == 0))
    {
      hungerTimer = millis();
      if (hungerLevel > 0) {
        hungerLevel-= 1;
        if (Firebase.ready()) {
          if (Firebase.setInt(fbdo, "/hunger", hungerLevel)){
              Serial.println("UPDATED HUNGER");
          }
          else {
            Serial.println("FAILED TO UPDATE HUNGER");
            Serial.println("REASON: " + fbdo.errorReason());
          }
        }
      }
    }
  
    // Update sprite
    if ((millis() - timeSinceSwitch > 1000 || timeSinceSwitch == 0) && (!startedTimer && (threwBall || threwFood)))
    {
      timeSinceSwitch = millis();
      startedTimer = true;
    }
    
    if (health == 0) {
      Serial.println("Died");
//      for (int i = 0; i < NUM_LEDS; i++) {
//        leds[i] = image_data_DeadFromPoop[i];
//      }
      currentSprite = "Died";
    } 
    else if (threwBall) {
      boredomTimer = millis();
      if ((millis() - timeSinceSwitch > 2000 || timeSinceSwitch == 0) && startedTimer)
      {
        timeSinceSwitch = millis();
        startedTimer = false;
        threwBall = false;
        if (Firebase.ready()) {
          if (Firebase.setBool(fbdo, "/threwBall", false)){
              Serial.println("UPDATED THREW BALL");
          }
          else {
            Serial.println("FAILED TO UPDATE THREW BALL");
            Serial.println("REASON: " + fbdo.errorReason());
          }
        }
      }
      Serial.println("Catching ball");
//      for (int i = 0; i < NUM_LEDS; i++) {
//        leds[i] = image_data_EatFruit[i];
//      }
      currentSprite = "Ball";
    }
    else if (threwFood) {
      hungerTimer = millis();
      if ((millis() - timeSinceSwitch > 3000 || timeSinceSwitch == 0) && startedTimer)
      {
        timeSinceSwitch = millis();
        startedTimer = false;
        threwFood = false;
        if (Firebase.ready()) {
          if (Firebase.setBool(fbdo, "/threwFood", false)){
              Serial.println("UPDATED threwFood");
          }
          else {
            Serial.println("FAILED TO UPDATE threwFood");
            Serial.println("REASON: " + fbdo.errorReason());
          }
        }
      }
      Serial.println("Eating food");
      currentSprite = "Eat";
    }
    else if (cleanlinessLevel < 3) {
      Serial.println("Poop");
      currentSprite = "Poop";
    }
    else if (boredomLevel < 3) {
      Serial.println("Bored or sleeping");
      currentSprite = "Bored";
    }
    else {  
      Serial.println("Normal");
      currentSprite = "Normal";
    }
  }

  if ((millis() - tick > frameRate || tick == 0))
  {
    tick = millis();
    // Update LEDS
    // Erase previous lit LEDS
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    
    FastLED.show();

    if (dragonExists) {
      
    if (currentSprite == "Normal") {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = image_data_Normal[i];
      }
    }
    else if (currentSprite == "Bored") {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = image_data_sleep[i];
      }
    }
    else if (currentSprite == "Poop") {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = image_data_Poop[i];
      }
    }
    else if (currentSprite == "Eat") {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = image_data_chicken[i];
      }
    }
    else if (currentSprite == "Ball") {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = image_data_EatFruit[i];
      }
    }
    else if (currentSprite == "Died") {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = image_data_DeadFromPoop[i];
      }
    }
    }

  }
    
  FastLED.show();
  
  // Listen for HTTP Post/Get Requests
  server.handleClient();

  // websocketServer.loop();
}
