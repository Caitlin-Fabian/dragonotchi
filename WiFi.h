#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "Credentials.h"
// #include <ArduinoWebsockets.h>

#include <WebSocketsServer.h>

#define USE_SERIAL Serial

// using namespace websockets;

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);
// WebsocketsServer websocketServer;
 WebSocketsServer websocketServer = WebSocketsServer(81);

const char* ssid = SSID;
const char* password = PASSWORD;
const char* domainName = "dragonotchi";

void handleRoot();
void handleNotFound();
void setupServer();
void startWebSocket();
void handleFeed();
void handlePlay();
void setupWifi();

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = websocketServer.remoteIP(num);
                USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
                // send message to client
                websocketServer.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
          {
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);

//            switch (doc["action"]) {
//              case "feed": {
//                if (doc["item"] == 
//              }
//            }
            String action = doc["action"];
            String item = doc["item"];

            Serial.printf("Action: %s\n", action);
            Serial.printf("Item: %s\n", item);

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
          }
        case WStype_BIN:
            USE_SERIAL.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }
}

void setupWifi() {
//    WiFi.softAP(ssid_ap, password_ap);
//    Serial.print(ssid_ap);
//    Serial.print(" started!");
//    Serial.print("IP address:\t");
//    Serial.println(WiFi.softAPIP());

    wifiMulti.addAP(ssid, password);
  
    while (wifiMulti.run() != WL_CONNECTED) {
      delay(1000);
      Serial.print('.');
    }

//  if(WiFi.softAPgetStationNum() == 0) {      // If the ESP is connected to an AP
//    Serial.print("Connected to ");
//    Serial.println(WiFi.SSID());             // Tell us what network we're connected to
//    Serial.print("IP address:\t");
//    Serial.print(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
//  } else {                                   // If a station is connected to the ESP SoftAP
//    Serial.print("Station connected to ESP8266 AP");
//  }
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());             // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.print(WiFi.localIP());  
  
  setupServer();


  if (MDNS.begin(domainName)) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80);

  startWebSocket();
}

void setupServer() {
  server.on("/", handleRoot);
  server.on("/feed", handleFeed);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");      
  delay(1000);
}

void startWebSocket() {
  websocketServer.begin();
  websocketServer.onEvent(webSocketEvent);
  //websocketServer.listen(81);
//  Serial.print("Is server live? ");
//  Serial.println(websocketServer.available());
}

void handleRoot() {
  server.send(200, "text/plain", "dragon!");
}

void handleFeed() {
  server.send(200, "text/plain", "fed the dragon!");
}

void handlePlay() {
  server.send(200, "text/plain", "increased dragon happiness!");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}
