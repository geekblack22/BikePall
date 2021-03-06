#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <LiquidCrystal.h>
#include "prox.h"
#include "hub.h"
#include "Timer.h"
#include "RFID.h"
#include "Voice.h"
#include "Talk.h"
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
char auth[] = " d8JVu9muQYonDxIKe1tPNPlUa_BDrhz5";
int count = 0;
int ledLeftPin = 10; //change values when connected to board
int ledRightPin = 12; //"
enum PLAY {FIRST,SECOND,LAST}play;
boolean done = false;
float test[59][2] = {{-71.091224,42.274514},{-71.091315,42.274402},{-71.091446,42.27431},{-71.091681,42.274181},{-71.092036,42.274056},{-71.092012,42.273907},{-71.091984,42.273615},{-71.091981,42.273577},{-71.091952,42.273165},{-71.09193,42.272702},{-71.091925,42.272624},{-71.092043,42.272157},{-71.092254,42.271586},{-71.09241,42.271213},{-71.092576,42.270806},{-71.092701,42.270622},{-71.092834,42.270518},{-71.092907,42.270465},{-71.093059,42.270353},{-71.093253,42.270211},{-71.093286,42.270196},{-71.093422,42.270169},{-71.093601,42.270167},{-71.093651,42.270161},{-71.09373,42.27014},{-71.093767,42.270125},{-71.093761,42.270069},{-71.093759,42.270009},{-71.093768,42.26976},{-71.093814,42.26869},{-71.093849,42.268112},{-71.093871,42.267741},{-71.093878,42.267625},{-71.093896,42.267407},{-71.093908,42.267234},{-71.093924,42.26694},{-71.093932,42.266802},{-71.09398,42.266494},{-71.09407,42.266282},{-71.094132,42.266174},{-71.094145,42.266115},{-71.094144,42.266047},{-71.09411,42.265991},{-71.094049,42.265959},{-71.093971,42.265949},{-71.09386,42.265988},{-71.093783,42.266114},{-71.093771,42.266159},{-71.093752,42.266599},{-71.093749,42.266705},{-71.093742,42.266936},{-71.093726,42.267227},{-71.093705,42.267296},{-71.093576,42.267517},{-71.093522,42.267582},{-71.093502,42.267567},{-71.093407,42.267535},{-71.093328,42.267538},{-71.092315,42.267731}};
void setup() {
  Serial.begin(115200);
  /*allowed.setup();
  while(!allowed.hasAccess){
    allowed.authorized();
  }
   Serial.println("Access Granted");
*/
 
prox_sensor.setup();
hub.setup();
//bikey.setup();
delay(500);
  Serial.println("\r\nRecord start!\r\n");
  Audio_speech* audio = new Audio_speech();
//  Audio* audio = new Audio(M5STACKFIRE);
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

// if(count == 0){
//   current_direction = direction;
//     audio.connecttospeech(strcpy(new char[direction.substring(0,current_direction.indexOf("on")).length() + 1],current_direction.substring(0,direction.indexOf("on")).c_str()), "en");
    
//   }
// if(count == 0){
//    current_direction = direction;
//    audio.connecttospeech(strcpy(new char[current_direction.substring(current_direction.indexOf("on") + 2,direction.length() -1).length() + 1], current_direction.substring(current_direction.indexOf("on") + 2,current_direction.length() -1).c_str()), "en");
//    }
//  Serial.println(current_direction.substring(current_direction.indexOf("on") + 2,direction.length() -1));

//  audio.loop();
if(!direction.equals(current_direction)){
  play = FIRST;
}
playSound(direction);
Serial.println(direction);
// delay(5000);
//bikey.textToSpeech("hello");



 




 prox_sensor.detectionAlert();
  //sprintf(data, "{\"lat\": \"%s\", \"lon\": \"%s\"}", lat_str, lng_str); 
        //hub.loop(data);
        //Serial.println(hub.getAdress(lat_str,lng_str));
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
        //sprintf(data, "{\"lat\": \"%s\", \"lon\": \"%s\"}", lat_str, lng_str); 
        //Blynk.virtualWrite(V0, 1, latitude, longitude, "Location");

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
// void audio_info(const char *info){
//     Serial.print("info        "); Serial.println(info);
// }
// void audio_id3data(const char *info){  //id3 metadata
//     Serial.print("id3data     ");Serial.println(info);
// }
// void audio_eof_mp3(const char *info){  //end of file
//     Serial.print("eof_mp3     ");Serial.println(info);
// }
// void audio_showstation(const char *info){
//     Serial.print("station     ");Serial.println(info);
// }
// void audio_showstreaminfo(const char *info){
//     Serial.print("streaminfo  ");Serial.println(info);
// }
// void audio_showstreamtitle(const char *info){
//     Serial.print("streamtitle ");Serial.println(info);
// }
// void audio_bitrate(const char *info){
//     Serial.print("bitrate     ");Serial.println(info);
// }
// void audio_commercial(const char *info){  //duration in sec
//     Serial.print("commercial  ");Serial.println(info);
// }
// void audio_icyurl(const char *info){  //homepage
//     Serial.print("icyurl      ");Serial.println(info);
// }
// void audio_lasthost(const char *info){  //stream URL played
//     Serial.print("lasthost    ");Serial.println(info);
// }
// void audio_eof_speech(const char *info){
//     Serial.print("eof_speech  ");Serial.println(info);
// }