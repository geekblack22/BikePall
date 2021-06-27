#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <LiquidCrystal.h>
float lat ,lon;
TinyGPSPlus gps;
String  lat_str , lng_str;
#define RXD2 18
#define TXD2 17
float latitude = 0.0;
float longitude = 0.0;
HardwareSerial SerialGPS(1);
LiquidCrystal lcd(0, 4, 5, 22, 19, 21);
void setup() {
Serial.begin(115200); // connect serial 
Serial.println("The GPS Received Signal:"); 
 SerialGPS.begin(9600, SERIAL_8N1, RXD2, TXD2);
lcd.begin(16, 2);


}
void loop() {

 
 while (SerialGPS.available() > 0) {
   
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

      }
      delay(1000);
      lcd.clear();
    }
     
 }
  
       
        
}