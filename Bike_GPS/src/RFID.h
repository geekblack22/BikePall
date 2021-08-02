#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN    21
#define RST_PIN   22

class RFID{
    
public:    
    void setup();
    void authorized();   
    bool hasAccess; 

};