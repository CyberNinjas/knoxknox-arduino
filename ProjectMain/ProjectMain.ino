//#include <SoftwareSerial.h>
#include <Wiegand.h>
WIEGAND wg; //initialize Wiegand system
//SoftwareSerial Serial(14, 12, false, 256); //save for possible future use 
/* Serial numbers for each key we have
long keyA = 787592; //SN4546427
long keyB = 2753672; //SN4546469
long keyC = 3540104; //SN4882903 -> Andrew's key
long blackList = 2496273;
*/

#define RELAY_CH 13 //pin D13, output to door lock relay
#define LED_CH 10 //pin D10, output to keypad LED
/*
//#define KEYPAD_D0 2 
// pin D2, input from keypad D0 (work in progress)
//#define KEYPAD_D1 3 
// pin D3, input from keypad D1 (WIP)
*/
/* Global variables, use long when pulling from keypad to get full yubi serial */
long tagCode =0; //Yubi serial number
long asterisk = 27; //* key
long pound = 13; //# key
long keyString =0; //input kept here
int pinStr[6] = {1,2,3,6,5,4}; //PIN keeper
int index;


void setup() {
/*get serial going, let serial know what's up */
  delay(100); //let system voltages settle
  Serial.begin(9600); //serial baud
  Serial.println("Hello!");
  Serial.println("Arduino started");
  wg.begin(); //start keypad
  Serial.println("Keypad enabled");
index = 0;
}

void loop() {
/*  code runs reatedly
this is the standard method used to poll the keypad from the Wiegand example but the decimal value is stored in keyString
  if(wg.available())
  {
    Serial.print("Wiegand HEX = ");
    Serial.print(wg.getCode(),HEX);
    Serial.print(", DECIMAL = ");
    keyString = wg.getCode();
    Serial.print(keyString);
    Serial.print(", Type W");
    Serial.println(wg.getWiegandType());    
  } 
  */
  /*setup interrupts for keypad, a better way of taking data from keypad (work in progress)
  
  pinMode(PIN_D0, INPUT);
  pinMode(PIN_D1, INPUT);
  setInterrupt(digitalPinToInterrupt(PIN_D0), pinChange, CHANGE);
  setInterrupt(digitalPinToInterrupt(PIN_D1), pinChange, CHANGE);
  */
  delay(500);
if(wg.available())
  {
keyString = wgData();
  }
  /*keyStrings over 27 are codes from the yubikeys */
  if(keyString > 27){
    tagCode = whoKey();
    keyString =0;
  }
 
  
 /*start taking code when * is hit */
if(keyString == asterisk){
    Serial.println("Enter code");
    //index=0;
    getKey();
    keyString = 0;
  
}
/*prints the pin to serial when # is pressed */
  if(keyString == pound){
  showCode();
  }


}
/*check for key owner */
long whoKey(){
  switch(keyString) {
    Serial.println(keyString);
    case 787592:
    Serial.println("Doug's key SN4546427");
    open();
    return keyString;
    break;
    case 2753672:
    Serial.println("Ben's key SN4546469");
    return keyString;
    break;
    case 3540104:
    Serial.println("Andrew's key SN4882903");
    return keyString;
    break;
    case 2496273:
    Serial.println("Blacklisted card");
    blinkThree();
    return 0;
    break;
   }
}
void open(){
  digitalWrite(RELAY_CH, HIGH); /*sets arduino pin #13 to HIGH to open lock*/
  Serial.println("Opening door");
  delay(3000); /*open for 3 seconds*/
  digitalWrite(RELAY_CH, LOW);   /*stops opening door circuit*/

}
/*attempts to get user pin from keypad, doesn't work right. Needs to be run via an interrupt*/
void getKey(){

  long local = 0;
  
  /*local = wg.getCode();*/
  for (int i = 0; i < 6; i++){

    local = wgData();
    pinStr[i] = local;
    Serial.print(index);
    Serial.println(local);
    keyString = 0;

}}

/*prints user pin to serial*/
void showCode(){
for (int i = 0; i < 6; i++){
   Serial.print(pinStr[i]);
   delay(50);
   keyString=0;
   }
Serial.println();
}

/*get data from keypad and show various forms on serial*/
long wgData(){
long keyData =0;
    Serial.print("Wiegand HEX = ");
    Serial.print(wg.getCode(),HEX);
    Serial.print(", DECIMAL = ");
    keyData = wg.getCode();
    Serial.print(keyString);
    Serial.print(", Type W");
    Serial.println(wg.getWiegandType());    

  return keyData;
}

/*blinks keypad LED when user not recognized or unathorized, LED not connected yet*/
void blinkThree(){
  Serial.println("User not recognized");
  digitalWrite(LED_CH, HIGH);
  delay(300);
    digitalWrite(LED_CH, LOW);
  delay(200);
    digitalWrite(LED_CH, HIGH);
  delay(300);
    digitalWrite(LED_CH, LOW);
  delay(200);
    digitalWrite(LED_CH, HIGH);
  delay(300);
    digitalWrite(LED_CH, LOW);
  delay(200);

}
