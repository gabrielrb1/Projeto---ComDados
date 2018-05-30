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
  if (acao == "S1W"){
    Serial.print("1");
    delay(10);  
    Serial.println("W");
    delay(10);
  }
  if (acao == "S2W"){
    Serial.print("2");
    delay(10);  
    Serial.println("W");
    delay(10);
  }
  //if (acao == "S1R"){
    //Serial.print("1");
    //delay(10);  
    //Serial.println("R");
    //delay(10);
  //}
  if (acao == "S2R"){
    Serial.print("2");
    delay(10);  
    Serial.println("R");
    Serial.flush();
    if (Serial.find("A")){
      delay(15);
      int temperatura = Serial.parseInt();
      Serial.println(temperatura);
    }
  }
}
}
