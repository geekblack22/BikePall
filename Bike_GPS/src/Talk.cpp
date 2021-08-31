#include "Talk.h"



//Audio audio;
void Talk::setup(){
  //pinMode(SD_CS, OUTPUT);      
 // digitalWrite(SD_CS, HIGH);
  //SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  //SD.begin(SD_CS);
  // audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  // audio.setVolume(21);
  
}
void Talk::textToSpeech(String line){
    
  // String speechURL = tts.getSpeechUrl(line);
  char *current_speech;
  // if(!line.equals(speech)){
  //   current_speech = strcpy(new char[line.length() + 1], line.c_str());
  // }
  //  speech = line;
  // audio.connecttospeech(current_speech, "en");
  // URL = "http://mp3.ffh.de/radioffh/hqlivestream.mp3";
 //audio.connecttohost(URL);
  //audio.loop();
}
