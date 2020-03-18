#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <Ticker.h>
Ticker timer;

const char* ssid = "Room313";
const char* password = "12345678";
String Link = "http://127.0.0.1:23654";
ESP8266WebServer server(80);
unsigned long updateT = millis();

int blindState = 0;
unsigned long blindMoveTime = 0;
int blindUpTime = 10000;
int blindDownTime = 12000;
bool firstDown = false;

int motorPin = D5;
int switchPin = D2;
bool switchIsPressed = false;
int motorValue = 0;
int motorUpValue = 500;
int motorDownValue = 1500;
int motorStayValue = 1000;

void ICACHE_RAM_ATTR runPWM() {
  if(motorValue != motorStayValue)
  {
    digitalWrite(D5, HIGH);
    delayMicroseconds(motorValue);
    digitalWrite(D5, LOW);
  }
  else
  {
    digitalWrite(D5, LOW);
  }
  
  
  timer1_write(130000);//12us
}

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

  server.on("/stopBlinds", []() {
    Serial.println("stopedBlinds");
    server.send(200, "text/plain", "stopedBlinds");
    blindState = 0;
    motorValue = motorStayValue;
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  timer1_attachInterrupt(runPWM); // Add ISR Function
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(150000);
  timer.attach(0.03, runPWM);
}

void loop() {
  test();
}
void test(){
  server.handleClient();
  MDNS.update();

  if (millis() - updateT > 1000)
  {
    Serial.println("Alive");
    updateT = millis();
  }

  switchIsPressed = true;
  if (digitalRead(switchPin))
  {
    switchIsPressed = false;
  }
  //Serial.println(switchIsPressed);
  if (blindState == 1 and (millis() - blindMoveTime < blindUpTime and !switchIsPressed))
  {
    motorValue = motorUpValue;
  }

  if (blindState == 1 and (millis() - blindMoveTime > blindUpTime or switchIsPressed))
  {
    motorValue = motorStayValue;
    blindState = 0;
  }

  if (blindState == -1 and millis() - blindMoveTime < blindDownTime and !switchIsPressed)
  {
    motorValue = motorDownValue;
    firstDown = false;
  }

  if (blindState == -1 and (millis() - blindMoveTime > blindDownTime or switchIsPressed))
  {
    firstDown = true;
    motorValue = motorStayValue;
    blindState = 0;
  }
  
  if(firstDown)
  {
    blindState = 3;
  }

  if(blindState == 3)
  {
    if(!digitalRead(switchPin))
    {
      motorValue = motorUpValue;
    }
    else
    {
      blindState = 0;
    }
  }
  
  if (blindState == 0)
  {
    motorValue = motorStayValue;
  }
  
  Serial.println(motorValue);
  delay(50);

}
