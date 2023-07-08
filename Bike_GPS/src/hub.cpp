 #include "hub.h"

DynamicJsonDocument doc(2048);
DynamicJsonDocument directionsDoc(7269); 
DynamicJsonDocument coordinatesdoc(2048); 


void Hub::setup() {
   
  Serial.println("Starting connecting WiFi.");
  int n = WiFi.scanNetworks();
      for (int i = 0; i < n; ++i) {
        Serial.println(WiFi.SSID(i));
      }

  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
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


String Hub::getAdress(String lng, String lat) { 
 
   String request = "https://api.openrouteservice.org/geocode/reverse?api_key=#&point.lon=" +lng+"&point."+"lat="+lat + "&size=0";

 
  //Parse JSON, read error if any
  //Wait two seconds for next joke
   http.begin(request);
  //Use HTTP GET request
  http.GET();
  //Response from server
  String response = http.getString();
  //Parse JSON, read error if any
  deserializeJson(doc, response);
  
  //Close connection  
  http.end();
  
  JsonObject features_0 = doc["features"][0];
  JsonObject features_0_properties = features_0["properties"];
  //Serial.println(response);
  
  //Print parsed value on Serial Monitor
 String features_0_properties_name = features_0_properties["name"]; 
  //Wait two seconds for next joke
  return features_0_properties_name;
}

 
String Hub::getDirections(String start[2], String destination[2], String current_location){
// start is staring location, destination is end location, step is the number of the step you want to access
//output is what you want to return. options are: instruction (1), name (2), way_points (3), coordinates (4)
// if waypoints is entered as the output then the way point number should be 0 or 1 and the output will be the number of the start (0) or end(1) way point for that step 
// if coordinates is entered as the outpout then the way point number can be any number and it will return the longitude and latitude of theat number way point (steps must be zero or 1 t get long or lat) 
 
String directionsRequest = "https://api.openrouteservice.org/v2/directions/driving-car?api_key=5b3ce3597851110001cf6248195201672845469886f81046422ae9ef&start="+start[0]+","+ start[1]+"&end="+destination[0]+","+ destination[1];
 String finalOutput = "";
  http.begin(directionsRequest);
  http.GET();
  String Directionsresponse = http.getString();

  deserializeJson(directionsDoc, Directionsresponse);
  
  //int WantedStep = step; 
  //int WantedWaypoint = waypoint;    
  
  JsonObject features_0 = directionsDoc["features"][0];
  const char* features_0_type = features_0["type"]; // "Feature"
  JsonObject features_0_properties = features_0["properties"];
  JsonObject features_0_properties_segments_0 = features_0_properties["segments"][0];
  
  JsonArray features_0_properties_segments_0_steps = features_0_properties_segments_0["steps"];
  int numOfSteps = features_0_properties_segments_0_steps.size(); 

  //if input name = instruction or name or way point 
  //return 
  String names[numOfSteps];
  String instructions[numOfSteps];
  int counter = 0;
  
 
  http.end();
  for (JsonObject elem : features_0_properties_segments_0["steps"].as<JsonArray>()) {

  float distance = elem["distance"]; // 1.9, 314, 737.6, 264.3, 155.3, 130.8, 0
  float duration = elem["duration"]; // 0.5, 75.4, 106.2, 38.5, 14, 31.4, 0
  int type = elem["type"]; // 11, 3, 1, 0, 5, 0, 10
  String instruction = elem["instruction"]; // "Head west on Gerhart-Hauptmann-Straße", "Turn sharp ...
  instructions[counter] = instruction;
  String name = elem["name"]; // "Gerhart-Hauptmann-Straße", "Wielandtstraße", "Mönchhofstraße", ...
  names[counter] = name;
  int way_points_0 = elem["way_points"][0]; // 0, 1, 11, 39, 55, 59, 62
  int way_points_1 = elem["wa3y_points"][1]; // 1, 11, 39, 55, 59, 62, 62
  counter++;
}
for(int i = 0; i < numOfSteps;i++){
  if (current_location.indexOf(simpleDirection(names[i])) != -1){
    if(i != numOfSteps - 1){
    finalOutput = instructions[i] + " towards " + names[i+1]; 
    }
  }
}
return finalOutput; 
}

void Hub::getCoordinates(String location){
 
 String coordinatesrequest = "https://api.openrouteservice.org/geocode/search?api_key=5b3ce3597851110001cf6248195201672845469886f81046422ae9ef&text="+ location;
  http.begin(coordinatesrequest);
  
  http.GET();
  
  String coordinatesresponse = http.getString();

  deserializeJson(coordinatesdoc, coordinatesresponse);
  
  JsonObject coordinates = coordinatesdoc["features"][0];
  const char* coordinates_type = coordinates["type"]; // "Feature"

  const char* coordinates_geometry_type = coordinates["geometry"]["type"]; // "Point"

  String coordinates_geometry_coordinates_0 = coordinates["geometry"]["coordinates"][0]; // -71.09144
  String coordinates_geometry_coordinates_1 = coordinates["geometry"]["coordinates"][1]; // 42.27461

http.end();
destinationCoordinates[0] = coordinates_geometry_coordinates_0;
destinationCoordinates[1]  =  coordinates_geometry_coordinates_1;

}

String Hub::simpleDirection(String direction){
  int index = 0;
  for(int i = 0;i < direction.length();i++){
      if(direction[i] == ','){
        index = i;
      }
  }
  String simple;

  if(index != 0){
    simple = direction.substring(0,index -1);
  }else{
    simple =direction;
  }
  return direction.substring(0,index -1);
}
