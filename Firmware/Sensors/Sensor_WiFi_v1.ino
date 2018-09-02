//-----INCLUDES-----//
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMesh.h>
#include <stdio.h>
#include <string.h>
//#include <Wire.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

//-----DEFINES-----//
#define FIREBASE_HOST "stanford-smart-farm.firebaseio.com"
#define FIREBASE_AUTH "4JxR22sgq6LsuMhiTew2Qaq3Xh42JRukUvC46A5X"

//-----SENSOR PINS---//
#define SENSOR A0

//------IDs-----------// !!!!!!!!!!TODO: CHANGE ME!!!!!!!!!
#define bedID 1
#define sensorID 1

//-----SETTINGS-----//
int postingInterval = 5000; // 5 seconds

// Sensors
int sensor_val = 99;


// Wi-Fi Settings
const char* ssid = "Stanford"; // your wireless network name (SSID)
const char* password = ""; // your Wi-Fi network password
WiFiClient client;

//-----SETUP/LOOP-----//
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  connectToInternet();
  beginFirebase();
}

void loop() {
  readSensor();
  uploadData();
  //delay(postingInterval);
  Serial.println("Going to sleep");
  ESP.deepSleep(postingInterval*1000);
  Serial.println("This will never be printed::: PENIS");
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
  Serial.print("Reconnecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("WIFI Connected");
  Serial.println();
}


void beginFirebase() {
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
  Serial.println("Firebase Connected");
}


//-----HELPER METHODS-----//
void readSensor() {
  getSettings();
  sensor_val = analogRead(SENSOR);
  Serial.print("The adc output of the sensor is: ");
  Serial.println(sensor_val);
}

void uploadData() {
  setLiveFirebase();
  setDatabaseFirebase();
}



void getSettings() {
  int intervalSetting = Firebase.getInt("Settings/sInterval");
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
  postingInterval = intervalSetting * 1000;
  Serial.print("Sampling Rate: "); 
  Serial.print(intervalSetting); 
  Serial.println(" seconds");
}


void setLiveFirebase() {
  char path[100];
  sprintf(path, "Live/G%d/sensor%d/value", bedID, sensorID);
  Firebase.setInt(path, sensor_val);
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
}

void setDatabaseFirebase() {
  char path[100];
  sprintf(path, "Database/G%d/sensor%d", bedID, sensorID);
  firebaseFormattedDatabaseCall(path, sensor_val);
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

