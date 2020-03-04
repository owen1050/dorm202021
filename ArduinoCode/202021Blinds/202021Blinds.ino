#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Room313";
const char* password = "12345678";
String Link = "http://127.0.0.1:23654";
ESP8266WebServer server(80);

boolean blindState = 0;
unsigned long blindMoveTime = 0;
int blindUpTime = 10000;
int blindDownTime = 9000;
int motorPin = 0;
int switchPin = 0;

void handleRoot() {
  server.send(200, "text/plain", "blindsAlive");
  Serial.println("AliveCheckedByLocal");
}

void handleNotFound() {  
  server.send(404, "text/plain", "notFound");
  Serial.println("404");
}

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);

  pinMode(motorPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/openBlinds", []() {
    Serial.println("openBlinds");
    server.send(200, "text/plain", "blindsOpen");
    blindState = 1;
    blindMoveTime = millis();
  });

  server.on("/closeBlinds", []() {
    Serial.println("closeBlinds");
    server.send(200, "text/plain", "closeBlinds");
    blindState = -1;
    blindMoveTime = millis();
  });

  if(blindState == 1 and millis() - blindMoveTime < blindUpTime)
  {
    
  }
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
