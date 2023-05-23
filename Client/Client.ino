#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "secret.h"

#define LED_BUILTIN 2
#define BUFFERSIZE 128 

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.43.196:2048/";

String test = "23.4:234";


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    Serial.print(".");
    digitalWrite(LED_BUILTIN,LOW);
    delay(250);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW);
}

void sendRequest(String buffer)
{
  if (buffer == "" || buffer == "\n")
    return ;
  if(WiFi.status()== WL_CONNECTED){r
      /*Serial.println("Send'");
      Serial.print(buffer);
      Serial.print("'");*/
      WiFiClient client;
      HTTPClient http;
     

      String serverPath = serverName + '\n' + buffer + ';';
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}


void loop() {
  // Send an HTTP POST request depending on timerDelay
    //Check WiFi connection status

  String request = Serial.readStringUntil(';');
  String sum = Serial.readStringUntil('&');
  sum.replace(';',' ');
  Serial.println(request);
  Serial.println(sum);
  if (1)//request.length() > 1 && sum == checksum(request))
  {
    sendRequest(request);
    Serial.println(request);
    Serial.println("1");
  }
}
