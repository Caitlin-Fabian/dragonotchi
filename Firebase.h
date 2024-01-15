#include <FirebaseESP8266.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>
// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 4. Define the Firebase Data object */
FirebaseData fbdo;
FirebaseData stream;

/* 5. Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* 6. Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long lastTime = 0;

String parentPath = "/";

int count = 0;

volatile bool dataChanged = false;

void sendMessage();


void streamCallback(StreamData data) {
  Serial.println("ayy lmao");

  Serial.printf("stream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
      data.streamPath().c_str(),
      data.dataPath().c_str(),
      data.dataType().c_str(),
      data.eventType().c_str());
      
  if (data.dataTypeEnum() == firebase_rtdb_data_type_integer)
      Serial.println(data.to<int>());
  else if (data.dataTypeEnum() == firebase_rtdb_data_type_float)
      Serial.println(data.to<float>(), 5);
  else if (data.dataTypeEnum() == firebase_rtdb_data_type_double)
      printf("%.9lf\n", data.to<double>());
  else if (data.dataTypeEnum() == firebase_rtdb_data_type_boolean)
      Serial.println(data.to<bool>()? "true" : "false");
  else if (data.dataTypeEnum() == firebase_rtdb_data_type_string)
      Serial.println(data.to<String>());
  else if (data.dataTypeEnum() == firebase_rtdb_data_type_json)
  {
      FirebaseJson *json = data.to<FirebaseJson *>();
      Serial.println(json->raw());
  }
  else if (data.dataTypeEnum() == firebase_rtdb_data_type_array)
  {
      FirebaseJsonArray *arr = data.to<FirebaseJsonArray *>();
      Serial.println(arr->raw());
  }

  Serial.println();

  dataChanged = true;
}

void streamTimeoutCallback(bool timeout) {
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected()){
    Serial.println("f's in the chat");
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
    
  }
}

void setupFirebase() {
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
    
    /* Assign the api key (required) */
    config.api_key = API_KEY;
    
    /* Assign the sevice account credentials and private key (required) */
    config.service_account.data.client_email = FIREBASE_CLIENT_EMAIL;
    config.service_account.data.project_id = FIREBASE_PROJECT_ID;
    config.service_account.data.private_key = PRIVATE_KEY;

    config.database_url = FIREBASE_HOST;

    // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
    Firebase.reconnectNetwork(true);

    // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
    // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
    fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
    stream.setBSSLBufferSize(4096 /* Rx in bytes, 512 - 16384 */, 1024 /* Tx in bytes, 512 - 16384 */);

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    Firebase.begin(&config, &auth);
}

void loopFirebase() {
  if (Firebase.ready() && (millis() - lastTime > 200 || lastTime == 0))
  {
      lastTime = millis();
      count++;
      FirebaseJson json;
      json.add("data", "hello");
      json.add("num", count);
      Serial.printf("Set json... %s\n\n", Firebase.setJSON(fbdo, "/test/stream/data/json", json) ? "ok" : fbdo.errorReason().c_str());

      sendMessage();
  }
}

void sendMessage() {
    // Write an Float number on the database path test/float
    if (Firebase.getInt(fbdo, "/boredomLevel")){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.stringData());
//      Serial.println("TYPE: " + fbdo.dataType());
      boredomLevel = (fbdo.intData());
    }
    else {
      Serial.println("FAILED TO FETCH BOREDOM");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.getInt(fbdo, "/hunger")){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.stringData());
//      Serial.println("TYPE: " + fbdo.dataType());
      hungerLevel = (fbdo.intData());
    }
    else {
      Serial.println("FAILED TO FETCH HUNGER");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.getBool(fbdo, "/dragonExists")){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.stringData());
//      Serial.println("TYPE: " + fbdo.dataType());
      dragonExists = fbdo.boolData();
    }
    else {
      Serial.println("FAILED TO FETCH EXISTS");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.getInt(fbdo, "/cleanlinessLevel")){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.stringData());
//      Serial.println("TYPE: " + fbdo.dataType());
      cleanlinessLevel = fbdo.intData();
    }
    else {
      Serial.println("FAILED TO FETCH CLEANLEVEL");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.getInt(fbdo, "/health")){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.stringData());
//      Serial.println("TYPE: " + fbdo.dataType());
      health = fbdo.intData();
    }
    else {
      Serial.println("FAILED TO FETCH HEALTH");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.getBool(fbdo, "/threwBall")){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.stringData());
//      Serial.println("TYPE: " + fbdo.dataType());
      threwBall = fbdo.boolData();
    }
    else {
      Serial.println("FAILED TO FETCH THREW BALL");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.getBool(fbdo, "/threwFood")){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.stringData());
//      Serial.println("TYPE: " + fbdo.dataType());
      threwFood = fbdo.boolData();
    }
    else {
      Serial.println("FAILED TO FETCH THREW FOOD");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.getBool(fbdo, "/usedMop")){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.stringData());
//      Serial.println("TYPE: " + fbdo.dataType());
      usedMop = fbdo.boolData();
    }
    else {
      Serial.println("FAILED TO FETCH USEDMOP");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}
