#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial serialOut(14, 12, false, 256);

const char* ssid     = "Q46JP_EXT";
const char* password = "92DDA91C4D";

const char* host = "data.sparkfun.com";
const char* streamId   = "....................";
const char* privateKey = "....................";

void setup() {
  // put your setup code here, to run once:
  // includes for PN532, internet, display
  //wemos wifi library headers
  Serial.begin(115200);
  serialOut.begin(115200);

  serialOut.println("\n\nArduino started\n");
  /* //test serial output
  for (char ch = ' '; ch <= 'z'; ch++) {
    serialOut.write(ch);
  }
  serialOut.println("\n");
*/
  delay(10);

  // We start by connecting to a WiFi network

  serialOut.println();
  serialOut.print("Connecting to ");
  serialOut.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    serialOut.print(".");
  }

  serialOut.println("");
  serialOut.println("WiFi connected");  
  serialOut.println("IP address: ");
  serialOut.println(WiFi.localIP());

  /*

//NFC lib headers & definitions
#include <PN532.h>
#include <SPI.h>
#define PN532_CS 10
PN532 nfc(PN532_CS);
#define  NFC_DEMO_DEBUG 1
//lcd library headers
#define bool ok = FALSE
*/

//check for NFC shield, keypad
//NFC setup
/*
#ifdef NFC_DEMO_DEBUG
  Serial.begin(9600);
  Serial.println("Hello!");
#endif
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
#ifdef NFC_DEMO_DEBUG
    Serial.print("Didn't find PN53x board");
#endif
    while (1); // halt
  }
#ifdef NFC_DEMO_DEBUG
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); 
  Serial.println((versiondata>>8) & 0xFF, DEC);
  Serial.print("Supports "); 
  Serial.println(versiondata & 0xFF, HEX);
#endif
  // configure board to read RFID tags and cards
  nfc.SAMConfig();
 */
//connect to internet & Azure
//wifi setup

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
  while (serialOut.available() > 0) {
    Serial.write(serialOut.read());
  }
  while (Serial.available() > 0) {
    serialOut.write(Serial.read());
  }
  delay(5000);
  ++value;

  serialOut.print("connecting to ");
  serialOut.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    serialOut.println("connection failed");
    return;
  }
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      serialOut.println(">>> Client Timeout !");
      client.stop();
      return;
        serialOut.println();
  serialOut.println("closing connection");
    }
  }
  /* receive data from server
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    serialOut.print(line);
  }
  */


  /*
if(ok){
  digitalWrite(CH1, HIGH); //sets arduino pin #10 to HIGH
  delay(1000);
  digitalWrite(CH1, LOW);   //stops opening door circuit
}
*/
}
