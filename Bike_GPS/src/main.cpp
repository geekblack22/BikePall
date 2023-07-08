#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <LiquidCrystal.h>
#include "prox.h"
#include "hub.h"
#include "Timer.h"
#include "RFID.h"
#include "Voice.h"
#include "Audio.h"
#include "Audio_speech.h"
// Digital I/O used
#define I2S_DOUT      25  // DIN connection
#define I2S_BCLK      27  // Bit clock
#define I2S_LRC       26  // Left Right Clock
 
Audio audio;
Timer AudioTimer(18000);
Timer dataTimer(200000);
float lat ,lon;
Voice commands;
TinyGPSPlus gps;
String  lat_str = "48.85756" , lng_str = "2.34280";
#define RXD2 18
#define TXD2 17
float latitude = 75.0;
float longitude = 35.0;
HardwareSerial SerialGPS(1);
LiquidCrystal lcd(0, 4, 5, 22, 19, 21);
RFID allowed;
Prox prox_sensor;
Hub hub;
bool flag = false; 
////Talk bikey;
char data[100];
char auth[] = " #";
int count = 0;
int ledLeftPin = 10; //change values when connected to board
int ledRightPin = 12; //"
enum PLAY {FIRST,SECOND,LAST}play;
boolean done = false;
void setup() {
  Serial.begin(115200);

 
prox_sensor.setup();
hub.setup();
//bikey.setup();
delay(500);
  Serial.println("\r\nRecord start!\r\n");
  Audio_speech* audio = new Audio_speech();

  audio_speech->Record();
  CloudSpeechClient* cloudSpeechClient = new CloudSpeechClient(USE_ACCESSTOKEN);
  cloudSpeechClient->Transcribe(audio_speech);
  delete cloudSpeechClient;
  delete audio_speech;
 audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(21);
  audio.setFileLoop(false);
  play = FIRST;
// connect serial 
Serial.println("The GPS Received Signal:"); 
SerialGPS.begin(9600, SERIAL_8N1, RXD2, TXD2);
lcd.begin(16, 2);
 
 //Blynk.begin(auth, hub.ssid, hub.password);
 //Blynk.virtualWrite(V0, "clr"); 
 pinMode(ledLeftPin,OUTPUT); 
 pinMode(ledRightPin, OUTPUT); 
}

//LED instructions: set a delay propotional to how far away the next turn
//function to compare points (only needs to be run after each turn, can be triggered by gyroscope and accelerometer)
    //how to determine if lat or longitude comparison is needed?

//need to compare current gps to next, if diffrence is increasing then the red led to indicate going the wrong way. 
  //only needs to be done after each turn.(use gyro and accelerometer to confirm turn as well as gps changes) 

//function for the blinking delay 
  //use diffrence in longitudes or latitudes for current gps and next point to set the blinking rate. 

//use left and right functions to control which led or both is on. use both if going straight 
void ledLeft(int delayValue){
  digitalWrite(ledLeftPin, HIGH); 
  delay(delayValue); 
  digitalWrite(ledLeftPin, LOW); 
  delay(delayValue); 
}

void ledRight(int delayValue){
  digitalWrite(ledRightPin, HIGH); 
  delay(delayValue); 
  digitalWrite(ledRightPin, LOW); 
  delay(delayValue); 
}

void playSound(String current_direction){
  switch(play){
    case FIRST:
        audio.connecttospeech(strcpy(new char[current_direction.substring(0,current_direction.indexOf("on")).length() + 1],current_direction.substring(0,current_direction.indexOf("on")).c_str()), "en");
        while(!(audio.getAudioFileDuration() == audio.getAudioCurrentTime() && audio.getAudioFileDuration() != 0)){
          audio.loop();
        }
        done = false;
        play = SECOND;
      break;
    case SECOND:
      audio.connecttospeech(strcpy(new char[current_direction.substring(current_direction.indexOf("on") ,current_direction.indexOf("towards")).length() + 1], current_direction.substring(current_direction.indexOf("on"),current_direction.indexOf("towards")).c_str()), "en");
      while(!(audio.getAudioFileDuration() == audio.getAudioCurrentTime() && audio.getAudioFileDuration() != 0)){
          audio.loop();
        }
        play = LAST;
      break;
    case LAST:
      if(!done){
       audio.connecttospeech(strcpy(new char[current_direction.substring(current_direction.indexOf("towards"),current_direction.length()).length() + 1], current_direction.substring(current_direction.indexOf("towards"),current_direction.length()).c_str()), "en");
       while(!(audio.getAudioFileDuration() == audio.getAudioCurrentTime() && audio.getAudioFileDuration() != 0)){
          audio.loop();
        }
      }
        done = true;
      break;
  }
}

void loop() {
  
 
  static String current_direction = "";
  
  //Serial.print("Voice: ");
  //Serial.println(commands.voiceInput());
String start[2] = {"-71.09144" , "42.27461"};
String end[2] = {"-71.09227" ,"42.2676"}; 
if(count == 58){count = 0;}

String current_location_coordinates[2] = {String(test[count][0],6),String(test[count][1],6)}; 

String current_location = hub.getAdress(current_location_coordinates[0],current_location_coordinates[1]);
String direction = hub.getDirections(start,end, current_location);

 direction = hub.getDirections(start, end , current_location);


if(!direction.equals(current_direction)){
  play = FIRST;
}
playSound(direction);
Serial.println(direction);
// delay(5000);
//bikey.textToSpeech("hello");



 




 prox_sensor.detectionAlert();
  
 while (SerialGPS.available() > 0) {
   prox_sensor.detectionAlert();
    
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
      

      }
       lcd.setCursor(16,1);
      lcd.autoscroll();    // Set diplay to scroll automatically
      lcd.print(" ");      // set characters
      delay(700);
      //lcd.clear();
    }  
   
 }
  //Blynk.run();
 
   count += 1;
 current_direction = direction;
     
        
}
