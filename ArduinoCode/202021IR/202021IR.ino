#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Room313";
const char* password = "12345678";
String Link = "http://127.0.0.1:23654";
ESP8266WebServer server(80);
unsigned long updateT = millis();

void handleRoot() {
  server.send(200, "text/plain", "IRAlive");
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

  server.on("/projectorOn", []() {
    Serial.println("projectorOn");
    server.send(200, "text/plain", "projectorOn");
  });  
}

void loop() {

  server.handleClient();
  MDNS.update();

}
