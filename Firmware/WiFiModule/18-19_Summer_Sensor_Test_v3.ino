//-----INCLUDES-----//
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

//-----DEFINES-----//
//Solenoid I/O
#define BED1 8
#define BED2 9
#define BED3 10
#define BED4 11
#define BED5 12
#define BED6 13

//Sensor I/O
#define SENSOR011 A0
#define SENSOR021 A1
#define SENSOR031 A2
#define SENSOR041 A3
#define SENSOR051 A4
#define SENSOR061 A5

//#define FIREBASE_HOST "stanford-farm.firebaseio.com"
//#define FIREBASE_AUTH "QcFhTkA1yECwRqv2vjhWFOrZAzoIPcElAHQPOUdj"
#define FIREBASE_HOST "stanford-smart-farm.firebaseio.com"
#define FIREBASE_AUTH "4JxR22sgq6LsuMhiTew2Qaq3Xh42JRukUvC46A5X"

//-----SETTINGS-----//
int postingInterval = 1000; // 5 seconds

// Wi-Fi Settings
//const char* ssid = "Stanford"; // your wireless network name (SSID)
//const char* password = ""; // your Wi-Fi network password
const char* ssid = "ahosler"; // your wireless network name (SSID)
const char* password = "somethingsexy"; // your Wi-Fi network password
WiFiClient client;

bool state1 = 0;
bool state2 = 0;
bool state3 = 0;
bool state4 = 0;
bool state5 = 0;
bool state6 = 0;

//-----SETUP/LOOP-----//
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  connectToInternet();
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
}

void loop() {
  readAndUploadData();
  delay(postingInterval);
}

//-----SETUP METHODS-----//
void connectToInternet() {
  Serial.print("Connecting");  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("WIFI Connected");
  Serial.println();
}
  int num = 0;
  int digit = 0;
//-----HELPER METHODS-----//
void readAndUploadData() {
  if (Serial.available() > 0) {
    num = 0;
    while (Serial.available() > 0) {
       digit = Serial.read();
       if (digit != 13 && digit != 10) {
          num *= 10;
          num += digit-48;
       } 
    }
    Serial.println(num);
    if (num == 1) {
      state1 = !state1;
      digitalWrite(D0, state1);
    } else if (num == 2) {
      state2 = !state2;
      digitalWrite(D1, state2);
    } else if (num == 3) {
      state3 = !state3;
      digitalWrite(D2, state3);
    } else if (num == 4) {
      state4 = !state4;
      digitalWrite(D3, state4);
    } else if (num == 5) {
      state5 = !state5;
      digitalWrite(D4, state5);
    } else if (num == 6) {
      state6 = !state6;
      digitalWrite(D5, state6);
    }
    Firebase.setInt("number", num);
    if (Firebase.failed()) {
        Serial.print("Firebase failure: ");
        Serial.println(Firebase.error());  
    }
  }  
  delay(1000);
  /*
  //int g1int = Firebase.getInt("Live/G1/sensor1");
  Firebase.setInt("S1", 100);
  Serial.print("G1 Integer: SET"); //Serial.println(g1int);
  if (Firebase.failed()) {
      Serial.print("Firebase failure: ");
      Serial.println(Firebase.error());  
  }
  
  delay(1000);
  Serial.println();
  */
//  Firebase.setInt("Live/G1/sensor1", 69);
//  Firebase.setInt(("Live/G2"), adc1);
//  Firebase.setInt(("Live/G3"), adc1);
//  Firebase.setInt(("Live/G4"), adc1);
//  Firebase.setInt(("Live/G5"), adc1);
//  Firebase.setInt(("Live/G6"), adc1);
//
//  String path = "Database/G1/sensor1";
//  DynamicJsonBuffer jsonBuffer;
//  JsonObject& upload = jsonBuffer.createObject();
//  JsonObject& uploadTime = upload.createNestedObject("timestamp");
//  upload["value"] = adc0;
//  uploadTime[".sv"] = "timestamp";
//  Firebase.push(path, upload);
}
