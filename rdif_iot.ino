#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define WIFI_SSID "BENILDA"
#define WIFI_PASSWORD "08101953"
#define RST_PIN 0
#define SS_PIN 2
#define FIREBASE_HOST "rfid-e67e1-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "g0kYWcSE0qGkMNwqddtCJt3xYArD732u4ApedLva"

MFRC522 reader(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup()
{
  Serial.begin(9600); 
  while (!Serial)
  {
  }
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("codeCard/code", "");

  SPI.begin();
  reader.PCD_Init();
  delay(4);
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  Serial.println("Ready!");
}

void loop()
{
  
   if (!reader.PICC_IsNewCardPresent())
  {
    return;
  }
  if (!reader.PICC_ReadCardSerial())
  {
    return;
  }
  String serial = "";
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
  Serial.println("Read serial is: " + serial);
  reader.PICC_HaltA();
  reader.PCD_StopCrypto1();
  
  Firebase.setString("codeCard/code", serial);
  
  if (Firebase.failed()) {
      Serial.print("fallo de conexion:");
      Serial.println(Firebase.error());  
      return;
     }

     }


     
