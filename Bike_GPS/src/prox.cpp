#include "prox.h"


//proximity sensor is pin 34 
int signalPin = 34; 

Prox proxSensor; 

void Prox::setup(){
    pinMode(signalPin,INPUT); 
   
}

//in main loop run as if detection alert is true the print something or set of LED
/**will read same until change in the reading, not super accurate since the it can read ever 3 seconds but
    but has a rest period of 5-6 second after a reading is taken **/
bool Prox::detectionAlert(){
    int reading = digitalRead(signalPin); 
     
    if (reading == HIGH){
        Serial.println("alert!!"); 
        return true; 
    }
    else{
        return false; 
        Serial.println("end alert");
    }
}

