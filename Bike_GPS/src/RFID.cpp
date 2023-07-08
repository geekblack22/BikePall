#include "RFID.h"

MFRC522 mfrc522(SS_PIN, RST_PIN); 
void RFID::setup() {
  // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
}

void RFID:: authorized(){
    // Look for new cards
 if ( !mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( !mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String content= "";

  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if ((content.substring(1) == "CA D1 3B 16")||(content.substring(1) == "#")) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    hasAccess = true;
    Serial.println();
   
    
  }
 
 else  {
     Serial.println("Authorized denied");
    hasAccess = false;
    
    
    
    
  }
}
