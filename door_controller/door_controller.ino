#include <ESP8266WiFi.h>
#define trig 4
#define echo 5
#define dist_thresh 10
 
const char* ssid = "marenostrum";
const char* password = "0123456789";

const char * host = "192.168.1.119"
const uint16_t port = 80;

bool in_house = false;

// ultrasound
long duration;
float dist, err, ef;



void setup() {
  Serial.begin(115200);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
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
      // Clear trig pin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  // trigger signal
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  // Reading the echo pin
  duration = pulseIn(echo, HIGH);
  dist = (float)duration/58;


  return dist <= dist_thresh; 
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
