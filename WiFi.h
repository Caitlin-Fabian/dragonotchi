#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "Credentials.h"
#include "Dragonotchi.h"

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
            
            String action = doc["action"];
            String item = doc["item"];

            Serial.printf("Action: %s\n", action);
            Serial.printf("Item: %s\n", item);
            break;
          }
        case WStype_BIN:
            USE_SERIAL.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);
            break;
    }
}

void setupWifi() {
  wifiMulti.addAP(ssid, password);

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  
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

  // startWebSocket();
}

void setupServer() {
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");      
  delay(1000);
}

void startWebSocket() {
  websocketServer.begin();
  websocketServer.onEvent(webSocketEvent);
}

void handleRoot() {
  if (health <= 0) {
    server.send(200, "text/html", "<p>I DIED :(</p>");
  }
  else if (cleanlinessLevel < 5) {
    server.send(200,  "text/html","<p>DRAGON POOPY!!!!</p>");
  } else 
  if (boredomLevel < 5) {
    server.send(200, "text/html", "<p>DRRAGON BORED.</p>");
  }else 
  if (threwBall) {
    server.send(200, "text/html", "<p>THAT WAS FUN THANK YOU!!</p>");
  }else 
  if (threwFood) {
    server.send(200,  "text/html","<p>YUMMY THANK YOU!!</p>");
  } else if (hungerLevel < 5) {
    
    server.send(200, "text/html", "<p>DRAGON HUNGRY!!!!!</p>");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}
