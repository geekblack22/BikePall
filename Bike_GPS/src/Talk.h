#include <Arduino.h>
#include "Audio.h"
#include <FS.h>
#include <SPIFFS.h>
#include <SD_MMC.h>
#include <FFat.h>
// Media pins


#define SD_CS         5
#define SPI_MOSI      13
#define SPI_MISO      32
#define SPI_SCK       14
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26
class Talk{

    public:
       void textToSpeech(String line);
       void speack();
       void setup();
    private:
        String speech;
    

       

};
