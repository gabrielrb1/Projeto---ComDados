void setup() {
Serial.begin(9600);
Serial.setTimeout(100);
}

void loop() {
if(Serial.available()){  
String acao = Serial.readString();  
//Biblioteca de comandos
//SW -> Slave Write, onde o escravo acende um LED
//SR -> Slave Read, leitura de uma porta do escravo
  if (acao == "SW"){
    Serial.println("W");
    delay(10);  
  }
  else if (acao == "SR"){
    Serial.println("R");
    delay(10);
  }
  else
  {
    Serial.println("Comando desconhecido");
  }
}


}
