#include <ESP8266WiFi.h>

const char* ssid = "COMDADOS_RS485";
const char* password = "12345678";
WiFiServer server(80);

int byteReceived;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
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
  Serial.flush();
  client.flush();
  // Match the request
  if (req.indexOf("1") != -1){
    client.print("Ligando atuador");
  }
  if (req.indexOf("2") != -1){
    client.print("Lendo sensor");
  }
  client.flush();

}
