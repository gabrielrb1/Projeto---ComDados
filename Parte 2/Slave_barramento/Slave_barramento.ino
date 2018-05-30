void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(A0,INPUT);
  digitalWrite(13,LOW);
}

void loop() {
  if (Serial.available()){
    if (Serial.read() == '2'){//Definição do ID do escravo
      delay(15);
      char ordem = Serial.read();
      if (ordem == 'W'){
        if (digitalRead(13)==LOW){
        digitalWrite(13,HIGH);
        //Serial.print("LED ligado");
        }
        else{
        digitalWrite(13,LOW);
        //Serial.print("LED foi desilgado");
        }
    }
    if (ordem == 'R'){
      int temp = analogRead(A0);
      Serial.print("A"); //confirmação de recebimento do comando de leitura
      delay(10);
      Serial.flush();
    }
  }
  }
}

