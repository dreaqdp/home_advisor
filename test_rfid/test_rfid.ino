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

#define SS_PIN 4
#define RST_PIN 5
#define fam 2

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;


//server config
const char* ssid = "marenostrum";
const char* password = "0123456789";
//const char * host = "192.168.1.119";
const char * host = "192.168.1.13";
const uint16_t port = 3000;


void setup () {
    Serial.begin(9600);
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522

    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

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
}

bool put_petition (WiFiClient &cl,  String path, String key) {
  cl.print("PUT " + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "uid: " + key + "\r\n" + "Connection: close\r\n" + "\r\n");
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

void loop () {

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! rfid.PICC_IsNewCardPresent())
        return;

    // Verify if the NUID has been readed
    if ( ! rfid.PICC_ReadCardSerial())
        return;
    /*
       int i:
       for (i = 0; i < fam; i++) {
       if (rfid.uid.uidByte[0] == nuidPICC[i][0] && rfid.uid.uidByte[1] == nuidPICC[i][1] && rfid.uid.uidByte[2] == nuidPICC[i][2] && rfid.uid.uidByte[3] == nuidPICC[i][3]) 
       member = i;

       }
     */

    String key = String(rfid.uid.uidByte[3]);
    key.concat(rfid.uid.uidByte[2]);
    key.concat(rfid.uid.uidByte[1]);
    key.concat(rfid.uid.uidByte[0]);
    Serial.println(key);
//    while (!send_key(key));
    send_key(key);  



    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();


}

