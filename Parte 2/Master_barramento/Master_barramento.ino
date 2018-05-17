void setup() {
Serial.begin(9600);
pinMode(7,OUTPUT); //string que receberá a ação a ser feita pelo usuário
}

void loop() {
String acao = Serial.readString();  
if(Serial.available()){
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
