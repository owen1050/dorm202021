#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Room313";
const char* password = "12345678";
String Link = "http://127.0.0.1:23654";
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {  
  server.send(404, "text/plain", "notFound");
}

void postReq(String head, String cont)
{
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   HTTPClient http;    //Declare object of class HTTPClient
   http.begin("http://100.35.205.75:23653");      //Specify request destination
   http.addHeader(head, cont);
   int httpCode = http.POST("null");   //Send the request
   http.end();  //Close connection
   Serial.println("Reset:"+ head);
 }else{
    Serial.println("Error in WiFi connection");    
 }
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

  server.on("/lightsOn", []() {
    Serial.println("lightsOn");
    server.send(200, "text/plain", "lightsOn");
    //postReq("lightsOn", "0");
  });
  server.on("/lightsOff", []() {
    Serial.println("lightsOff");
    server.send(200, "text/plain", "lightsOff");
    //postReq("lightsOff", "0");
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  MDNS.update();
}
