// Include Required Libraries

#include <string.h>
#include <math.h>
#include "WiFi.h"
#include "Message/decoder.h"
#include "Message/send.h"
#include "Message/generator.h"
#include "Message/proccesor.h"
#include "secrets.h"
#define LED_BUILTIN 4  //33


WiFiClient client;
/*___Struct and Enum___*/

void initWifi() {
  // Connect to Wi-Fi
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(SSID);
  Serial.print(" ");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Wifi OK");

  Serial.print("Conneting to server ");
  while (!client.connect(HOST, PORT)) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connection OK");
}

void setup() {

  // Start Serial Monitor
  // Serial.begin(115200);
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("");
  Serial.println("____Setup____");
  initWifi();
  Serial.println("");
  digitalWrite(LED_BUILTIN, HIGH);
}


void sendData(TypeRequest actualType, uint) 
{
  uint8_t actualSize = getSizeType(actualType);
  uint8_t request[3 + actualSize];
  request[0] = START_REQUEST;
  request[1] = actualType;
  for (uint32_t i = 0; i < actualSize; i++) 
  {
    request[2 + i] = payload[i];
  }
  uint8_t sum = computeCheckSum(&payload[2], actualSize);
  request[sizeof(request) - 1] = sum;
}

void loop() {
  
}
