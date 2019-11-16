#include <ESP8266WiFi.h>
 
const char* ssid = "marenostrum";
const char* password = "0123456789";

const char * host = "192.168.1.119"
const uint16_t port = 80;

bool in_house = false;

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

void sendInHouse(){
  WiFiClient cl;
  if (!client.connect(host, port)){
    Serial.println("Connection failed");
    return;
  }

  if (client.connected()){
    getPetition(&cl, "/api/in");
  }
}

void sendOutHouse(){
  WiFiClient cl;
  if (!client.connect(host, port)){
    Serial.println("Connection failed");
    return;
  }

  if (client.connected()){
    getPetition(&cl, "/api/out");
  }
}

void delay_s(int s){
  for(int i = 0; i<s; i++) delay(1000);
}

void loop() {
  if(doorOpen()){

    //user was in house -> exiting
    if (in_house){
      sendOutHouse();
    }else{
      sendInHouse();
    }

    in_house = !in_house;
    delay_s(10);
  }
}
