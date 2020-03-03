#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Room313";
const char* password = "12345678";
String Link = "http://127.0.0.1:23654";
IPAddress staticIP(192, 168, 212, 150); //ESP static ip
IPAddress gateway(192, 168, 212, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(192, 168, 212, 1);  //DNS

ESP8266WebServer server(80);

HTTPClient http;    //Declare object of class HTTPClient

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {  
  server.send(404, "text/plain", "notFound");
}

void postReq(String head, String cont)
{
  WiFiClient client; 
  const int httpPort = 23654; 
  if (!client.connect("127.0.0.1", httpPort)) 
  { 
    Serial.println("connection failed"); 
  } 
  HTTPClient http; 
  http.begin("127.0.0.1"); 
  http.addHeader(head, cont); 
  auto httpCode = http.POST("null"); 
  http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.hostname("light.arduino");
  //WiFi.config(staticIP, subnet, gateway, dns);
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
    postReq("lightsOn", "0");
  });
  server.on("/lightsOff", []() {
    Serial.println("lightsOff");
    server.send(200, "text/plain", "lightsOff");
    postReq("lightsOff", "0");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  MDNS.update();
}
