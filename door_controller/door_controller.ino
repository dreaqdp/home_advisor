#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
/*
 * RC522 Interfacing with NodeMCU
 * 
 * Typical pin layout used:
 * ----------------------------------
 *             MFRC522      Node     
 *             Reader/PCD   MCU      
 * Signal      Pin          Pin      
 * ----------------------------------
 * RST/Reset   RST          D1 (GPIO5)        
 * SPI SS      SDA(SS)      D2 (GPIO4)       
 * SPI MOSI    MOSI         D7 (GPIO13)
 * SPI MISO    MISO         D6 (GPIO12)
 * SPI SCK     SCK          D5 (GPIO14)
 * 3.3V        3.3V         3.3V
 * GND         GND          GND
 */

//ultrsound parameters
#define trig 15
#define echo 0
#define dist_thresh 10
#define key_time_thres 10000 // 5 seg
#define SS_PIN 4
#define RST_PIN 5
#define ultrasound 16

//server config
const char* ssid = "marenostrum";
const char* password = "0123456789";
//const char * host = "192.168.1.119";
const char * host = "192.168.1.13";
const uint16_t port = 3000;

//initial state
bool in_house = false;

// ultrasound
long duration;
float dist, err, ef;

// RF
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

void setup() {
    //Setup ultrasound pins
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);

    //Setup serial for log
    Serial.begin(115200);
    Serial.println();
    Serial.println();

    Serial.print("Connecting to ");
    Serial.println(ssid);

    //connect to network
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    //wait for wifi connection
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    //Log the newtwork info
    Serial.println("");
    Serial.println("WiFi connnected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // RF
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522

    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}


bool getPetition(WiFiClient &cl,  String path){
    cl.print("GET " + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n" + "\r\n");
}

bool put_petition (WiFiClient &cl,  String path, String key) {
    cl.print("PUT " + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "uid: " + key + "\r\n" + "Connection: close\r\n" + "\r\n");
}
/*
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
Serial.print(dist);

if ( dist <= dist_thresh ){
Serial.println("Door is openned");
}else{
Serial.println("Door is closed");
}
return dist <= dist_thresh; 
}
 */

bool doorOpen () {
    return digitalRead(ultrasound);
}

void sendInHouse(){
    Serial.println("User entering home...");
    WiFiClient cl;
    if (!cl.connect(host, port)){
        Serial.println("Connection failed");
        return;
    }

    if (cl.connected()){
        getPetition(cl, "/api/in");
    }
}

void sendOutHouse(){
    Serial.println("User exiting home...");
    WiFiClient cl;
    if (!cl.connect(host, port)){
        Serial.println("Connection failed");
        return;
    }

    if (cl.connected()){
        getPetition(cl, "/api/out");
    }
}

bool send_key (String key){
    WiFiClient cl;
    if (!cl.connect(host, port)) {
        Serial.println("Connection failed");
    }

    if (cl.connected()) {
        put_petition(cl, "/api/user_enter", key);
        //        getPetition(cl, "/api/user_enter");
        Serial.println("Connection sended ");
        Serial.print(key);
        return true;
    }
    return false;
}

void send_leave_keys(){
    Serial.println("User forgot keys...");
    WiFiClient cl;
    if (!cl.connect(host, port)){
        Serial.println("Connection failed");
        return;
    }

    if (cl.connected()){
        getPetition(cl, "/api/advise_leave_keys");
    }
}

void send_take_keys(){
    Serial.println("User remember keys...");
    WiFiClient cl;
    if (!cl.connect(host, port)){
        Serial.println("Connection failed");
        return;
    }

    if (cl.connected()){
        getPetition(cl, "/api/advise_take_keys");
    }
}

void delay_s(int s){
    for(int i = 0; i<s; i++) delay(1000);
}

unsigned long door_time;
bool keys_inside = false;

void loop() {
    if(doorOpen()){
        door_time = millis();

        if (in_house){
            //user was in house -> exiting
            sendOutHouse();
        }else{
            //user was outside -> is entering
            sendInHouse();
        }

        //state changed
        in_house = !in_house;
        delay_s(3);
    }
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.

    if ( rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        String key = String(rfid.uid.uidByte[3]);
        key.concat(rfid.uid.uidByte[2]);
        key.concat(rfid.uid.uidByte[1]);
        key.concat(rfid.uid.uidByte[0]);
        Serial.println(key);
        send_key(key);
        keys_inside ^= 1;
    }

    Serial.print("keys_inside ");
    Serial.print(keys_inside);
    Serial.print(" in_house ");
    Serial.println(in_house);
    if ((keys_inside != in_house) && (millis() - door_time) > key_time_thres) {
        Serial.println("something");
        if (keys_inside)
            send_take_keys();
        else
            send_leave_keys();
    }
    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
    delay (50);
}
