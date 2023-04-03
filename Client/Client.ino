#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define BUFFERSIZE 128 
#define SSID "vym"
#define PASSWORD "rozavel29"

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.29.3:2048/";


void setup() {
  Serial.begin(9600); 

  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void sendRequest(String buffer)
{
  if (buffer == "" || buffer == "\n")
    return ;
  if(WiFi.status()== WL_CONNECTED){
      Serial.println("Send'");
      Serial.print(buffer);
      Serial.print("'");
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverName + '\n' + buffer + ':';
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      // Free resources
      http.end();
      delay(1000);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}

void loop() {
  // Send an HTTP POST request depending on timerDelay
    //Check WiFi connection status
  sendRequest(Serial.readStringUntil(';'));
  
}