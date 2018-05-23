
void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT); //LED do SA
  pinMode(15,OUTPUT); //LED do SR
  digitalWrite(13,LOW);
  digitalWrite(15,LOW);
}

void loop() {
  String ordem;
  if (Serial.available()){
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
      digitalWrite(15,HIGH);
      delay(2000);
      digitalWrite(15,LOW);
    }
  }

}
