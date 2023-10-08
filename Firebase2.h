#include <FirebaseArduino.h>

//Set these to the Firebase project settings
#define FIREBASE_URL "https://dragonotchi-default-rtdb.firebaseio.com"
#define FIREBASE_DB_SECRET "pWoIw4pHYnPwPbn82trjowwt5VOHuJdt2KkW0Z7o"
#define PATH "/"

int last_read = 0;
int diff = 0;

void setupFirebase() {
  //begin Firebase
  Firebase.begin(FIREBASE_URL, FIREBASE_DB_SECRET);
  
  //start streaming the data for the updating value
  Firebase.stream(PATH);
}


void loopFirebase() {
  //check if streaming failed and try to restart
  if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());
    delay(1000);
    Firebase.stream(PATH);
    return;
  }
  
  if (Firebase.available()) {
    //get the event
    FirebaseObject event = Firebase.readEvent();
    String data = event.getString("/boredomLevel");
    
    Serial.println(data);

    //set streaming again
    Firebase.stream(PATH);
  }  
}
