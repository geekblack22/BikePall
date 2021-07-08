#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <LiquidCrystal.h>
#include "prox.h"
#include "hub.h"
float lat ,lon;
TinyGPSPlus gps;
String  lat_str = "75" , lng_str = "35";
#define RXD2 18
#define TXD2 17
float latitude = 75.0;
float longitude = 35.0;
HardwareSerial SerialGPS(1);
LiquidCrystal lcd(0, 4, 5, 22, 19, 21);
Prox prox_sensor;
Hub hub;
char data[100];

void setup() {
prox_sensor.setup();
hub.setup();
Serial.begin(115200); // connect serial 
Serial.println("The GPS Received Signal:"); 
SerialGPS.begin(9600, SERIAL_8N1, RXD2, TXD2);
lcd.begin(16, 2);
}


void loop() {

 prox_sensor.detectionAlert();
  sprintf(data, "{\"lat\": \"%s\", \"lon\": \"%s\"}", lat_str, lng_str); 
        hub.loop(data);
 while (SerialGPS.available() > 0) {
   prox_sensor.detectionAlert();
    sprintf(data, "{\"lat\": \"%s\", \"lon\": \"%s\"}", lat_str, lng_str); 
        hub.loop(data);
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
         lcd.setCursor(0, 0);
        lcd.print("Lat = ");
       
        lcd.print(lat_str);
       
        lcd.setCursor(0,1);
        lcd.print("Long = ");
        lcd.print(lng_str);
        sprintf(data, "{\"lat\": \"%s\", \"lon\": \"%s\"}", lat_str, lng_str); 
        hub.loop(data);
      }
      delay(1000);
      lcd.clear();
    }
     
 }
  
       
        
}