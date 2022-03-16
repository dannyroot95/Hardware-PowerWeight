#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>                    
#define WIFI_SSID "BENILDA"
#define WIFI_PASSWORD "08101953"
#include <Separador.h>
#define RST_PIN 0
#define SS_PIN 2
#define FIREBASE_HOST "rfid-e67e1-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "g0kYWcSE0qGkMNwqddtCJt3xYArD732u4ApedLva"
Separador s;

const int ledPIN_GREEN = 16;
const int ledPIN_RED = 5;

String peso = "";
String rfid = "";
Servo servo1;    
int pin = 15;    
int angulo = 0; 
bool flag = false;

void setup()
{
  Serial.begin(9600); 
  servo1.attach(pin);
  pinMode(ledPIN_GREEN , OUTPUT);
  pinMode(ledPIN_RED , OUTPUT);
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
  Firebase.setString("rfid/code", "");



  Serial.println("Ready!");
  
}

void loop()
{

   if(Serial.available()){
    String datosrecibidos = Serial.readString();
    rfid = s.separa(datosrecibidos, ',',0);
    peso = s.separa(datosrecibidos, ',',1);
    Serial.println("Recibido : "+rfid+","+peso);
   }
        
   if(rfid != ""){
  
      String statusStudent = Firebase.getString("users/students/"+rfid+"/status");
      String valuePerson = Firebase.getString("person/personValue");
      
      if(valuePerson != "stop"){
        
         if(statusStudent == "Pagado"){
               flag = false;
               openDoor();
               Serial.print("Pagado");
               digitalWrite(ledPIN_GREEN , HIGH);   // poner el Pin en HIGH
               Firebase.setString("rfid/code", rfid);
               Firebase.setString("person/personValue", "stop");
               delay(3000);                   // esperar un segundo
               digitalWrite(ledPIN_GREEN , LOW);    // poner el Pin en LOW
    }
    else if (statusStudent == "Deudor"){
      Serial.print("Deudor");
      digitalWrite(ledPIN_RED , HIGH);   // poner el Pin en HIGH
      delay(3000);                   // esperar un segundo
      digitalWrite(ledPIN_RED , LOW);
      }
    else{
      Serial.print("No existe");
      }
  
  if (Firebase.failed()) {
      Serial.print("fallo de conexion:");
      Serial.println(Firebase.error());
      digitalWrite(ledPIN_RED , HIGH);   // poner el Pin en HIGH
      delay(3000);                   // esperar un segundo
      digitalWrite(ledPIN_RED , LOW);  
      return;
     }
    }
 else{
      Serial.print("Stop");
      digitalWrite(ledPIN_RED , HIGH);   // poner el Pin en HIGH
      delay(3000);                   // esperar un segundo
      digitalWrite(ledPIN_RED , LOW);  
  }
 }

  if(peso != ""){
      Firebase.setString("weight/valueWeight", peso);
      }
}


void openDoor(){
   // Giro de 0º a 180º
 if(flag == false){
  for(angulo = 85; angulo <=180; angulo=angulo+10)
    {
      servo1.write(angulo);
      delay(50);
  }
  // Giro de 180º a 0º
  delay(3000);
  for(angulo = 180; angulo >=85; angulo=angulo-10)
    {
      servo1.write(angulo);
      delay(50);
  }

  flag = true;
  
  }  
}  
