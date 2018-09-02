//-----DEFINES-----//
// Sensor Pins
#define S011 A0
#define S021 A1
#define S031 A2
#define S041 A3
#define S051 A4
#define S061 A5

//Solenoid I/O 
#define BED1 31
#define BED2 32
#define BED3 33
#define BED4 34
#define BED5 35
#define BED6 36
#define BED7 37
#define BED8 38
#define BED9 39
#define BED10 40
#define BED11 41
#define BED12 42
#define BED13 43
#define BED14 44
#define BED15 45

//-----SETTINGS-----//
int readingDelay = 1000;

// Sensor Values
int s011 = 0;
int s021 = 0;
int s031 = 0;
int s041 = 0;
int s051 = 0;
int s061 = 0;

// Irrigation States
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

// Begin Functionality
void setup() {
 Serial.begin(115200);
 Serial.println("Interfacfing arduino with nodemcu");
 Serial2.begin(115200);
 analogReference(INTERNAL2V56);
 pinModeSetup();
}

void loop() {
  readSensors();
  getIrrigationFlags();
  updateIrrigation();
  delay(readingDelay);
}

// Setup all Irrigation pins and initialize to off
void pinModeSetup() {
  pinMode(BED1, OUTPUT);
  pinMode(BED2, OUTPUT);
  pinMode(BED3, OUTPUT);
  pinMode(BED4, OUTPUT);
  pinMode(BED5, OUTPUT);
  pinMode(BED6, OUTPUT);
  pinMode(BED7, OUTPUT);
  pinMode(BED8, OUTPUT);
  pinMode(BED9, OUTPUT);
  pinMode(BED10, OUTPUT);
  pinMode(BED11, OUTPUT);
  pinMode(BED12, OUTPUT);
  pinMode(BED13, OUTPUT);
  pinMode(BED14, OUTPUT);
  pinMode(BED15, OUTPUT);
  
  digitalWrite(BED1, LOW);
  digitalWrite(BED2, LOW);
  digitalWrite(BED3, LOW);
  digitalWrite(BED4, LOW);
  digitalWrite(BED5, LOW);
  digitalWrite(BED6, LOW);
  digitalWrite(BED7, LOW);
  digitalWrite(BED8, LOW);
  digitalWrite(BED9, LOW);
  digitalWrite(BED10, LOW);
  digitalWrite(BED11, LOW);
  digitalWrite(BED12, LOW);
  digitalWrite(BED13, LOW);
  digitalWrite(BED14, LOW);
  digitalWrite(BED15, LOW);
}

void readSensors() {
  s011 = analogRead(S011);
  s021 = analogRead(S021);
  s031 = analogRead(S031);
  s041 = analogRead(S041);
  s051 = analogRead(S051);
  s061 = analogRead(S061);

  String msg = "@1:" + String(s011) + "#2:" + String(s021) + "#3:" + String(s031) + "#4:";
  msg += String(s041) + "#5:" + String(s051) + "#6:" + String(s061) + "#!";
  //Serial.println(msg);
  Serial2.println(msg);
}

//Example:
void getIrrigationFlags() {
  String typeStr;
  int type;
  String num;
  if (Serial2.available() > 0) {
    //typeStr = Serial2.readString();
    //Serial.println(typeStr);
    String myst = Serial2.readStringUntil('&');
    //Serial.print("MYST: ");
    //Serial.println(myst)
    while (Serial2.available() > 0) {
       typeStr = Serial2.readStringUntil(':');
       //Serial.print("Writing Bed ");
       //Serial.println(typeStr);
       type = typeStr.toInt();
       switch (type) {
          case 1:
            num = Serial2.readStringUntil('#');
            iG1 = num.toInt();
            break;
          case 2:
            num = Serial2.readStringUntil('#');
            iG2 = num.toInt();
            break;
          case 3:
            num = Serial2.readStringUntil('#');
            iG3 = num.toInt();
            break;
          case 4:
            num = Serial2.readStringUntil('#');
            iG4 = num.toInt();
            break;
          case 5:
            num = Serial2.readStringUntil('#');
            iG5 = num.toInt();
            break;
          case 6:
            num = Serial2.readStringUntil('#');
            iG6 = num.toInt();
            break;
          case 7:
            num = Serial2.readStringUntil('#');
            iG7 = num.toInt();
            break;
          case 8:
            num = Serial2.readStringUntil('#');
            iG8 = num.toInt();
            break;
          case 9:
            num = Serial2.readStringUntil('#');
            iG9 = num.toInt();
            break;
          case 10:
            num = Serial2.readStringUntil('#');
            iG10 = num.toInt();
            break;
          case 11:
            num = Serial2.readStringUntil('#');
            iG11 = num.toInt();
            break;
          case 12:
            num = Serial2.readStringUntil('#');
            iG12 = num.toInt();
            break;
          case 13:
            num = Serial2.readStringUntil('#');
            iG13 = num.toInt();
            break;
          case 14:
            num = Serial2.readStringUntil('#');
            iG14 = num.toInt();
            break;
          case 15:
            num = Serial2.readStringUntil('#');
            iG15 = num.toInt();
            break;
          case 16:
            num = Serial2.readStringUntil('#');
            break;
          default:
            Serial.print("error: got the following string: ");
            Serial.println(typeStr);
       }
       //Serial.print(" to be: ");
       //Serial.println(num.toInt());
    }
  }
  Serial.print("iG1: "); Serial.println(iG1);
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
  Serial.print("iG15: "); Serial.println(iG14);
  Serial.println();
}

void updateIrrigation() {
  digitalWrite(BED1, iG1);
  digitalWrite(BED2, iG2);
  digitalWrite(BED3, iG3);
  digitalWrite(BED4, iG4);
  digitalWrite(BED5, iG5);
  digitalWrite(BED6, iG6);
  digitalWrite(BED7, iG7);
  digitalWrite(BED8, iG8);
  digitalWrite(BED9, iG9);
  digitalWrite(BED10, iG10);
  digitalWrite(BED11, iG11);
  digitalWrite(BED12, iG12);
  digitalWrite(BED13, iG13);
  digitalWrite(BED14, iG14);
  digitalWrite(BED15, iG14);
}

