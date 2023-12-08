#include <NTPClient.h>
#include <WiFi.h> // for WiFi shield
#include <WiFiUdp.h>

const char ssid[] = "NETGEARS95";
const char pass[] = "purpleunicorn212";

WiFiClient  client;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//Get date/time from NTP server
void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  
  timeClient.begin();
}

void loop() {

  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  delay(100);
}
