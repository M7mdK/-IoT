#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "DHT.h"
#include <Adafruit_NeoPixel.h>

#define DHTPIN 13
#define DHTTYPE DHT22 
#define PIN 12
//#define GPIO_NUM_33 15

const int grayButton = 15;
const int pinkButton = 32;
const int potPin = 14;    

int menuItem = 0;
int selectedItem = 0;
long potValue = 0; 
boolean tempType = false; //degre C=false , F=true
boolean lightIsOn = false;

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
DHT dht(DHTPIN, DHTTYPE);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);


void setup(){
  pinMode(grayButton, INPUT_PULLUP);
  pinMode(pinkButton, INPUT_PULLUP);
  pinMode(potPin, INPUT); 
  dht.begin();
  pixels.begin();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0);
  //esp_sleep_enable_timer_wakeup(5000);
}

void loop(){

  if(digitalRead(grayButton) == LOW){
    wakeUp();
    if(menuItem == 0){
      selectedItem = 0;
      display.print("Press to Display Humidity and Tempareture");
    }
    else if(menuItem == 1){
      selectedItem = 1;
      display.print("Change Temp C/F.");
    }
    else if(menuItem == 2){
     selectedItem = 2;
     display.print("Turn Light ON/OFF");
    }
    else if(menuItem == 3){
      selectedItem = 3;
      display.print("Set Light Color.");
    }
    else if(menuItem == 4){
      selectedItem = 4;
      display.print("Go To Deep Sleep.");
    }
    menuItem = (menuItem + 1)%5;
    display.display();
    delay(200);
  }

  if(digitalRead(pinkButton) == LOW){
    
    if(selectedItem == 0){
      print_hum_temp();
    }
    else if(selectedItem == 1){
      change_C_F();
    }
    else if(selectedItem == 2){
      turn_on_off_light();
    }
    else if(selectedItem == 3){
      setLightColor();
    }
    else if(selectedItem == 4){
      goToDeepSleep();
    }
    delay(200);
  }
  
}


void wakeUp(){
  display.begin(0x3C, true);

  display.clearDisplay();  //Clear the buffer.
  //display.display();

  display.setRotation(1); //Portrait display

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
}

void print_hum_temp(){

  display.clearDisplay(); 
  display.setCursor(0,0);
  
  delay(2000);  // Wait a few seconds between measurements.

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();  //C
  float f = dht.readTemperature(true);  //F

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

    display.print("Humidity: ");
    display.print(h);
    display.println("%");
    //Everytime I will check tempType to see if it is C or F (default is C):
    display.print("Temperature: ");
    if(!tempType){  //C
      display.print(t);
      display.print("C");
    }
    else if(tempType){  //F
      display.print(f);
      display.print("F");
    }
    
    display.display();
}

void change_C_F(){
  display.clearDisplay(); 
  display.setCursor(0,0);
  
  if(tempType == false){  //It was C, changing to F
    tempType = true;
    display.print("Temp type changed to F succesfully!");
  }
  else if(tempType == true){ //It was F, changing to C
    tempType = false;
    display.print("Temp type changed to C succesfully!");
  }
    
  display.display();

}

void turn_on_off_light(){

      display.clearDisplay(); 
      display.setCursor(0,0);
    
      if(lightIsOn == false){
        lightIsOn = true;
        pixels.setPixelColor(0, pixels.Color(255,255,255));
        pixels.show();
        display.print("Light is ON");
      }
      else if(lightIsOn == true){
        lightIsOn = false;
        pixels.setPixelColor(0, pixels.Color(0,0,0));
        pixels.show();
        display.print("Light is OFF");
      }

      display.display();
}

void setLightColor(){

  display.clearDisplay(); 
  display.setCursor(0,0);

  if(lightIsOn == false){
    display.print("You Have To Turn The Light On First!");
    display.display();
    return;
  }
    
  potValue = analogRead(potPin);  // 0 <= potValue < 4096

  if(potValue >= 0 && potValue < 1400){
        pixels.setPixelColor(0, pixels.Color(255,0,0));
        pixels.show();
        display.print("Red Light");
  }
  else if(potValue >= 1400 && potValue < 2800){
        pixels.setPixelColor(0, pixels.Color(0,255,0));
        pixels.show();
        display.print("Green Light");
  }
  else if(potValue >= 2800 && potValue < 4096){
        pixels.setPixelColor(0, pixels.Color(0,0,255));
        pixels.show();
        display.print("Blue Light");
  }
  
  display.display();
  delay(200);
}

void goToDeepSleep(){
      display.clearDisplay(); 
      display.setCursor(0,0);
      display.print("Lets go to deep sleep");
      display.display();
      esp_deep_sleep_start();  
}
