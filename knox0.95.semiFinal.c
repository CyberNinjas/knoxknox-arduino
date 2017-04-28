//==============================================================================//==============================================================================
// KNOX-KNOX
// DEVELOPED FOR CYBER NINJAS, SARASOTA FLORIDA
// BY STUDENTS FROM THE UNIVERSITY OF SOUTH FLORIDA:
//    COPYRIGHT 2017 BRENDON BOLLES, STEPHEN DARIES, ANDREW KIVELA, ZACHARY STANDRIDGE
//		WRITTEN IN XCODE, SUBLIME TEXT, ARDUINO IDE
//============================================================================/V0.95/===========================================================================

//==============================================================================//==============================================================================
/* 
TO DO: REDUCE, REUSE, RECYCLE. 
	SWAP ARDUINO FOR RASPI, SWAP PINPAD FOR NFC CAPABLE DEVICE.
	GET RID OF GLOBAL VARIABLES???
		UNFORTUNATELY... RESEARCH SHOWS GLOBAL VARIABLES PERFORM BETTER ON ARDUINO...
*/
//==============================================================================//==============================================================================

//==============================================================================//==============================================================================
/* 
DIRECTIONS: INSTALL ARDUINO
			ADD WIEGAND_KNOX TO ARDUINO LIBRARIES
			COPY CODE TO ARDUINO IDE
			CHECK FOR COMPILE ISSUES
			COMPILE AND UPLOAD
			DEVICE IS READY ONCE UPLOAD COMPLETES
			SERIAL MONITOR WILL OUTPUT RELEVANT DATA FOR DEBUG/VERIFICATION
*/
//==============================================================================//==============================================================================


//==============================================================================//==============================================================================
//BEGIN KNOX KNOX 2 FACTOR AUTHENTICATING DOOR LOCK!//==========================================================================================================
//==============================================================================//==============================================================================


//==============================================================================//==============================================================================
// INCLUDES AND DEFINES
//==============================================================================//==============================================================================

//INCLUDE ETHERNET LIBRARIES
#include <SPI.h>
#include <Ethernet2.h>

//INCLUDE SEMI-CUSTOM WIEGAND LIBRARY
#include <Wiegand_Knox.h>

//DEFINE HARDWARE PINS
#define LOCK_OUT 6 //LOCK OUT LED
#define PIN_LIGHT 12 //PIN STATE RUNNING LED
#define KEY_LIGHT 11 // KEY STATE RUNNING LED
#define ATTEMPT1 4 //FIRST ATTEMPT LED
#define ATTEMPT2 9 //SECOND ATTEMPT LED
#define ATTEMPT3 8 //THIRD ATTEMPT LED
#define NETWORK 7 // WHITE LED 
#define RELAY_CH 13 //DOOR LOCK RELAY

//==============================================================================//==============================================================================
// ETHERNET SETUP
//==============================================================================//==============================================================================
//INITIALIZE THE ON BOARD ETHERNET ADAPTER
EthernetClient client;

//AZURE VARIABLES
//AZURE SERVICES ADDRESS
char server[] = "knoxknox-backend-dev2.azurewebsites.net";

//MAC ADDRESS OF DEVICE
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE};

//==============================================================================//==============================================================================
// WIEGAND SETUP
//==============================================================================//==============================================================================

//INITIALIZE THE WIEGAND KEY PAD
WiegandKnox wg;

//==============================================================================//==============================================================================
// VARIABLES
//==============================================================================//==============================================================================

//STATE MACHINE
bool keystate = true; //State 1
bool pinstate = false; //State 2
bool unlockstate = false; //Unlock State

//YUBIKEY
long yubikeyTMP = NULL;
long yubikey = NULL;

//PINPAD
int pinTMP = NULL;
int pincount = 0;
int pinKey = NULL;
int pin[6] = {NULL};

//PIN CONVERSION
int PINin = 0;
long PINout = 0;

//ACCESS CONTROL
int accessAttempts = 0;
int type = 0;

//LOCAL AUTH STATES
bool keyPass = false;
bool pinPass = false;

//AUTHENTICATION TEST DATA
//long key1Test = 787592;
//int pin1Test[6] = {1, 2, 3, 4, 5, 6};
//long pin1Test = 123456;

char serveResp[10];

//==============================================================================//==============================================================================
// BEGIN SETUP FUNCTION
// THIS RUNS AT STARTUP BEFORE LOOP STATE FUNCTION
//==============================================================================//==============================================================================
void setup()
{
  delay(100);
  //SERIAL OUTPUT 
  Serial.begin(57600);
  delay(5000);
  Serial.println("Hello!");
  Serial.println("Arduino started");

  //SET LIGHTS
  Serial.println("Setting lights");
  Serial.println("Ready...");
  digitalWrite(KEY_LIGHT, HIGH);
  digitalWrite(PIN_LIGHT, LOW);
  digitalWrite(ATTEMPT1, LOW);
  digitalWrite(ATTEMPT2, LOW);
  digitalWrite(ATTEMPT3, LOW);
  digitalWrite(LOCK_OUT, LOW);

  //CHECK FOR A NETWORK CONNECT
  Serial.println("Checking for a network connection");
  //PING SERVER TO WAKE IT UP -- THIS DOESN'T ALWAYS WORK
  //IF THIS DOESNT WORK THE SERVER WILL BE ASLEEP WHEN DATA IS CHECKED AND THE FIRST ATTEMPT TO UNLOCK DOOR WILL ALWAYS RESULT IN A NOACK
  client.connect(server, 80);

  //BEGIN KEYPAD FUNCTIONALITY
  wg.begin();
  Serial.println("Keypad enabled");
  Serial.println("Ready...");
}


//==============================================================================//==============================================================================
// BEGIN LOOP STATE FUNCTION
// THIS IS THE MAIN ARDUINO STATE FUNCTIONALITY
//==============================================================================//==============================================================================
void loop()
{
  //resetState();
  if(accessAttempts == 0)
  {
    digitalWrite(ATTEMPT1, LOW);
    digitalWrite(ATTEMPT2, LOW);
    digitalWrite(ATTEMPT3, LOW);
  }
  else if(accessAttempts == 1)
  {
    digitalWrite(ATTEMPT1, HIGH);
    digitalWrite(ATTEMPT2, LOW);
    digitalWrite(ATTEMPT3, LOW);
  }
  else if(accessAttempts == 2)
  {
    digitalWrite(ATTEMPT1, HIGH);
    digitalWrite(ATTEMPT2, HIGH);
    digitalWrite(ATTEMPT3, LOW);
  }
  else if(accessAttempts == 3)
  {
    digitalWrite(ATTEMPT1, HIGH);
    digitalWrite(ATTEMPT2, HIGH);
    digitalWrite(ATTEMPT3, HIGH);
    lockOut();
  }

  //S1
  //STATE 1: GET KEY DATA FROM USER
  while(keystate == true && yubikeyTMP == NULL)
  {
    Serial.print("Checking for key");
    delay(200);
    Serial.print(".");
    delay(200);
    Serial.print(".");
    delay(200);
    Serial.print(".\n");
    delay(200);
    //CALL GET KEY CHECKS TO SEE IF A KEY IS PRESENT
    getKey();
    if(type != 26)
    {
      getKey();
    }
    //MANIPULATES THE DATA SO THAT IT IS ONLY READ IN ONCE
    yubikey = yubikeyTMP;
    yubikeyTMP = NULL;

    //IF KEY IS READ, LOOK FOR PIN INPUT
    if(yubikey != NULL && type == 26)
    {
      //TURN KEY LIGHT OFF
      digitalWrite(KEY_LIGHT, LOW);
      //TURN PIN LIGHT ON
      digitalWrite(PIN_LIGHT, HIGH);
      getPin();
    }
  }


  //S2
  //STATE 2: GET PIN DATA FROM USER
  while(pinstate == true && pinTMP == NULL && pincount < 6)
  {
    getPin();
    pinTMP = NULL;
    if(pinTMP != NULL && type == 4)
    {
      getPin();
    }
  }
}
//==============================================================================//==============================================================================
// END LOOP STATE FUNCTIONS
//==============================================================================//==============================================================================


//==============================================================================//==============================================================================
// BEGIN USER DEFINED FUNCTIONS
//==============================================================================//==============================================================================

//==============================================================================
// STATE 1: GET KEY DATA FROM USER
//==============================================================================
void getKey()
{
 if(wg.available())
  {
    //Pause interupts
    wg.pause();
    delay(250);
    yubikeyTMP = wg.getCode();
    type = 0;
    type = wg.getWiegandType();
    Serial.println("TYPE: \n");
    Serial.println(type);
    //IF INVALID KEY TYPE IGNORE INPUT
    if(type != 26)
    {
      Serial.println("INVALID KEY TYPE\n");
      yubikeyTMP = NULL;
      getKey();
    }
    //IF NO KEY READ KEEP CHECKING
    if(yubikeyTMP == NULL)
    {
      Serial.println("No Key Read\n");
      yubikeyTMP = wg.getCode();
      getKey();
    }
    //IF CORRECT KEY TYPE READ
    if(yubikeyTMP != NULL && type == 26)
    {
      yubikey = yubikeyTMP;
      Serial.println("Key Read!\n");
      Serial.println("Key Value: \n");
      Serial.println(yubikey);
      keystate = false;
      pinstate = true;
    }
    //Clear data and reenable interupts
    wg.clear();
    delay(1000);
  }
}

//==============================================================================
// STATE 2: GET PIN DATA FROM USER
//==============================================================================
//Check for pin data

void getPin()
{
  delay(200);
  Serial.println("Checking for pin...\n");
  delay(200);

 if(wg.available())
  {
    //Pause interupts
    wg.pause();
    delay(200);
    pinTMP = wg.getCode();
    type = 0;
    type = wg.getWiegandType();
    Serial.println("TYPE: \n");
    Serial.println(type);
    if(type != 4)
    {
      Serial.println("INVALID PIN TYPE\n");
      pinTMP = NULL;
      getPin();
    }
    if(pinTMP == NULL)
    {
      Serial.println("No Pin Read\n");
      getPin();
    }
    if(pinTMP != NULL && type == 4)
    {
      pinKey = pinTMP;
      Serial.println("Pin Read!\n");
      Serial.println("Pin Value: \n");
      Serial.println(pinKey);
      pin[pincount] = pinKey;
      pincount = pincount + 1;
    }
    //Clear data and reenable interupts
    wg.clear();
    delay(200);
  }

  if(pincount == 6)
  {
    //CONVERT ARRAY TO LONG INT
    for(PINin = 0; PINin < pincount; PINin++)
    {
      PINout = (10 * PINout) + pin[PINin];
    }

    Serial.println("\nCOMPLETE INTEGER PIN: ");
    Serial.print(PINout);
    //TURN ON PIN AND KEY LIGHT
    digitalWrite(KEY_LIGHT, HIGH);
    //TURN PIN LIGHT ON
    digitalWrite(PIN_LIGHT, HIGH);
    Serial.println("\nTesting Data... \n");
    delay(5000);
    pinstate = false;
    httpPOST();
  }
}

//==============================================================================
// STATE 3: CHECK AGAINST AZURE
//==============================================================================
void httpPOST() 
{
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for (;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
  client.stop();
  String userName = "{\"yubiKey\" : \"";
  String passWord = "pin\" : \"";
  String user = String(yubikey);
  String pass = String(PINout);
  String webData;
  String quoteData;
  //{"yubikey" : "yubikeyVal", "pin" : "pinVal"}
  
  userName = userName + user;
  userName = userName + "\", \"";
  passWord = passWord + pass;
  webData = userName + passWord;
  webData = webData + "\"}";
  
  Serial.println("\nconnecting...\n");
  // if there's a successful connection:
  if (client.connect(server, 80))
  {
    digitalWrite(NETWORK, HIGH);
    Serial.println("\nSending this POST data: \n");
    Serial.println("POST /api/ HTTP/1.1");
    Serial.println("Host: knoxknox-backend-dev2.azurewebsites.net");
    Serial.println("Connection: close");
    Serial.println("User-Agent: Arduino/1.0");
    Serial.println("Content-type: application/json");
    Serial.print("Content-Length: ");
    Serial.println(webData.length());
    Serial.println();
    Serial.println(webData);
    Serial.println("SENDING!");
    Serial.println();
    //send the HTTP POST request:
    client.println("POST /api/ HTTP/1.1");
    client.println("Host: knoxknox-backend-dev2.azurewebsites.net");
    client.println("Connection: close");
    client.println("User-Agent: Arduino/1.0");
    client.println("Content-type: application/json");
    client.print("Content-Length: ");
    client.println(webData.length());
    client.println();
    client.println(webData);
    //client.println("Connection: close");
    delay(1000);

    serverResponse();
  }
  else {
    // if you couldn't make a connection:
    digitalWrite(NETWORK, LOW);
    Serial.println("connection failed");
    resetState();
  }
}

//==============================================================================
// STATE 4: GET AZURE RESPONSE
//==============================================================================
void serverResponse()
{
//HTTP Post Response Data
  char c = '!';
  int i = 0;
  int respCount = 0;
  if(client.available())
  {
    while(c != '*')
    {
      c = client.read();
      
      if(c == '*')
      {
        break;
      }
      
      if(c == '[')
      { 
        while(c != ']')
        {
          c = client.read();
          if(c == '[' || c == ']'){ c = client.read();}
          if(c == '*') {break;}
          Serial.write(c);
          serveResp[i] = c;
          i++;
          respCount++;
          //c = client.read();
        }
      }
      Serial.write(c);
    }
  }
  Serial.println("\nPrinting Server ACK/NOACK \n");
  Serial.println("\nResponse Count:  \n");
  Serial.println(respCount);
  for(i = 0; i <= respCount; i++)
  {
    Serial.write(serveResp[i]);
  }
  if(respCount == 5)
  {
    accessAttempts = accessAttempts + 1;
  }
  if(respCount == 3)
  {
    unlockDoor();
  }
  Serial.println();
  Serial.println();
  resetState();
}


//==============================================================================
// STATE 5: UNLOCK DOOR OR HARDLOCK DOOR OR RESET LOCK
//==============================================================================
void unlockDoor()
{
  digitalWrite(RELAY_CH, HIGH); //sets arduino pin #6 to HIGH to open lock
  Serial.println("Opening door");
  accessAttempts = 0;
  delay(10000); //open for 10 seconds
  digitalWrite(RELAY_CH, LOW);   //stops opening door circuit
  resetState();
}

void resetState()
{
  //STATE MACHINE
  keystate = true; //State 1
  pinstate = false; //State 2
  unlockstate = false; //State 8

  //YUBIKEY
  yubikeyTMP = NULL;
  yubikey = NULL;

  //PINPAD
  pinTMP = NULL;
  pincount = 0;
  pinKey = NULL;
  pin[6] = {NULL};

  //PIN CONVERSION
  PINin = 0;
  PINout = 0;

  //LIGHTS RESET
  digitalWrite(KEY_LIGHT, HIGH);
  digitalWrite(PIN_LIGHT, LOW);
  digitalWrite(ATTEMPT1, LOW);
  digitalWrite(ATTEMPT2, LOW);
  digitalWrite(ATTEMPT3, LOW);
  digitalWrite(LOCK_OUT, LOW);
  digitalWrite(NETWORK, LOW);

  //LOCAL AUTHENTICATION
  //keyPass = false;
  //pinPass = false;

  //AUTHENTICATION TEST DATA
  //key1Test = 787592;
  //int pin1Test[6] = {1, 2, 3, 4, 5, 6};
  //pin1Test = 123456;
  loop();
}

void lockOut()
{
  Serial.println("\n Door Locked! \n");
  Serial.println("\n MAX ATTEMPTS EXCEEDED! \n");
  digitalWrite(KEY_LIGHT, LOW);
  digitalWrite(PIN_LIGHT, LOW);
  digitalWrite(ATTEMPT1, HIGH);
  digitalWrite(ATTEMPT2, HIGH);
  digitalWrite(ATTEMPT3, HIGH);
  digitalWrite(LOCK_OUT, HIGH);
  digitalWrite(NETWORK, LOW);
  delay(180000);
  accessAttempts = 0;
  resetState();
}

//==============================================================================//==============================================================================
//END OF KNOX KNOX VIABLE CODE. ALL CODE BELOW THIS LINE IS PURELY TESTBED DEBUGGING DATA//=====================================================================
//==============================================================================//==============================================================================


//==============================================================================//==============================================================================
// END KNOX KNOX
//==============================================================================//==============================================================================

//==============================================================================
//TEST KEY DATA
//==============================================================================

//TEST KEYS
/*
OUTPUT 787592: Key Serial SN4546427
OUTPUT 2753672: Key Serial SN4546469
OUTPUT 3540104: Key Serial SN4882903
*/
