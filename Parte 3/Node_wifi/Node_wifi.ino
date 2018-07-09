#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid = "COMDADOS_RS485";
const char* password = "12345678";
WiFiServer server(80);

#define SSerialRX        13  //Serial Receive pin
#define SSerialTX        15  //Serial Transmit pin
#define pinLED         7

SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

int byteReceived;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(SSerialTxControl,OUTPUT);
  pinMode(pinLED,OUTPUT);
  
  IPAddress staticIP(192, 168, 4, 2); // IP set to Static
  IPAddress gateway(192, 168, 4, 1);// gateway set to Static
  IPAddress subnet(255, 255, 255, 0);// subnet set to Static

  WiFi.mode(WIFI_AP);// Working mode only as Acess Point 

  WiFi.softAP(ssid, password, 2, 0);
  WiFi.config(staticIP, gateway, subnet);

  server.begin();

  Serial.println("Server started"); 
  Serial.println(WiFi.softAPIP());

  Serial.begin(9600);
  RS485Serial.begin(4800);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  req = req.substring(req.indexOf("/") + 1, req.indexOf("HTTP") - 1);
  Serial.println(req);
  client.flush();

  // Match the request

  if (req.indexOf("Liga") != -1)
  {
    client.print("Ligando LED na porta 13  ");
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    delay(1000);
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    delay(1000);
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    delay(1000);
  }
  else {
    client.print("Invalid Request");
    client.flush();
    client.stop();
    return;
  }

  client.print("HTTP/1.1 200 OK\n\n");
  client.flush();

}
