#//include <Wire.h>
#//include <FirebaseArduino.h>
#//include <ArduinoJson.h>
#define FLOWSENSORPIN 2
#define SENSOR011 A0
#define SENSOR021 A1
#define SENSOR031 A2
#define SENSOR041 A3
#define SENSOR051 A4
#define SENSOR061 A5

#define STEMP A15

#define BED1 8
#define BED2 9
#define BED3 10
#define BED4 11
#define BED5 12
#define BED6 13

#define esp8266 Serial2
#define CH_PD 4 
#define speed8266 115200 

 
#define DEBUG true

int s011;
int s021;
int s031;
int s041;
int s051;
int s061;
int stemp;

int BED1STATE;
int BED2STATE;
int BED3STATE;
int BED4STATE;
int BED5STATE;
int BED6STATE;

int bed1flow;
int bed2flow;
int bed3flow;
int bed4flow;
int bed5flow;
int bed6flow;
/*
// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
  
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}
*/
void setup() {
   // Init the ESP
   esp8266.begin (speed8266); 
   Serial.begin(9600);
   reset8266(); // Pin CH_PD needs a reset before start communication
   InitWifiModule(); // Inciate module as WebServer 
   Serial.println("MCU Readout:");

   // Init the Flow Rate Sensor
   //pinMode(FLOWSENSORPIN, INPUT);
   //digitalWrite(FLOWSENSORPIN, HIGH);
   //lastflowpinstate = digitalRead(FLOWSENSORPIN);
   //useInterrupt(true);

   
   //Init the irrigation Beds
   pinMode(BED1, OUTPUT);
   pinMode(BED2, OUTPUT);
   pinMode(BED3, OUTPUT);
   pinMode(BED4, OUTPUT);
   pinMode(BED5, OUTPUT);
   pinMode(BED6, OUTPUT);

   BED1STATE = 0;
   BED2STATE = 0;
   BED3STATE = 0;
   BED4STATE = 0;
   BED5STATE = 0;
   BED6STATE = 0;
   
   digitalWrite(BED1, BED1STATE);
   digitalWrite(BED2, BED2STATE);
   digitalWrite(BED3, BED3STATE);
   digitalWrite(BED4, BED4STATE);
   digitalWrite(BED5, BED5STATE);
   digitalWrite(BED6, BED6STATE);

   
   
}

void loop()                     // run over and over again
{ 
  // Flow Meter
  //Serial.print("Freq: "); Serial.println(flowrate);
  //Serial.print("Pulses: "); Serial.println(pulses, DEC);
  //float liters = pulses;
  //liters /= 7.5;
  //liters /= 60.0;
  //Serial.print(liters); Serial.println(" Liters");

  //Sensor Read Outs
  s011 = analogRead(SENSOR011);
  //Serial.print("Bed 1, Sensor 1 readout: "); Serial.println(s011);
  s021 = analogRead(SENSOR021);
  //Serial.print("Bed 2, Sensor 1 readout: "); Serial.println(s021);
  s031 = analogRead(SENSOR031);
  //Serial.print("Bed 3, Sensor 1 readout: "); Serial.println(s031);
  s041 = analogRead(SENSOR041);
  //Serial.print("Bed 4, Sensor 1 readout: "); Serial.println(s041);
  s051 = analogRead(SENSOR051);
  //Serial.print("Bed 5, Sensor 1 readout: "); Serial.println(s051);
  s061 = analogRead(SENSOR061);
  //Serial.print("Bed 6, Sensor 1 readout: "); Serial.println(s061);
  stemp = analogRead(STEMP);
  //Serial.print("Soil Temperature: "); Serial.println(stemp);
  //delay(100);
  
  /* Insert Algorithm for whether or not to turn on solenoids, which ones, etc
   *  If S1 <= thresh_S1:
   *    water s1
   *    change s1 state
   *  
   */

  //delay(1000);
  // Read Irrigation Values
  if(esp8266.available())
  {
    Serial.println("ESP IS AVALIABLE!!!!!!!!!!!!");
    if(esp8266.find("+IPD,"))
    {
      delay(1000); 
      Serial.println("HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
      int connectionId = esp8266.read()-48;  
      int ret1 = esp8266.find("pin="); 
      
      Serial.println(ret1);
      if(ret1 == 1) {
        int type = (esp8266.read()-48);
        Serial.print("THE TYYYYYPPPEEE ISSSSS:    ");
        Serial.println(type);
  
        if (type == 1) {
        
          int pinNumber = (esp8266.read()-48)*10; 
          pinNumber += (esp8266.read()-48); 
          int statusLed =(esp8266.read()-48);
          Serial.print("setting pin: ");
          Serial.print(pinNumber);
          Serial.print(" to: ");
          Serial.println(statusLed);
          
          switch(pinNumber) {
            case BED1: BED1STATE = statusLed;
              break;
            case BED2: BED2STATE = statusLed;
              break;
            case BED3: BED3STATE = statusLed;
              break;
            case BED4: BED4STATE = statusLed;
              break;
            case BED5: BED5STATE = statusLed;
              break;
            case BED6: BED6STATE = statusLed;
            default: break;
          }
          Serial.println(BED1STATE);
          Serial.println(BED2STATE);
          Serial.println(BED3STATE);
          Serial.println(BED4STATE);
          Serial.println(BED5STATE);
          Serial.println(BED6STATE);
          digitalWrite(pinNumber, statusLed);
        }
      } else {
        // Read Sensor Values
        Serial.println("Did not find anything to write, so refreshing website");
        String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
        
        

        
        webpage += "<script src=\"https://ajax.aspnetcdn.com/ajax/jQuery/jquery-3.3.1.min.js\"></script>";
        webpage += "</head><h1>Stanford Smart Farm Controls Page</h1><h2>Bed 1, Sensor 1 readout: ";
        webpage += s011;      
        webpage += "<h4>Bed 1: Buckwheat";
        if (BED1STATE) {
          webpage += "<button style=\"color:black\"; id=\"1081\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:red\"; id=\"1080\" class=\"led\">OFF</button>";
        } else {
          webpage += "<button style=\"color:blue\"; id=\"1081\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:black\"; id=\"1080\" class=\"led\">OFF</button>";
        }
        webpage += "<h4> Select the threshold Value you would like:  ";
        webpage +="<button style=\"color:black\"; id=\"9999\" class=\"led\">200</button> ";
        webpage +="<button style=\"color:black\"; id=\"9999\" class=\"led\">250</button> ";
        webpage +="<button style=\"color:black\"; id=\"9999\" class=\"led\">300</button> ";
        webpage +="<button style=\"color:black\"; id=\"9999\" class=\"led\">350</button> ";
        webpage += "<h2>Bed 2, Sensor 1 readout: ";
        webpage += s021;
        webpage += "<h4>Bed 2: Buckwheat";
        if (BED2STATE) {
          webpage += "<button style=\"color:black\"; id=\"1091\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:red\"; id=\"1090\" class=\"led\">OFF</button>";
        } else {
          webpage += "<button style=\"color:blue\"; id=\"1091\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:black\"; id=\"1090\" class=\"led\">OFF</button>";
        }
        webpage += "<h2>Bed 3, Sensor 1 readout: ";
        webpage += s031;
        webpage += "<h4>Bed 3: Buckwheat";
        if (BED3STATE) {
          webpage += "<button style=\"color:black\"; id=\"1101\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:red\"; id=\"1100\" class=\"led\">OFF</button>";
        } else {
          webpage += "<button style=\"color:blue\"; id=\"1101\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:black\"; id=\"1100\" class=\"led\">OFF</button>";
        }
        webpage += "<h2>Bed 4, Sensor 1 readout: ";
        webpage += s041;
        webpage += "<h4>Bed 4: Buckwheat";
        if (BED4STATE) {
          webpage += "<button style=\"color:black\"; id=\"1111\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:red\"; id=\"1110\" class=\"led\">OFF</button>";
        } else {
          webpage += "<button style=\"color:blue\"; id=\"1111\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:black\"; id=\"1110\" class=\"led\">OFF</button>";
        }
        webpage += "<h2>Bed 5, Sensor 1 readout: ";
        webpage += s051;
        webpage += "<h4>Bed 5: Buckwheat";
        if (BED5STATE) {
          webpage += "<button style=\"color:black\"; id=\"1121\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:red\"; id=\"1120\" class=\"led\">OFF</button>";
        } else {
          webpage += "<button style=\"color:blue\"; id=\"1121\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:black\"; id=\"1120\" class=\"led\">OFF</button>";
        }
        webpage += "<h2>Bed 6, Sensor 1 readout: ";
        webpage += s061;
        webpage += "<h4>Bed 6: Buckwheat";
        if (BED1STATE) {
          webpage += "<button style=\"color:black\"; id=\"1131\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:red\"; id=\"1130\" class=\"led\">OFF</button>";
        } else {
          webpage += "<button style=\"color:blue\"; id=\"1131\" class=\"led\">ON</button> ";
          webpage += "<button style=\"color:black\"; id=\"1130\" class=\"led\">OFF</button>";
        }
        webpage += "<h2>Soil Temperature: ";
        webpage += stemp;
        webpage += "</h2>";

// BART!!!!!!!!!!!! ADD THE IP ADDRESS BELOW, IN THE FORMAT OF HTTP://XX.XX.XX.XX:80/
        webpage += "<script> $(\".led\").click(function() {    var p = $(this).attr('id');     $.get(\"http://10.35.122.11:80/\", {pin:p}); });</script>";

        
        String cipSend = "AT+CIPSEND=";
        cipSend += connectionId;
        cipSend += ",";
        cipSend += webpage.length();
        cipSend += "\r\n";
   
        sendData(cipSend, 1000, DEBUG);
        sendData(webpage, 1000, DEBUG);
      }
      
      String closeCommand = "AT+CIPCLOSE="; 
      closeCommand+=connectionId; 
      closeCommand+="\r\n";
      sendData(closeCommand,1000,DEBUG); 
      Serial.println("Data sent, connection closed");
    }
    Serial.println("could not find IDP");
  }
  Serial.println("ESP not avaliable");
}

/*****************************************************************************************
* Name: sendData
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }    
    return response;
}




/***********************************************************************/  
// This initializes the Wifi Module as a server 
void InitWifiModule()
{
  sendData("AT+RST\r\n", 2000, DEBUG); // reset
  sendData("AT+CWJAP=\"Stanford\",\"\"\r\n", 2000, DEBUG); //Connect network
  delay(5000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // Show IP Adress
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); // Multiple conexions
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // start comm port 80
}

/*************************************************/
// Reset funtion to accept communication
void reset8266 ()
{
  pinMode(CH_PD, OUTPUT);
  digitalWrite(CH_PD, LOW);
  delay(300);
  digitalWrite(CH_PD, HIGH);
}



