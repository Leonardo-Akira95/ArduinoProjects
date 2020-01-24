//#include <SPI.H>
#include <MFRC522.h>

//Descomentar se for utilizar LCD
//#include <LiquidCrystal.h>

//Descomentar se for utilizar um Servo Motor para controle de fechadura
//#include <Servo.h>

#define RST_PIN 9
#define SS_PIN 10

byte readCard[4];
char* myTags[100] = {};
int tagsCount = 0;
String tagID = "";
boolean successRead = false;
boolean correctTag = false;
//int proximitySensor;
boolean doorStatus = false;

//Instancias
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  //SPI.begin();
  mfrc522.PCD_Init();

  while(!successRead){
    successRead = getID();
    if(successRead == true) {
      myTags[tagsCount] = strdup(tagID.c_str());
      Serial.print("Administrador");
      tagsCount++;
    }
  }
  successRead = false;
  
  printDefaultMessage();
  
}

void loop() {
  
    if (tagID == myTags[0]) {
      Serial.println("Entering administrator mode..");
      Serial.println("Add/Remover Tag");
      while (!successRead) {
        successRead = getID();
        if ( successRead == true) {
          for (int i = 0; i < 100; i++) {
            if (tagID == myTags[i]) {
              myTags[i] = "";
              Serial.println("Tag removida com sucesso");
              printDefaultMessage();
              return;
            }
          }
          myTags[tagsCount] = strdup(tagID.c_str());
          Serial.println("Tag adicionada com sucesso");
          printDefaultMessage();
          tagsCount++;
          return;
        }
      }
    }

    for(int i = 0; i < 100; i++){
      if(tagID == myTags[i]){
        Serial.println("Acesso garantido!");
        printDefaultMessage();
      }
    }
    if(correctTag == false){
       Serial.println("Acesso negado!");
       printDefaultMessage();
    }
    else{
      Serial.println("Porta aberta!");
      while(!doorStatus){
        doorStatus = true;
      }
      doorStatus = false;
      delay(500);
      printDefaultMessage();
    }
} 

uint8_t getID(){

  if (!mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
  
}

void printDefaultMessage(){
  delay(1500);
  Serial.println("Aproxime seu cartão de identificação!");
  
}
