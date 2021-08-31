#include <Arduino.h>
#include <WiFi.h>
#include "Esp32MQTTClient.h"
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include <esp_wifi.h>
/*#define INTERVAL 10000
#define DEVICE_ID "Esp32Device"
#define MESSAGE_MAX_LEN 256*/

class Hub{
// Please input the SSID and password of WiFi
private:
uint8_t newMACAddress[6] = {0x90,0x32,0x4B,0xAF,0x2B,0x15};
   HTTPClient http;
bool hasIoTHub = false;
/*String containing Hostname, Device Id & Device Key in the format:                         */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
public:
     const char* connectionString = "HostName=BikePal.azure-devices.net;DeviceId=ESP-PAL;SharedAccessKey=7IrpFWkMjrOcBJzSjaiccO7cNmYxg/EChTA04xD/aqA=";
      const char* ssid     = "Samsung Galaxy A20 6416";
    const char* password = "loverboy";
    String destinationCoordinates[2];
    void setup();
    String getAdress(String lng, String lat);
    //String for storing server response
    
    //JSON document
    //DynamicJsonDocument doc(2048);
  // String getPoints(int point); //input the step number (indexing from zero) that you want in the steps 
   String getDirections (String start[2], String destination[2], String current_location); 
   void getCoordinates(String location);
private:
   String simpleDirection(String direction);
   
   

};
