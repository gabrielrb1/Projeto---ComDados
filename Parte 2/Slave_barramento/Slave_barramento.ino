#include <SoftwareSerial.h>
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define pinLED 7
#define sensor 0

SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

byte byteReceived;
byte byteSend;
int temp;
int temp1;
int temp2;
int temp3;
int temp4;

void setup()
{

  Serial.begin(9600);  
  pinMode(pinLED, OUTPUT);   
  pinMode(SSerialTxControl, OUTPUT);    
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver
  RS485Serial.begin(9600);   // set the data rate
}


void loop()
{
  //Copy input data to output  
  if (RS485Serial.available()) 
  {
    byteSend = RS485Serial.read();   // Read the byte 
    delay(10);  
    
    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit  
    if (byteSend == 49){ //Valor "1" do command window
      RS485Serial.write(13);
      RS485Serial.write(10);
      RS485Serial.write("ACK");
      RS485Serial.write(13);
      RS485Serial.write(10); 
      delay(10);
      if (digitalRead(pinLED)){
        digitalWrite(pinLED,LOW);
        RS485Serial.write("Desligando atuador");
      }
      else {
        digitalWrite(pinLED,HIGH);
        RS485Serial.write("Ligando Atuador");
      }      
    }
    if (byteSend == 50){//Valor "2" do command window
      temp = analogRead(sensor);
      
      temp1 = temp/1000;         //recurso técnico provisório de carater permanente
      temp = temp - (temp1*1000);
      temp2 = temp/100;
      temp = temp - (temp2*100);
      temp3 = temp/10;
      temp = temp - (temp3*10);
      temp4 = temp;
      
      RS485Serial.write(13);
      RS485Serial.write(10);
      RS485Serial.write("ACK");
      RS485Serial.write(13);
      RS485Serial.write(10);
      RS485Serial.write("Lendo sensor  ");
      RS485Serial.write(temp1+'0');
      RS485Serial.write(temp2+'0');
      RS485Serial.write(temp3+'0');
      RS485Serial.write(temp4+'0');
    }
    //RS485Serial.write(13);
    //RS485Serial.write(10);
    //RS485Serial.write(byteSend); // Send the byte back
    delay(10);   
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit      
  }
  
}
