#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

Servo hallServo;
Servo mainServo;

const char* ssid = "Room313";
const char* password = "12345678";
String Link = "http://127.0.0.1:23654";
ESP8266WebServer server(80);
int mainState = 0;
int hallState = 0;
unsigned long mainTime = 0;
unsigned long hallTime = 0;

int hallPosOff = 120;
int hallPosOn = 50;
int hallPosN = (int)((hallPosOn + hallPosOff)/2);
int mainPosOff = 20;
int mainPosOn = 94;
int mainPosN = (int)((mainPosOn + mainPosOff)/2);
int hallPos = 0;
int mainPos = 0;

int hallPin = 5;
int mainPin = 3;

void handleRoot() {
  server.send(200, "text/plain", "lightsAlive");
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
    mainState = 1;
    hallState = 1;
  });
  server.on("/lightsOff", []() {
    Serial.println("lightsOff");
    server.send(200, "text/plain", "lightsOff");
    mainState = -1;
    hallState = -1;
  });
  server.on("/mainOn", []() {
    Serial.println("mainOn");
    server.send(200, "text/plain", "mainOn");
    mainState = 1;
    
  });
  server.on("/mainOff", []() {
    Serial.println("mainOff");
    server.send(200, "text/plain", "mainOff");
    mainState = -1;
    
  });
  server.on("/hallOn", []() {
    Serial.println("hallOn");
    server.send(200, "text/plain", "hallOn");
    hallState = 1;
    
  });
  server.on("/hallOff", []() {
    Serial.println("hallOff");
    server.send(200, "text/plain", "hallOff");
    hallState = -1;
    
  });
  server.on("/mainOnHallOff", []() {
    Serial.println("mainOnHallOff");
    server.send(200, "text/plain", "mainOnHallOff");
    hallState = -1;
    mainState = 1;
    
  });
  server.on("/mainOffHallOn", []() {
    Serial.println("mainOffHallOn");
    server.send(200, "text/plain", "mainOffHallOn");
    hallState = 1;
    mainState = -1;
    
  });
  
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void moveMotors()
{
  if(hallState != 0)
  {
    if(hallState == 1)
    {
      hallPos = hallPosOn;
    }
    else
    {
      hallPos = hallPosOff;
    }
    hallTime = (int)(millis()/1000);
    hallState = 0;
    Serial.println("hall move");
  }
  if(mainState != 0)
  {
    if(mainState == 1)
    {
      mainPos = mainPosOn;
    }
    else
    {
      mainPos = mainPosOff;
    }
    mainTime = (int)(millis()/1000);
    mainState = 0;
    Serial.println("main move");
  }

  int ct = (int)(millis()/1000);

  if(ct - mainTime > 2)
  {
    mainPos = mainPosN;
  }
  if(ct - mainTime < 4)
  {
    Serial.println("attached main");
    mainServo.attach(mainPin);
    mainServo.write(mainPos);
  }
  else
  {
    mainServo.detach();
    Serial.println("detached main");
  }

  if(ct - hallTime >2)
  {
    hallPos = hallPosN;
  }
  if(ct - hallTime < 4
  )
  {
    Serial.println("attached hall");
    hallServo.attach(hallPin);
    hallServo.write(hallPos);
  }
  else
  {
    hallServo.detach();
    Serial.println("detached hall");
  }
}

void loop() {
  server.handleClient();
  MDNS.update();
  moveMotors();
  Serial.print("Hall Pos:");
  Serial.println(hallServo.read());
  Serial.print("Main Pos:");
  Serial.println(mainServo.read());
  delay(100);
}
