//#include <Wire.h>
//#include <SoftwareSerial.h>
//#include <ESP8266WiFi.h>
#include <Wiegand.h>
//#define wifiEN 0;
WIEGAND wg; //catches data from keypad
//SoftwareSerial Serial(14, 12, false, 256); //save for possible future use 
const char* keyA = "123WRX";
const char* keyB = "113WEX";
const char* keyC = "223WRT";

const char* ssid     = "Q46JP_EXT";
const char* password = "92DDA91C4D";

const char* host = "data.sparkfun.com";
const char* streamId   = "....................";
const char* privateKey = "....................";
char* keyString;

void setup() {
//get serial going
  Serial.begin(9600);
  Serial.println("Hello!");
  Serial.println("Arduino started");

  delay(10);

  // We start by connecting to a WiFi network

//connect to internet & Azure
//wifi setup
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#if defined (wifiEN)
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

#endif
  

//initialize display 

//setup relay
/*
 #define CH1 10 //CH# ardPin#
 */
}
int value = 0;
void loop() {
  // put your main code here, to run repeatedly:
//listen for key from NFC

//if key, then start keypad
//listen for keypad
//if keypad check key & pin against azure
//if match then open, else log & block where applicable
//open
  while (Serial.available() > 0) {
    Serial.write(Serial.read());
  }
  while (Serial.available() > 0) {
    Serial.write(Serial.read());
  }
  delay(5000);
  ++value;
#if defined (wifiEN)
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
        Serial.println();
  Serial.println("closing connection");
  
    }
  }
  #endif
  //check for NFC shield, keypad
  wg.begin();

  if(wg.available())
  {
    Serial.print("Wiegand HEX = ");
    Serial.print(wg.getCode(),HEX);
    Serial.print(", DECIMAL = ");
    //strcpy(keyString, wg.getCode());
    //Serial.print(keyString);
    Serial.print(wg.getCode());
    Serial.print(", Type W");
    Serial.println(wg.getWiegandType());    
    
  if(keyString == (keyA || keyB || keyC)) 
  {
    
    while(keyString != 13){
    //push buttons to string   
    strcpy(keyString, wg.getCode());
    }
  } else if (keyString = "2496273"){
  Serial.println("Blacklisted card");
  } else {
  keyString = "";
  }
  Serial.println(keyString);
  }
  



  /*
if(ok){
  digitalWrite(CH1, HIGH); //sets arduino pin #10 to HIGH
  delay(1000);
  digitalWrite(CH1, LOW);   //stops opening door circuit
}
*/
}
