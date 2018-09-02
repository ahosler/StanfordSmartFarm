#include <SoftwareSerial.h>
#define S011 A0
#define S021 A1
#define S031 A2
#define S041 A3
#define S051 A4
#define S061 A5

int s011 = 0;
int s021 = 0;
int s031 = 0;
int s041 = 0;
int s051 = 0;
int s061 = 0;
SoftwareSerial sw(2, 3); // RX, TX

void setup() {
 Serial.begin(115200);
 Serial.println("Interfacfing arduino with nodemcu");
 sw.begin(115200);
}

void loop() {
/*  s011 = analogRead(S011);
  s021 = analogRead(S021);
  s031 = analogRead(S031);
  s041 = analogRead(S041);
  s051 = analogRead(S051);
  s061 = analogRead(S061);
*/
  s011 += 1;
  s021 += 1;
  s031 += 1;
  s041 += 1;
  s051 += 1;
  s061 += 1;

  
  String msg = "1:" + String(s011) + "#2:" + String(s021) + "#3:" + String(s031) + "#4:";
  msg += String(s041) + "#5:" + String(s051) + "#6:" + String(s061) + "#";
  Serial.println(msg);
  sw.println(msg);
  delay(5000);
}
