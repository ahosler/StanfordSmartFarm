
//-----INCLUDES-----//
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMesh.h>
#include <stdio.h>
#include <string.h>
#include <Wire.h>

//Mesh Message
String msgToSend;
String manageRequest(String request);

/* Create the mesh node object */
ESP8266WiFiMesh mesh_node = ESP8266WiFiMesh(ESP.getChipId(), manageRequest);

/**
 * Callback for when other nodes send you data
 *
 * @request The string received from another node in the mesh
 * @returns The string to send back to the other node
 */
String manageRequest(String IRStr) {
  /* Print out received message */
  Serial.println(IRStr);

  /* return a string to send back */
  return msgToSend;
}

//-----SETUP/LOOP-----//
void setup() {
  Serial.begin(115200);
  mesh_node.begin();
  Serial.println("Done with set up");
}

void loop() {
  getSensorStr();
  getMesh();
  delay(500);
}

/*
 * TODO: REMOVE PRINT STATEMENT, TURN WHILE TO 2ND IF
 */
void getSensorStr() {
  if (Serial.available() > 0) {
    Serial.readStringUntil('@');
    while (Serial.available() > 0) {
      msgToSend = Serial.readStringUntil('!');
      Serial.println(msgToSend);
    }
  }
}

void getMesh() {
  /* Accept any incoming connections */
  mesh_node.acceptRequest();
  /* Scan for other nodes and send them a message */
  mesh_node.attemptScan(msgToSend);
  delay(1000);
}




