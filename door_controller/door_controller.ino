#include <ESP8266WiFi.h>
 
const char* ssid = "marenostrum";
const char* password = "0123456789";

const char * host = "192.168.1.11"
const uint16_t port = 80;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connnected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


bool getPetition(WiFiClient &cl,  String path){
  cl->print("GET " + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n" + "\r\n");
}

bool doorOpen(){
  return true; 
}

void sendDoorOppenned(){
  WiFiClient cl;
  if (!client.connect(host, port)){
    Serial.println("Connection failed");
    return;
  }

  if (client.connected()){
    getPetition(&cl, "/api/door_open");
  }
}
void loop() {
  if(doorOpen()){
    sendDoorOpenned(();
    delay(100000);
  }
}
