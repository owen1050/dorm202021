#include <IRremoteESP8266.h>
#include <IRsend.h>

IRsend irsend(D2);

#define RAW_DATA_LEN 68
uint16_t projPowerIR[RAW_DATA_LEN]={8998, 4502, 562, 566, 562, 566, 562, 566, 562, 562, 562, 566, 562, 566, 534, 594, 562, 566, 562, 1682, 562, 1686, 562, 1682, 562, 1682, 562, 1686, 562, 566, 562, 1682, 562, 1686, 562, 562, 562, 566, 562, 1686, 562, 1682, 562, 1686, 562, 562, 534, 594, 562, 566, 562, 1682, 562, 1686, 562, 566, 562, 562, 566, 562, 534, 1714, 534, 1694, 582, 1682, 562, 1000};

void setup() {
 pinMode(D2, OUTPUT);

}

void loop() {
   irsend.sendRaw(projPowerIR, RAW_DATA_LEN, 36);
 // digitalWrite(D2, HIGH);
  delay(3000);
  //digitalWrite(D2, LOW);
  //delay(500);
  

}
