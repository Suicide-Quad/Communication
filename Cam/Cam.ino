// Include Required Libraries


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <string.h>
#include <math.h>
#include "WiFi.h"
#include "Message/decoder.h"
#include "Message/send.h" 
#include "Message/generator.h"
#include "Message/proccesor.h"
#include "secrets.h" 
#define LED_BUILTIN 4 //33


WiFiClient client;
/*___Struct and Enum___*/

void initWifi()
{
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
  while(!client.connect(HOST, PORT))
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

  pinMode(LED_BUILTIN,OUTPUT);
  Serial.println("");
  Serial.println("____Setup____");
  initWifi();
  Serial.println("");
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() 
{
      
}
