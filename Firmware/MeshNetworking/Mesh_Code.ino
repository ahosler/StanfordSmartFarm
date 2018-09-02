/*
 * Sensor 1 Code
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMesh.h>
#include <stdio.h>
#include <string.h>

const char* ssid = "Stanford"; // your wireless network name (SSID)
const char* password = ""; // your Wi-Fi network password

unsigned int request_i = 0;
unsigned int response_i = 0;

String manageRequest(String request);

/* Create the mesh node object */
ESP8266WiFiMesh mesh_node = ESP8266WiFiMesh(ESP.getChipId(), manageRequest);

/**
 * Callback for when other nodes send you data
 *
 * @request The string received from another node in the mesh
 * @returns The string to send back to the other node
 */
String manageRequest(String request)
{
  /* Print out received message */
  Serial.print("received: ");
  Serial.println(request); 

  /* return a string to send back */
  char response[60];
  sprintf(response,"Hello world request #%d from Mesh_Node%d.", response_i++, ESP.getChipId());
  return response;
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  connectToInternet();
  delay(10);

  Serial.println();
  Serial.println();
  Serial.println("Setting up mesh node...");

  /* Initialise the mesh node */
  mesh_node.begin();
}

void loop()
{
  /* Accept any incoming connections */
  mesh_node.acceptRequest();

  /* Scan for other nodes and send them a message */
  char request[60];
  sprintf(request, "What the fuck is up boys #%d", request_i++);
  mesh_node.attemptScan(request);
  delay(500);
   long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);
  delay(500);
}

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
