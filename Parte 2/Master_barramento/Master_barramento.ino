//MESTRE
//FONTE: https://arduino-info.wikispaces.com/SoftwareSerialRS485Example
/-----( Import needed libraries )-----/
#include <SoftwareSerial.h>
/-----( Declare Constants and Pin Numbers )-----/
#define SSerialRX        10  //Serial Receive pin 10
#define SSerialTX        11  //Serial Transmit pin 11

#define SSerialTxControl 3   //RS485 Direction control 3

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define pinLED         13

/-----( Declare objects )-----/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

/-----( Declare Variables )-----/
byte byteReceived;
byte byteSend;

void setup()   /** SETUP: RUNS ONCE **/
{
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Comunicacao RS485 iniciada");
  
  pinMode(pinLED, OUTPUT);   
  pinMode(SSerialTxControl, OUTPUT);    
  
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver   
  
  // Start the software serial port, to another device
  RS485Serial.begin(9600);   // set the data rate 

}//--(end setup )---


void loop()   /** LOOP: RUNS CONSTANTLY **/
{
  if (Serial.available())
  {
    byteReceived = Serial.read();
    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit   
    RS485Serial.write(byteReceived);          // Send byte to Remote Arduino
    delay(10);
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit       
  }

   if (Serial1.available())
  {
    byteReceived = Serial1.read();
    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit   
    RS485Serial.write(byteReceived);          // Send byte to Remote Arduino
    delay(10);
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit       
  }
  
  if (RS485Serial.available())  //Look for data from other Arduino
   {
    byteReceived = RS485Serial.read();    // Read received byte
    Serial.write(byteReceived);        // Show on Serial Monitor
    Serial1.write(byteReceived);
    delay(10);  
   }  

}
