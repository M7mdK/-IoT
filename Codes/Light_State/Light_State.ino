#include <NTPClient.h>
#include <WiFi.h> // for WiFi shield
#include <WiFiUdp.h>
#include "HTTPClient.h"

const char ssid[] = "NETGEARS95";
const char pass[] = "purpleunicorn212";

const String serverAddress = "https://192.168.1.149/api/9VMYGIRVu4kL-eEXbLiGgAGRoe7RIQOQ3TOaGRIH/";  // server address
int port = 8080;

const int lightSensor = 34;

WiFiClient  client;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

float value = 0;

void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, pass); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}

//Getting light Status every 100ms
void loop() {

  //Get the current state of the light
 if(WiFi.status()== WL_CONNECTED){
 
   HTTPClient http;   
   String GetLightState = serverAddress + "/lights/5/state"; 
   http.begin(GetLightState.c_str());
   // Send HTTP GET request
      int httpResponseCode_1 = http.GET();
       if (httpResponseCode_1>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode_1);
        String payload = http.getString();
        Serial.println(payload);
       }
       else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode_1);
      }
       
  delay(100);
}
}
