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
#define CH1 13 //Ch1=D13, output to door lock relay
#define CH2 10 //Ch2=D10, output to keypad LED
int pinStr[6] = {1,2,3,6,5,4};
int index;


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
index = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
/*
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
  delay(500);
if(wg.available())
  {
keyString = wgData();
  }
  
  if(keyString > 27){
    tagCode = whoKey();
    keyString =0;
  }
 
  
  
if(keyString == asterisk){
    Serial.println("Enter code");
    //index=0;
    getKey();
    keyString = 0;
  
}

  if(keyString == 13){
  showCode();
  }


}

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
    return 0;
    break;
   }
}
void open(){
  digitalWrite(CH1, HIGH); //sets arduino pin #13 to HIGH
  Serial.println("open");
  delay(1000);
  digitalWrite(CH1, LOW);   //stops opening door circuit

}
void getKey(){
  //int index = index;
  long local = 0;
  
  //local = wg.getCode();
  for (int i = 0; i < 6; i++){
    //delay(500);
    local = wgData();
    pinStr[i] = local;
    //index++;
    Serial.print(index);
    Serial.println(local);
    keyString = 0;
    //return index;
}}

void showCode(){
for (int i = 0; i < 6; i++){
   Serial.print(pinStr[i]," ");
   delay(50);
   keyString=0;
   }
Serial.println();
}
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
void blinkThree(){
  Serial.println("User not recognized");
  digitalWrite(CH2, HIGH);
  delay(300);
    digitalWrite(CH2, LOW);
  delay(200);
    digitalWrite(CH2, HIGH);
  delay(300);
    digitalWrite(CH2, LOW);
  delay(200);
    digitalWrite(CH2, HIGH);
  delay(300);
    digitalWrite(CH2, LOW);
  delay(200);

}

