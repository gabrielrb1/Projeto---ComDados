#include <SoftwareSerial.h>
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define pinLED         7
#define sensor A0

SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

int byteReceived;
int byteSend;

void setup()
{

  Serial.begin(9600);  
  pinMode(pinLED, OUTPUT);   
  pinMode(SSerialTxControl, OUTPUT);    
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver
  RS485Serial.begin(4800);   // set the data rate 
}


void loop()
{
  //Copy input data to output  
  if (RS485Serial.available()) 
  {
    byteSend = RS485Serial.read();   // Read the byte 
    //digitalWrite(Pin13LED, HIGH);  // Show activity
    delay(10);              
    //digitalWrite(Pin13LED, LOW);   
    
    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit  
    if (byteSend == 49){ //Valor "1" do command window
      RS485Serial.write("ACK"); 
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
      int temp = analogRead(sensor);
      RS485Serial.write("Lendo sensor");
      RS485Serial.write(temp);
    }
    RS485Serial.write(byteSend); // Send the byte back
    delay(10);   
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit      
  }
  
}
