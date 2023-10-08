#include <FirebaseESP8266.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>
// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#define FIREBASE_HOST "https://dragonotchi-default-rtdb.firebaseio.com"                         
//database api url
#define API_KEY "pWoIw4pHYnPwPbn82trjowwt5VOHuJdt2KkW0Z7o"           
//database secret

#define FIREBASE_PROJECT_ID "dragonotchi"
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-lwanp@dragonotchi.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCVmsLtm0U8/96V\njSpsDxmWRziJy7hJlcbwbvUwzYOA9V/0HUeffN9LF1NPIIm2sExTbyMncWNGy0oI\ngZf7ynyGtVxRi5G5J5pXJLo1imP3NAYkJsgqEm4vrPEC7nJPcPKeZARfbPCOF/pf\nJbXcAhargbKhalmL2ApDnK1wj16P0xF8C6j5ZH2D3w/wpC8k3a84rWLTkdoRUX63\n0SRLC+xiJGfHSq8wIy5SSZIafzHcthB9T14ApdfJyAwYaLc3UO7G+zAdU6doiW9p\nkK+/9GT3kADRTygLGYS9YMmwpXoxOQf3xsKowsn7QQg7rw6iKPF84C5rKDe8gA9Q\nhNOwb/9NAgMBAAECggEACv3O2oQw5xVpdhVkC4Jkf90Fs2ILTSS69AlC7G38/mmI\nvje0fmw/P5YcIdEjLQ/w26Rrsccr4xSmgM98qhwrX/5NJRSbBT56oXJqV+quA4iH\n9vn+dNDb1ZbgMjXwohOLT8XLaHNpjRcH5xuvK9XfcoRZ6nAU2ihqQKNvPrjZ2rkr\nauhaXqqbDCNsZfThYN3aqBzWMli/Q8o+H0ANnbsvX+cyLOWmQhh3VOXPC4dYMf8v\ncU18v+LGh8lywHQMT/rJ85kvSItmwax5iLO7rzLQbMbUdmTcLUKAuVaMOwmOX6d+\neHAChTeykJAjSyil17qtE1OEj2vUECs+g4DcRxhgQQKBgQDEkuWE8Xwvoj4Ew/R1\n6W1zA85UJA9WdqhTCU73wDHooIBDKjJmnFTvpAKbrzI6h0FgbQfVneGP76I4GMNk\n9Ywo/DXmDnOEgTwXZLOpjQ1tyO0opznKWA2ZIC5l8UPya5dW1AFAx/hs+h2ojmsX\nBklHTRR0e/j6f38XnQx1mbksfQKBgQDC1Nnv+quModtJTooCnckMuuEq6Fk5TRAb\nYuU7R/pVrq7uiaUdeOrusq6/esBoIu0/YFpZ5dldrIgNbKYvN9/JA8I2PbuKk1a+\nTaKSq5Cx+vGtuZ4ihJhxLbDYGXUgkDSjsSuyaFrvu2YNkidBwc62//eKxzw9SqbC\nfn7rMmEnEQKBgGQJfdSSMGjtYDJ8psP670BBB5e6YjPWXwMjEZTzsnJ4e82yg8PE\ne1UZWlpwZldyzcqMshf8KPZfNWbm80lFzPZuo99YK2Gbg0FEdkorBlKAlnDB6Z8Y\nAJOU1Qf8KGAOcQ43Bgw5uCt9fepxz/3/rPskNuywypxwlfUmb1jJynrhAoGAQpa7\nEfErtroPF6P3OWOyNl6c3Kxi6lmzWpjXXgaAwc4dTbvPqh4MtZ5uaVVL84uUaKfG\n2ZjcTM7OBUCQH+1J2Asn0qAQvBEVgZ9JJDXtE1eASG77ZDLLY0tJeOJijMmcpCHG\nnIhcJDl5bMP6FNhnjVY4xb7IkO41xp2zRTGH0PECgYEAigXKJEUTIWp9NCwtv5Se\nN978BEdjfIH2MsKPdP1wa/GPQdtWsE94xV/qF6VvO7QwxWBkXt5IB0LvWfNJKFNF\nEn2eDnR7TdEtIbOcNAg/OMU0/bLi4SDev8acGsFc6/WaGPTqY7ZL0578Ca00qnIj\n/JdTQOvVi3bRVv1MxfyGryg=\n-----END PRIVATE KEY-----\n";

/* 4. Define the Firebase Data object */
FirebaseData fbdo;
FirebaseData stream;

/* 5. Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* 6. Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long lastTime = 0;

String parentPath = "/";
String childPath[4] = {"/boredomLevel", "/cleanlinessLevel", "/dragonExists", "/health"};

int count = 0;

volatile bool dataChanged = false;

void sendMessage();


void streamCallback(StreamData data) {
  Serial.println("ayy lmao");
// Multipath stream data
// get size of each node in bytes
//  size_t numChild = sizeof(childPath) / sizeof(childPath[0]);
//
//  for (size_t i = 0; i < numChild; i++)
//  {
//    if (stream.get(childPath[i]))
//    {
//      Serial.printf("path: %s, event: %s, type: %s, value: %s%s", stream.dataPath.c_str(), stream.eventType.c_str(), stream.type.c_str(), stream.value.c_str(), i < numChild - 1 ? "\n" : "");
//    }
//  }

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
    
    // Firebase.setStreamCallback(fbdo, streamCallback, streamTimeoutCallback);
    
//    if (!Firebase.beginStream(stream, "/"))
//    {
//      // Could not begin stream connection, then print out the error detail
//      Serial.println(stream.errorReason());
//    }

}

void loopFirebase() {
  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  // Firebase.runStream();
  
  if (Firebase.ready() && (millis() - lastTime > 1000 || lastTime == 0))
  {
      lastTime = millis();
      count++;
      
      FirebaseJson json;
      json.add("data", "hello");
      json.add("num", count);
      Serial.printf("Set json... %s\n\n", Firebase.setJSON(fbdo, "/test/stream/data/json", json) ? "ok" : fbdo.errorReason().c_str());

      sendMessage();
  }

//  if (dataChanged)
//  {
//    dataChanged = false;
//    Serial.println("Data changed");
//    // When stream data is available, do anything here...
//  }
//  if (Firebase.ready()) {  
//    if (!Firebase.readStream(stream))
//    {
//      Serial.println(fbdo.errorReason());
//    }
//  
//    if (stream.streamTimeout())
//    {
//      Serial.println("Stream timeout, resume streaming...");
//      Serial.println();
//    }
//  
//  //
//    if (stream.streamAvailable()) {
//      if (stream.dataTypeEnum() == firebase_rtdb_data_type_integer)
//        Serial.println(stream.to<int>());
//      else if (stream.dataTypeEnum() == firebase_rtdb_data_type_float)
//        Serial.println(stream.to<float>(), 5);
//      else if (stream.dataTypeEnum() == firebase_rtdb_data_type_double)
//        printf("%.9lf\n", stream.to<double>());
//      else if (stream.dataTypeEnum() == firebase_rtdb_data_type_boolean)
//        Serial.println(stream.to<bool>() ? "true" : "false");
//      else if (stream.dataTypeEnum() == firebase_rtdb_data_type_string)
//        Serial.println(stream.to<String>());
//    }
//    else if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_json)
//    {
//        FirebaseJson *json = fbdo.to<FirebaseJson *>();
//        Serial.println(json->raw());
//    }
//    else if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_array)
//    {
//        FirebaseJsonArray *arr = fbdo.to<FirebaseJsonArray *>();
//        Serial.println(arr->raw());
//    }
//  }
//  }
}

void sendMessage() {
    // Write an Int number on the database path test/int
//    if (Firebase.setBool(fbdo, "/dragonExists", true)){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.dataPath());
//      Serial.println("TYPE: " + fbdo.dataType());
//    }
//    else {
//      Serial.println("FAILED");
//      Serial.println("REASON: " + fbdo.errorReason());
//    }
    
    // Write an Float number on the database path test/float
//    if (Firebase.setString(fbdo, "/boredomLevel", "5")){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.dataPath());
//      Serial.println("TYPE: " + fbdo.dataType());
//    }
//    else {
//      Serial.println("FAILED");
//      Serial.println("REASON: " + fbdo.errorReason());
//    }
    
    // Write an Float number on the database path test/float
    if (Firebase.getString(fbdo, "/boredomLevel")){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.stringData());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}
