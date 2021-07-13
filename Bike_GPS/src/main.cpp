#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <LiquidCrystal.h>
#include "prox.h"
#include "hub.h"
#include "BlynkSimpleEsp32.h"

WidgetMap myMap(V0); 
float lat ,lon;
TinyGPSPlus gps;
String  lat_str = "48.85756" , lng_str = "2.34280";
#define RXD2 18
#define TXD2 17
float latitude = 75.0;
float longitude = 35.0;
HardwareSerial SerialGPS(1);
LiquidCrystal lcd(0, 4, 5, 22, 19, 21);
Prox prox_sensor;
Hub hub;
char data[100];
char auth[] = " d8JVu9muQYonDxIKe1tPNPlUa_BDrhz5";
void setup() {
  Serial.begin(115200); 
prox_sensor.setup();
hub.setup();
// connect serial 
Serial.println("The GPS Received Signal:"); 
SerialGPS.begin(9600, SERIAL_8N1, RXD2, TXD2);
lcd.begin(16, 2);
 Blynk.begin(auth, hub.ssid, hub.password);
 Blynk.virtualWrite(V0, "clr"); 
}


void loop() {

 prox_sensor.detectionAlert();
  //sprintf(data, "{\"lat\": \"%s\", \"lon\": \"%s\"}", lat_str, lng_str); 
        //hub.loop(data);
        Serial.println(hub.getAdress(lat_str,lng_str));
 while (SerialGPS.available() > 0) {
   prox_sensor.detectionAlert();
    //sprintf(data, "{\"lat\": \"%s\", \"lon\": \"%s\"}", lat_str, lng_str); 
        //hub.loop(data);
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
         lcd.setCursor(0, 0);
       // lcd.print("Lat = ");
        lcd.print(hub.getAdress(lat_str,lng_str));
        //lcd.print(lat_str);
       
       // lcd.setCursor(0,1);
       // lcd.print("Long = ");
        //lcd.print(lng_str);
        sprintf(data, "{\"lat\": \"%s\", \"lon\": \"%s\"}", lat_str, lng_str); 
        
        Blynk.virtualWrite(V0, 1, latitude, longitude, "Location");
      }
       lcd.setCursor(16,1);
      lcd.autoscroll();    // Set diplay to scroll automatically
      lcd.print(" ");      // set characters
      delay(700);
      //lcd.clear();
    }
     
 }
  
  Blynk.run();
  
     
        
}