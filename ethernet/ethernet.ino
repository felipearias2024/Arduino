#include <DHT.h>
#include <Ethernet.h>
#define DHTPIN 9
#define DHTTYPE DHT11
 
boolean reading = false;
float h = 0;
float t = 0;
 
// Mac address
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0E, 0x0C, 0x55};

IPAddress ip(192, 168, 30, 240);
EthernetServer server = EthernetServer(50080);
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  server.begin();
  Serial.println("Arduino connected");
  Serial.println(Ethernet.localIP());
  dht.begin();
}
 
void loop() {
  EthernetClient client = server.available();
  if(client) {
    boolean currentLineIsBlank = true;
    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        if(c == ' ') {
          reading = false;
        }
        if(c == '?') {
          reading = true;
        }
        if(reading) {
          if(c == '?') continue;
          switch(c) {
          case '1':
            readTempHum(client);
            break;
          }
        }
        if(c == '\n') {
          break;
        }
        if(c == '\n') {
          currentLineIsBlank = true;
        } 
        else if(c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
}
 
// Show temp &amp; humidity in the console 
void readTempHum(EthernetClient client) {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if(isnan(t) || isnan(h)) {
    Serial.println( "Problem!");
  } 
  else {
    // Print in the console
    Serial.print("Humidite :");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature :");
    Serial.print(t);
    Serial.println(" *C");
 
    sendJSON(client, t, h);
  }
}
 
// Send JSON data
void sendJSON(EthernetClient client, float t, float h) {
 
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: keep-alive");
  // to allow access from anywhere
  client.println("Access-Control-Allow-Origin: *");
  client.println();
 
  // output json valid request : {"temperature":"XX","humidity":"XX"}
  client.print("{");
  client.print("\"temperature\":");
  client.print("\"");
  client.print(t);
  client.print("\"");
  client.print(',');
  client.print("\"humidity\":");
  client.print("\"");
  client.print(h);
  client.print("\"");
  client.println("}");
}
