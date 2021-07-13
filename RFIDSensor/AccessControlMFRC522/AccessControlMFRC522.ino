/*
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

/*
#define ALLOWED 2   // no of allowed cards
byte read_card[4];

byte cards[ALLOWED][4] = {{33 58 FC 1B}},
                         {0x9A, 0x48, 0x3A, 0xD5}};
*/      

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           
#define SS_PIN          10          

MFRC522 mfrc522(SS_PIN, RST_PIN);   
bool validCard = false;

void setup() {
  Serial.begin(9600);                                           
  SPI.begin();                                                
  mfrc522.PCD_Init();                                              
  Serial.println(F("Leitura de dados pessoais de cartoes MIFARE PICC"));   
}


void loop() {

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  
  byte block;
  byte len;
  
  MFRC522::StatusCode status;

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  
  Serial.println(F("**Cartao detectado**"));

  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println("Analisando compatibilidade do cartao..");
  delay(300);
  
  // Checar compatibilidade
  if(     piccType != MFRC522::PICC_TYPE_MIFARE_MINI
      &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
      &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K){
      Serial.println(F("Your PICC card is not compatible with our system.."));
      validCard = true;
      return;
  }
  
  Serial.println("**Cartao compativel**");
  Serial.println();
  
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();
  if (content.substring(1) == "33 58 FC 1B")
  {
    Serial.println();
    Serial.println("Acesso autorizado, abrindo porta");
    Serial.println();
    delay(3000);
  }
 
 else   {
    Serial.println(" Acesso negado..");
    delay(3000);
  }
  
}
