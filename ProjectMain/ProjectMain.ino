//#include <SoftwareSerial.h>
#include <Wiegand.h>
WIEGAND wg; //catches data from keypad
//SoftwareSerial Serial(14, 12, false, 256); //save for possible future use 
/*
long keyA = 787592; //SN4546427
long keyB = 2753672; //SN4546469
long keyC = 3540104; //SN4882903 -> Andrew's key
long blackList = 2496273;
*/
long tagCode =0;
long asterisk = 27;
long keyString =0;

boolean flagDone = false;
unsigned int wiegand_counter;


void setup() {
//get serial going
  delay(10);
  Serial.begin(9600);
  Serial.println("Hello!");
  Serial.println("Arduino started");
  delay(10);
  wg.begin();
  Serial.println("Keypad enabled");
  delay(10);

//initialize display 

//setup relay
/*
 #define CH1 10 //CH# ardPin#
 */
}

void loop() {
  // put your main code here, to run repeatedly:

  if(wg.available())
  {
    Serial.print("Wiegand HEX = ");
    Serial.print(wg.getCode(),HEX);
    Serial.print(", DECIMAL = ");
    //strcpy(keyString, wg.getCode());
    //
    keyString = wg.getCode();
    Serial.print(keyString);
    Serial.print(", Type W");
    Serial.println(wg.getWiegandType());    
  } 
  
  if(keyString > 27){
    tagCode = whoKey();
    keyString =0;
  }
 
  
  
if(keyString == asterisk){
    Serial.println("Enter code");
    keyString = 0;
    //keyString = wg.getCode();
    //Serial.println(keyString);
  
}

  /*
if(ok){
  digitalWrite(CH1, HIGH); //sets arduino pin #10 to HIGH
  delay(1000);
  digitalWrite(CH1, LOW);   //stops opening door circuit
}
*/
}

long whoKey(){
  switch(keyString) {
    Serial.println(keyString);
    case 787592:
    Serial.println("Doug's key SN4546427");
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
    return 0;
    break;
   }
}
void getCode(){
  
}