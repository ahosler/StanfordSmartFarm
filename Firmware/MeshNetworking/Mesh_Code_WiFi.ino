
//-----INCLUDES-----//
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMesh.h>
#include <stdio.h>
#include <string.h>
#include <Wire.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

//-----DEFINES-----//
//Solenoid I/O ON THE MEGA

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
WiFiClient client;

unsigned int request_i = 0;
unsigned int response_i = 0;
String msgToSend;
String sensorStr;

String manageRequest(String request);

/* Create the mesh node object */
ESP8266WiFiMesh mesh_node = ESP8266WiFiMesh(ESP.getChipId(), manageRequest);

/**
 * Callback for when other nodes send you data
 *
 * @request The string received from another node in the mesh
 * @returns The string to send back to the other node
 */
String manageRequest(String response) {
  /* Print out received message */
  sensorStr = response;
  Serial.print("received sensor string: ");
  Serial.println(sensorStr); 

  /* return a string to send back */
  return msgToSend;
}

//-----SETUP/LOOP-----//
void setup() {
  Serial.begin(115200);
  connectToInternet();
  beginFirebase();
  mesh_node.begin();
}

void loop() {
  readAndUploadData();
  processChanges();
  sendMesh();
  delay(postingInterval);
  connectToInternet();
  
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
  msg += String(iG12) + "#13:" + String(iG13) + "#14:" + String(iG14) + "#15:" + String(iG15) + "#" + String(response_i);
  Serial.println("Sending the following flags:");
  Serial.println(msg);
  Serial.println();
  response_i++;
  msgToSend = msg;
}

void sendMesh() {
  /* Accept any incoming connections */
  mesh_node.acceptRequest();

  /* Scan for other nodes and send them a message */
  mesh_node.attemptScan(msgToSend);
  delay(500);
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);
  delay(500);
}

//-----SUB-HELPER METHODS-----//

void getSettings() {
  int intervalSetting = Firebase.getInt("Settings/iInterval");
  if (Firebase.failed()) {
    Serial.print("Firebase failure: ");
    Serial.println(Firebase.error());  
    resetAll();
  }
  //postingInterval = intervalSetting * 1000;
  Serial.print("Sampling Rate: "); 
  Serial.print(intervalSetting); 
  Serial.println(" seconds");
}

void getSensorReadings() { //TODO: turn this into parsing a string
  int ind = sensorStr.indexOf('@');
  g1_sensor1 = sensorStr.substring(sensorStr.indexOf('1:', ind), sensorStr.indexOf('#', ind)).toInt();
  ind = sensorStr.indexOf('#', ind);
  g2_sensor1 = sensorStr.substring(sensorStr.indexOf('2:', ind), sensorStr.indexOf('#', ind)).toInt();
  ind = sensorStr.indexOf('#', ind);
  g3_sensor1 = sensorStr.substring(sensorStr.indexOf('3:', ind), sensorStr.indexOf('#', ind)).toInt();
  ind = sensorStr.indexOf('#', ind);
  g4_sensor1 = sensorStr.substring(sensorStr.indexOf('4:', ind), sensorStr.indexOf('#', ind)).toInt();
  ind = sensorStr.indexOf('#', ind);
  g5_sensor1 = sensorStr.substring(sensorStr.indexOf('5:', ind), sensorStr.indexOf('#', ind)).toInt();
  ind = sensorStr.indexOf('#', ind);
  g6_sensor1 = sensorStr.substring(sensorStr.indexOf('6:', ind), sensorStr.indexOf('#', ind)).toInt();
  Serial.print("Sensor 1: ");
  Serial.println(g1_sensor1);
  Serial.print("Sensor 2: ");
  Serial.println(g2_sensor1);
  Serial.print("Sensor 3: ");
  Serial.println(g3_sensor1);
  Serial.print("Sensor 4: ");
  Serial.println(g4_sensor1);
  Serial.print("Sensor 5: ");
  Serial.println(g5_sensor1);
  Serial.print("Sensor 6: ");
  Serial.println(g6_sensor1);
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






    /*Firebase.setInt("number", num);
    if (Firebase.failed()) {
        Serial.print("Firebase failure: ");
        Serial.println(Firebase.error());  
    } */
