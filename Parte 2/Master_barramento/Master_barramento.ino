void setup() {
Serial.begin(9600);
Serial.setTimeout(100);
}

void loop() {
if(Serial.available()){  
String acao = Serial.readString();  
//Biblioteca de comandos
//SA -> Slave Act, onde o escravo acende um LED
//SR -> Slave Read, leitura de uma porta do escravo
  if (acao == "SA"){
    Serial.print("A");
    delay(10);  
  }
  if (acao == "SR"){
    Serial.print("R");
    delay(10);
  }
}


}
