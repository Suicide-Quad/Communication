#include <ESP8266WiFi.h>


// WiFi Router Login - change these to your router settings
const char* SSID = "vym";
const char* password = "rozavel29";
char response[] = "HTTP/1.1 200 OK\r\n\r\n12:45";

WiFiServer WebServer(80);
// Web Client
WiFiClient client;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  Serial.begin(9600);

  Serial.println("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.println(WiFi.localIP());

  WebServer.begin();

  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  client = WebServer.available();

  if (!client)
  {
    return;
  }

  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r\n');
  Serial.println(request);
  client.flush();

  client.printf("%s",response);
     
}
