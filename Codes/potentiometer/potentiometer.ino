#include "WiFi.h"
#include "HTTPClient.h"
#include "ThingSpeak.h"
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "0SJWL7WQBOKSDG72";
WiFiClient  client;

const char ssid[] = "NETGEAR95";
const char pass[] =  "purpleunicorn212";
 int potPin = 14; //PotentioMeter connected

const String serverAddress = "https://192.168.1.149/api/9VMYGIRVu4kL-eEXbLiGgAGRoe7RIQOQ3TOaGRIH/";  // server address

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

void setup() {
 
  Serial.begin(115200);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
 
  WiFi.begin(ssid, pass); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
int sensorValue = analogRead(36);
 sensorValue=sensorValue *254;
  sensorValue=sensorValue /4095; // convert the pot value into 0 to 254

  //Serial. println(Var/16.12); //Converting the The Max Potentio meter to 254 the required LED Brithness value
 //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) 
  {
 if(WiFi.status()== WL_CONNECTED){
 
   HTTPClient http;   
   String Get_all_lights = serverAddress + "/lights";  //Details of all lights are qued
     // Your Domain name with URL path or IP address with path
   http.begin(Get_all_lights.c_str()); // get all the Lights satus
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
       
 
   http.begin(serverAddress + "lights/5/state");
   http.addHeader("Content-Type", "text/json");            
   String Value = "{\"sat\":"+ String(sensorValue) + "}";  //control the RGB value with Potentiometer
  Serial. println(sensorValue);
   int httpResponseCode = http.PUT(Value);   
 
   if(httpResponseCode>0){
 
    String response = http.getString();   
 
    Serial.println(httpResponseCode);
    Serial.println(response);          
 
   }else{
 
    Serial.print("Error on sending PUT Request: ");
    Serial.println(httpResponseCode);
 
   }
 
 }else{
    Serial.println("Error in WiFi connection");
 }

 
  lastTime = millis();
  delay(1000);
}
}
