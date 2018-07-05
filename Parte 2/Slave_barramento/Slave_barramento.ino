const int led = 4;
const int trans = 8;

void setup() {
  Serial.begin(9600);
  pinMode(led,OUTPUT);//pino de acionamento do LED
  pinMode(A0,INPUT);//leitura do sensor
  pinMode(trans,INPUT);//pino ligado aos terminais DE e RE do transceiver
  digitalWrite(led,LOW);
  digitalWrite(trans,LOW);//escravo começa 
  //DE e RE = 0 -> transceiver recebe dados
  //DE e RE = 1 -> transceiver envia dados
}

void loop() {
  if (Serial.available()){
    if (Serial.read() == '2'){//Definição do ID do escravo
      delay(15);
      char ordem = Serial.read();
      if (ordem == 'W'){
        if (digitalRead(led)==LOW){
        digitalWrite(led,HIGH);
        //Serial.print("LED ligado");
        }
        else{
        digitalWrite(led,LOW);
        //Serial.print("LED foi desilgado");
        }
    }
    if (ordem == 'R'){
      int temp = analogRead(A0);
      digitalWrite(trans,HIGH);
      Serial.print("ACK"); //confirmação de recebimento do comando de leitura
      delay(10);
      Serial.print(temp);
      Serial.flush();
      digitalWrite(trans,LOW);
    }
  }
  }
}
