//#include <ESP8266WiFi.h>
//Foram inseridos dois LED's, para testar os comandos SA e SR

void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT); //LED do SA
  pinMode(15,OUTPUT); //LED do SR
}

void loop() {
  String ordem;
  if (Serial.available()){
    ordem = Serial.read();
    if (ordem == "A"){
      digitalWrite(13,HIGH);
      delay(2000);
      digitalWrite(13,LOW);
    }
    if (ordem == "R"){
      digitalWrite(15,HIGH);
      delay(2000);
      digitalWrite(15,LOW);
    }
  }

}
