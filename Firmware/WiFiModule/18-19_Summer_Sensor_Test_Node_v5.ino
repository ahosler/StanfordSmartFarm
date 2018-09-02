//-----INCLUDES-----//
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMesh.h>
#include <stdio.h>
#include <string.h>
#include <Wire.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

//-----DEFINES-----//
//Solenoid I/O MOVED TO MEGA
/*
#define BED1 16
#define BED2 5
#define BED3 4
#define BED4 0
#define BED5 2
#define BED6 14
*/

//#define FIREBASE_HOST "stanford-farm.firebaseio.com"
//#define FIREBASE_AUTH "QcFhTkA1yECwRqv2vjhWFOrZAzoIPcElAHQPOUdj"
#define FIREBASE_HOST "stanford-smart-farm.firebaseio.com"
#define FIREBASE_AUTH "4JxR22sgq6LsuMhiTew2Qaq3Xh42JRukUvC46A5X"

//-----SETTINGS-----//
int postingInterval = 2000; // 10 seconds

// Irrigation
bool iG1 = false;
bool iG2 = false;
bool iG3 = false;
bool iG4 = false;
bool iG5 = false;
bool iG6 = false;
bool iG7 = false;
bool iG8 = false;
bool iG9 = false;
bool iG10 = false;
bool iG11 = false;
bool iG12 = false;
bool iG13 = false;
bool iG14 = false;
bool iG15 = false;


// Sensors
int g1_sensor1 = 100;
int g2_sensor1 = 100;
int g3_sensor1 = 100;
int g4_sensor1 = 100;
int g5_sensor1 = 100;
int g6_sensor1 = 100;


// Wi-Fi Settings
const char* ssid = "Stanford"; // your wireless network name (SSID)
const char* password = ""; // your Wi-Fi network password
//const char* ssid = "ahosler"; // your wireless network name (SSID)
//const char* password = "somethingsexy"; // your Wi-Fi network password
WiFiClient client;


//-----SETUP/LOOP-----//
void setup() {
  Serial.begin(115200);
  connectToInternet();
  beginFirebase();
  
}

void loop() {
  readAndUploadData();
  processChanges();
  delay(postingInterval);
  reconnectToInternet();
}

//-----SETUP METHODS-----//
void connectToInternet() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting");  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("WIFI Connected");
  Serial.println();
}

void reconnectToInternet() { 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}


void beginFirebase() {
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
}


//-----HELPER METHODS-----//
void readAndUploadData() {
  getSettings();
  getSensorReadings();
  getIrrigationFlags();
  setLiveFirebase();
  setDatabaseFirebase();
}

void processChanges() {
  // Initiate irrigation state
  String msg = "&1:" + String(iG1) + "#2:" + String(iG2) + "#3:" + String(iG3) + "#4:";
  msg += String(iG4) + "#5:" + String(iG5) + "#6:" + String(iG6) + "#7:" + String(iG7) + "#8:";
  msg += String(iG8) + "#9:" + String(iG9) + "#10:" + String(iG10) + "#11:" + String(iG11) + "#12:";
  msg += String(iG12) + "#13:" + String(iG13) + "#14:" + String(iG14) + "#15:" + String(iG15) + "#";
  Serial.println(msg);
}

//-----FIREBASE METHODS-----//
void getSensorReadings() {
  String typeStr;
  int type;
  String num;
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
       typeStr = Serial.readStringUntil(':');
       type = typeStr.toInt();
       switch (type) {
          case 1:
            num = Serial.readStringUntil('#');
            g1_sensor1 = num.toInt();
            break;
          case 2:
            num = Serial.readStringUntil('#');
            g2_sensor1 = num.toInt();
            break;
          case 3:
            num = Serial.readStringUntil('#');
            g3_sensor1 = num.toInt();
            break;
          case 4:
            num = Serial.readStringUntil('#');
            g4_sensor1 = num.toInt();
            break;
          case 5:
            num = Serial.readStringUntil('#');
            g5_sensor1 = num.toInt();
            break;
          case 6:
            num = Serial.readStringUntil('#');
            g6_sensor1 = num.toInt();
            break;
          default:
            ;
            //Serial.print("error: got the following string: ");
            //Serial.println(type);
       }
    }
  }
 /* Serial.println(g1_sensor1);
  Serial.println(g2_sensor1);
  Serial.println(g3_sensor1);
  Serial.println(g4_sensor1);
  Serial.println(g5_sensor1);
  Serial.println(g6_sensor1);*/
    /*Firebase.setInt("number", num);
    if (Firebase.failed()) {
        Serial.print("Firebase failure: ");
        Serial.println(Firebase.error());  
    } */
}

void getSettings() {
  //Serial.print("got the interval as:");
  int intervalSetting = Firebase.getInt("Settings/iInterval");
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
  postingInterval = intervalSetting * 1000;
  /*Serial.print("Sampling Rate: "); 
  Serial.print(intervalSetting); 
  Serial.println(" seconds");*/
}

void getIrrigationFlags() {
  iG1 = Firebase.getBool("iFlag/G1");
  iG2 = Firebase.getBool("iFlag/G2");
  iG3 = Firebase.getBool("iFlag/G3");
  iG4 = Firebase.getBool("iFlag/G4");
  iG5 = Firebase.getBool("iFlag/G5");
  iG6 = Firebase.getBool("iFlag/G6");
  iG7 = Firebase.getBool("iFlag/G7");
  iG8 = Firebase.getBool("iFlag/G8");
  iG9 = Firebase.getBool("iFlag/G9");
  iG10 = Firebase.getBool("iFlag/G10");
  iG11 = Firebase.getBool("iFlag/G11");
  iG12 = Firebase.getBool("iFlag/G12");
  iG13 = Firebase.getBool("iFlag/G13");
  iG14 = Firebase.getBool("iFlag/G14");
  iG15 = Firebase.getBool("iFlag/G15");
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
  
  
  /*Serial.print("iG1: "); Serial.println(iG1);
  Serial.print("iG2: "); Serial.println(iG2);
  Serial.print("iG3: "); Serial.println(iG3);
  Serial.print("iG4: "); Serial.println(iG4);
  Serial.print("iG5: "); Serial.println(iG5);
  Serial.print("iG6: "); Serial.println(iG6);
  Serial.print("iG7: "); Serial.println(iG7);
  Serial.print("iG8: "); Serial.println(iG8);
  Serial.print("iG9: "); Serial.println(iG9);
  Serial.print("iG10: "); Serial.println(iG10);
  Serial.print("iG11: "); Serial.println(iG11);
  Serial.print("iG12: "); Serial.println(iG12);
  Serial.print("iG13: "); Serial.println(iG13);
  Serial.print("iG14: "); Serial.println(iG14);
  Serial.print("iG15: "); Serial.println(iG15);
  Serial.println();
  */
}

void setLiveFirebase() {
  Firebase.setInt("Live/G1/sensor1/value", g1_sensor1);
  Firebase.setInt("Live/G2/sensor1/value", g2_sensor1);
  Firebase.setInt("Live/G3/sensor1/value", g3_sensor1);
  Firebase.setInt("Live/G4/sensor1/value", g4_sensor1);
  Firebase.setInt("Live/G5/sensor1/value", g5_sensor1);
  Firebase.setInt("Live/G6/sensor1/value", g6_sensor1);
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
}

void setDatabaseFirebase() {
  firebaseFormattedDatabaseCall("Database/G1/sensor1", g1_sensor1);
  firebaseFormattedDatabaseCall("Database/G2/sensor1", g2_sensor1);
  firebaseFormattedDatabaseCall("Database/G3/sensor1", g3_sensor1);
  firebaseFormattedDatabaseCall("Database/G4/sensor1", g4_sensor1);
  firebaseFormattedDatabaseCall("Database/G5/sensor1", g5_sensor1);
  firebaseFormattedDatabaseCall("Database/G6/sensor1", g6_sensor1);
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
}

void firebaseFormattedDatabaseCall(String path, int adc) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& upload = jsonBuffer.createObject();
  JsonObject& uploadTime = upload.createNestedObject("timestamp");
  upload["value"] = adc;
  uploadTime[".sv"] = "timestamp";
  Firebase.push(path, upload);
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
}

void resetAll() {
  Serial.println("RESETING....");
  delay(100);
  ESP.restart();
}

