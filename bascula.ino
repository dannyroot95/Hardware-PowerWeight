#include "HX711.h"
#include <SoftwareSerial.h>
#define DEBUG_HX711
SoftwareSerial espSerial(4,5);
String str;
// Parámetro para calibrar el peso y el sensor
#define CALIBRACION 25100.0
 
// Pin de datos y de reloj
byte pinData = 3;
byte pinClk = 2;
 
// Objeto HX711
HX711 bascula;
 
void setup() {
 
#ifdef DEBUG_HX711
  // Iniciar comunicación serie
  Serial.begin(9600);
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
}
 
void loop() {

#ifdef DEBUG_HX711

if(bascula.get_units() > 0.5 ){
  String peso = String(bascula.get_units(), 1);
  //espSerial.println(peso+String(","));
  Serial.print(bascula.get_units(), 1);
  Serial.print(" Kg");
  Serial.println();
  }
  else{
    espSerial.println("0.0"+String(","));
    }
#endif

delay(1000);

}
