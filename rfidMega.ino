#include <SPI.h>
#include <MFRC522.h>
#include "HX711.h"
#include <SoftwareSerial.h>
#define DEBUG_HX711
SoftwareSerial espSerial(4,5);
String sendDataEsp;
#define CALIBRACION 25100.0
// Pin de datos y de reloj
byte pinData = 3;
byte pinClk = 2;
// Objeto HX711
HX711 bascula;
#define RST_PIN 9
#define SS_PIN 53

MFRC522 reader(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

String peso   = "";
String serial = "";

void setup() {
  Serial.begin(9600); //Iniciamos la comunicación  serial
  Serial.println("Lectura del RFID");
  SPI.begin();
  reader.PCD_Init();
  delay(4);
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
#ifdef DEBUG_HX711
  espSerial.begin(9600);
  Serial.println("[HX7] Inicio del sensor HX711");
#endif
  // Iniciar sensor
  bascula.begin(pinData, pinClk);
  // Aplicar la calibración
  bascula.set_scale(CALIBRACION);
  // Iniciar la tara
  // No tiene que haber nada sobre el peso
  bascula.tare();
  Serial.println("Listo!");
}

void loop() {

  rfid();
  dataBascula();
  Serial.println("RFID : "+serial+","+"Peso : "+peso+",");
  espSerial.println(serial+","+peso+",");
  peso = "";
  serial = "";
  delay(1000);
  
 
}

void rfid(){
  
  if (!reader.PICC_IsNewCardPresent())
  {
    return;
  }
  if (!reader.PICC_ReadCardSerial())
  {
    return;
  }
  
  for (int x = 0; x < reader.uid.size; x++)
  {
    if (reader.uid.uidByte[x] < 0x10)
    {
      serial += "0";
    }
    serial += String(reader.uid.uidByte[x], HEX);
    if (x + 1 != reader.uid.size)
    {
      serial += "-";
    }
  }
  serial.toUpperCase();
  //Serial.println("Read serial is: " + serial);
  reader.PICC_HaltA();
  reader.PCD_StopCrypto1();

}

void dataBascula(){
  
#ifdef DEBUG_HX711

if(bascula.get_units() > 0.5 ){
  peso = String(bascula.get_units(), 1);
  }
  else{
    peso = "";
    }
#endif
  }
