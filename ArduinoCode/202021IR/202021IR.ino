#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender

IRsendRaw mySender;

#define RAW_DATA_LEN 68
uint16_t projPowerIR[RAW_DATA_LEN]={8998, 4502, 562, 566, 562, 566, 562, 566, 562, 562, 562, 566, 562, 566, 534, 594, 562, 566, 562, 1682, 562, 1686, 562, 1682, 562, 1682, 562, 1686, 562, 566, 562, 1682, 562, 1686, 562, 562, 562, 566, 562, 1686, 562, 1682, 562, 1686, 562, 562, 534, 594, 562, 566, 562, 1682, 562, 1686, 562, 566, 562, 562, 566, 562, 534, 1714, 534, 1694, 582, 1682, 562, 1000};
//uint16_t tvPower[RAW_DATA_LEN]={9002, 4510, 474, 654, 534, 1698, 558, 570, 474, 654, 534, 570, 558, 570, 558, 570, 558, 574, 530, 1698, 498, 630, 534, 1698, 558, 1702, 534, 1698, 470, 1762, 558, 1698, 534, 594, 530, 574, 558, 1698, 534, 1698, 554, 574, 558, 570, 558, 546, 558, 1698, 534, 566, 590, 1670, 450, 678, 534, 570, 558, 1698, 558, 1674, 558, 1698, 534, 574, 554, 1702, 530, 1000};
uint16_t volumeUpIR[RAW_DATA_LEN]={8958, 4518, 530, 1702, 530, 582, 550, 1702, 530, 598, 530, 602, 530, 598, 530, 602, 530, 1702, 530, 1702, 530, 602, 530, 1706, 526, 1706, 530, 598, 530, 1706, 530, 1702, 530, 1702, 530, 1706, 530, 598, 530, 602, 530, 598, 530, 602, 530, 1702, 530, 1706, 526, 602, 530, 598, 530, 1706, 530, 1702, 530, 1702, 530, 1706, 530, 598, 530, 602, 530, 1702, 530, 1000};
uint16_t volumeDownIR[RAW_DATA_LEN]={8986, 4486, 562, 1674, 530, 602, 554, 1678, 558, 570, 558, 574, 558, 570, 558, 574, 558, 1674, 558, 1674, 562, 570, 558, 1674, 562, 1674, 558, 570, 558, 1674, 558, 1678, 558, 1674, 558, 574, 530, 598, 558, 574, 558, 570, 558, 574, 558, 1674, 558, 1674, 558, 574, 558, 1674, 562, 1670, 562, 1674, 558, 1674, 562, 1674, 558, 570, 530, 602, 558, 1658, 546, 1000};
uint16_t auxOneIR[RAW_DATA_LEN]={8990, 4462, 582, 1674, 558, 574, 558, 1654, 550, 598, 558, 574, 558, 570, 530, 602, 558, 1674, 558, 1674, 562, 570, 558, 1674, 558, 1678, 558, 570, 558, 1678, 558, 1674, 558, 1678, 554, 574, 558, 1674, 558, 1678, 558, 1674, 530, 602, 558, 1674, 558, 1674, 558, 574, 558, 1674, 558, 574, 558, 570, 558, 570, 534, 1702, 558, 570, 562, 570, 558, 1674, 558, 1000};
uint16_t auxTwoIR[RAW_DATA_LEN]={8986, 4486, 558, 1674, 530, 602, 558, 1674, 558, 570, 562, 570, 558, 574, 558, 570, 558, 1674, 562, 1674, 558, 570, 558, 1678, 558, 1674, 558, 574, 530, 1702, 530, 1702, 530, 1706, 530, 1702, 530, 1686, 550, 1702, 558, 1674, 558, 574, 530, 1682, 550, 1706, 526, 602, 530, 602, 558, 570, 530, 602, 530, 598, 558, 1678, 526, 602, 530, 598, 530, 1706, 530, 1000};
uint16_t muteIR[RAW_DATA_LEN]={8986, 4486, 582, 1650, 586, 546, 586, 1646, 586, 546, 558, 570, 558, 574, 582, 546, 586, 1650, 558, 1674, 558, 570, 586, 1650, 558, 1674, 558, 574, 558, 1674, 558, 1678, 582, 1650, 582, 546, 586, 1650, 582, 546, 558, 1678, 566, 562, 558, 1674, 558, 1678, 582, 546, 586, 1650, 582, 546, 558, 1674, 562, 570, 558, 1674, 582, 550, 558, 570, 586, 1650, 558, 1000};
uint16_t cdIR[RAW_DATA_LEN]={8986, 4486, 558, 1678, 554, 574, 558, 1674, 558, 574, 526, 602, 558, 574, 526, 602, 558, 1678, 558, 1674, 558, 570, 562, 1674, 554, 1678, 558, 574, 554, 1678, 558, 1674, 558, 1678, 558, 570, 558, 1678, 558, 1674, 558, 574, 554, 1678, 558, 1678, 554, 1658, 578, 570, 558, 1658, 578, 570, 558, 574, 554, 1678, 558, 574, 554, 574, 558, 570, 558, 1678, 554, 1000};
uint16_t phonoIR[RAW_DATA_LEN]={8958, 4514, 530, 1706, 530, 598, 530, 1706, 526, 602, 530, 598, 530, 602, 530, 598, 530, 1706, 530, 1702, 530, 582, 550, 1702, 530, 1702, 530, 602, 530, 1702, 530, 1706, 526, 1706, 530, 602, 530, 1702, 530, 598, 534, 598, 530, 1702, 530, 1706, 530, 1702, 530, 602, 530, 1702, 530, 602, 526, 1706, 530, 1702, 530, 582, 550, 598, 530, 602, 530, 1702, 530, 1000};

int projPower = 0;
//int tvPower = 0;
int volumeUp = 0;
int volumeDown = 0;
int auxOne = 0;
int auxTwo = 0;
int mute = 0;
int cd = 0;
int phono = 0;
bool irBlast = false;

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
    projPower = 1;
  });  

  server.on("/projectorOff", []() {
    Serial.println("projectorOff");
    server.send(200, "text/plain", "projectorOff");
    projPower = 3;
  });

  server.on("/volumeUp2", []() {
    Serial.println("volumeUp2");
    server.send(200, "text/plain", "volumeUp2");
    volumeUp = 2;
  });

  server.on("/volumeDown2", []() {
    Serial.println("volumeDown2");
    server.send(200, "text/plain", "volumeDown2");
    volumeDown = 2;
  });

  server.on("/volumeUp5", []() {
    Serial.println("volumeUp5");
    server.send(200, "text/plain", "volumeUp5");
    volumeUp = 5;
  });

  server.on("/volumeDown5", []() {
    Serial.println("volumeDown5");
    server.send(200, "text/plain", "volumeDown5");
    volumeDown = 5;
  });

  server.on("/auxOne", []() {
    Serial.println("auxOne");
    server.send(200, "text/plain", "auxOne");
    auxOne = 2;
  });

  server.on("/auxTwo", []() {
    Serial.println("auxTwo");
    server.send(200, "text/plain", "auxTwo");
    auxTwo = 2;
  });

  server.on("/phono", []() {
    Serial.println("phono");
    server.send(200, "text/plain", "phono");
    phono = 2;
  });

  server.on("/mute", []() {
    Serial.println("mute");
    server.send(200, "text/plain", "mute");
    mute = 1;
  });

  server.on("/cd", []() {
    Serial.println("cd");
    server.send(200, "text/plain", "cd");
    cd = 2;
  });
}

void loop() {

  server.handleClient();
  MDNS.update();

  irBlast = false;

  if(projPower != 0 and irBlast == false)
  {
    projPower--;
    irBlast = true;
    mySender.send(projPowerIR, RAW_DATA_LEN, 36);
  }

  if(volumeUp != 0 and irBlast == false)
  {
    volumeUp--;
    irBlast = true;
    mySender.send(volumeUpIR, RAW_DATA_LEN, 36);
  }

  if(volumeDown != 0 and irBlast == false)
  {
    volumeDown--;
    irBlast = true;
    mySender.send(volumeDownIR, RAW_DATA_LEN, 36);
  }

  if(auxOne != 0 and irBlast == false)
  {
    auxOne--;
    irBlast = true;
    mySender.send(auxOneIR, RAW_DATA_LEN, 36);
  }

  if(auxTwo != 0 and irBlast == false)
  {
    auxTwo--;
    irBlast = true;
    mySender.send(auxTwoIR, RAW_DATA_LEN, 36);
  }

  if(mute != 0 and irBlast == false)
  {
    mute--;
    irBlast = true;
    mySender.send(muteIR, RAW_DATA_LEN, 36);
  }

  if(cd != 0 and irBlast == false)
  {
    cd--;
    irBlast = true;
    mySender.send(cdIR, RAW_DATA_LEN, 36);
  }

  if(phono != 0 and irBlast == false)
  {
    phono--;
    irBlast = true;
    mySender.send(phonoIR, RAW_DATA_LEN, 36);
  }

}
