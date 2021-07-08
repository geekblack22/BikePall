#include "hub.h"

DynamicJsonDocument doc(2048);
String address;

void Hub::setup() {
  
  Serial.println("Starting connecting WiFi.");
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /*if (!Esp32MQTTClient_Init((const uint8_t*)connectionString))
  {
    hasIoTHub = false;
    Serial.println("Initializing IoT hub failed.");
    return;
  }
  hasIoTHub = true;*/
}

String Hub::getAdress(String lat, String lng) {
 
   String request = "https://atlas.microsoft.com/search/address/reverse/json?subscription-key=Ltpn8KxW4omY2KFRQ9gDMq1KObgUM_LARJkHThNiF_k&api-version=1.0&query="+lat+","+lng;
  /*Serial.println("start sending events.");
  if (hasIoTHub)
  {
   
    if (Esp32MQTTClient_SendEvent(buff))
    {
      Serial.println("Sending data succeed");
    }
    else
    {
      Serial.println("Failure...");
    }
    delay(5000);
  }*/
   http.begin(request);
  //Use HTTP GET request
  http.GET();
  //Response from server
  response = http.getString();
  //Parse JSON, read error if any
  //Wait two seconds for next joke
   http.begin(request);
  //Use HTTP GET request
  http.GET();
  //Response from server
  response = http.getString();
  //Parse JSON, read error if any
  DeserializationError error = deserializeJson(doc, response);
  //Print parsed value on Serial Monitor
  address = doc["street"].as<char*>();
  //Close connection  
  http.end();
  //Wait two seconds for next joke
  return response;


}