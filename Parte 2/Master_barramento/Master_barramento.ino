const int trans = 16;

void setup() {
Serial.begin(9600);
Serial.setTimeout(100);
pinMode(trans,OUTPUT);
digitalWrite(trans,HIGH);//Habilita a escrita do mestre no canal
}

void loop() {
if(Serial.available()){  
String acao = Serial.readString();  
//Biblioteca de comandos
//n = número do escravo
//SnW -> Slave Write, onde o escravo acende um LED
//SnR -> Slave Read, leitura de um sensor do escravo
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
  if (acao == "S1R"){
    digitalWrite(trans,LOW);
    Serial.print("1");
    delay(10);  
    Serial.println("R");
    Serial.flush();
    if (Serial.find("ACK")){
      delay(15);
      int temperatura = Serial.parseInt();
      Serial.println(temperatura);
    }
    digitalWrite(trans,HIGH);
  }
  if (acao == "S2R"){
    digitalWrite(trans,LOW);
    Serial.print("2");
    delay(10);  
    Serial.println("R");
    Serial.flush();
    if (Serial.find("ACK")){
      delay(15);
      int temperatura = Serial.parseInt();
      Serial.println(temperatura);
    }
    digitalWrite(trans,LOW);
  }
}
}
